#include "DataSender.h"

#include "GpuIpc/LocalSocketClient.h"
#include "GpuIpc/Protocol.h"
#include "Timer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>

using namespace boost;
using namespace boost::filesystem;
using namespace std;

void readStream(ifstream & stream, void * data, size_t size) {
  stream.read(reinterpret_cast<char *>(data), size);
}

DataSender::DataSender(
    int                      index,
    const char             * servicePath,
    directory_entry_vector & paths,
		vector<DiffMessage>    & diffMessages,
    mutex                  & pathsMutex,
		bool                     verifyOutput) :
    m_index        (index),
    m_paths        (paths),
		m_diffMessages (diffMessages),
    m_mutex        (pathsMutex),
    m_verifyOutput (verifyOutput),
    m_transport (new LocalSocketClient(servicePath)),
    m_protocol  (new Protocol(*m_transport)) {
}

void DataSender::operator() ()
try {
  for (;;) {
    // load and send the next item from the paths collection
    string path;
    {
      scoped_lock lock(m_mutex);
      if (m_paths.empty())
        return;
      path = m_paths.back().path().string();
      m_paths.pop_back();
    }

    ifstream stream(path.c_str(), ios_base::binary);

    string handlerName;
    vector<uint8_t> recordedInput;
    vector<uint8_t> recordedOutput;
    readData(path.c_str(), handlerName, recordedInput, recordedOutput);

    Timer timer;
    timer.start();

    // send the name of the addressee
    m_protocol->writeString(handlerName);

    // send recorded input
    m_protocol->writeUInt32(recordedInput.size());
    if (!recordedInput.empty())
      m_protocol->writeData(recordedInput.data(), recordedInput.size());

    // receive the result and handle errors
    const size_t resultSize = m_protocol->readUInt32();

    const size_t FAIL_FLAG = 0xFFFFFFFF;
    if (resultSize == FAIL_FLAG) {
      string message = m_protocol->readString();
      ostringstream msg;
      msg << m_index << ": " << path << " | " << handlerName << " | error: " << message << "\n";
      cerr << msg.str();
      return;
    }

    vector<uint8_t> result(resultSize);
    if (!result.empty())
      m_protocol->readData(result.data(), resultSize);

		// verify output
		if (m_verifyOutput) {
			string message = diff(result, recordedOutput);
			if (!message.empty()) {
				scoped_lock lock(m_mutex);
				m_diffMessages.push_back(DiffMessage(path, message));
			}
		}

    // receive performance information
    const double serverSecondsElapsed = m_protocol->readDouble();

    timer.stop();

    perfLog.addRecord(path, handlerName, timer.secondsElapsed(), serverSecondsElapsed);
  }
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << endl;
} catch (...) {
  cout << "Unkonwn unrecoverable error." << endl;
}

string DataSender::diff(
		const vector<uint8_t> & data,
		const vector<uint8_t> & reference) {
  // check size
	if (data.size() != reference.size()) {
		ostringstream message;
		message << "size mismatch: " << data.size() << " B instead of " << reference.size() << " B";
		return message.str();
	}

  // check contents
  size_t mismatchCount = 0;
  for (size_t i = 0, size = data.size(); i != size; ++i)
    mismatchCount += (data[i] != reference[i]);
	if (mismatchCount > 0) {
    ostringstream message;
    message << "data mismatch on " << mismatchCount << " bytes";
    return message.str();
  }

	return "";
}

void DataSender::readData(
    const char      * path,
    string          & handlerName,
    vector<uint8_t> & input,
		vector<uint8_t> & output) {
  ifstream stream(path, ios_base::binary);

  uint32_t handlerSize;
  readStream(stream, &handlerSize, 4);

	// read name
  vector<char> handlerChars(handlerSize + 1); // +1 for terminating zero
  readStream(stream, handlerChars.data(), handlerSize);
  handlerName = handlerChars.data();

	// read input
  uint32_t inputSize;
  readStream(stream, &inputSize, 4);

  input.resize(inputSize);
	readStream(stream, input.data(), inputSize);

	// read output
  uint32_t outputSize;
  readStream(stream, &outputSize, 4);

  output.resize(outputSize);
	readStream(stream, output.data(), outputSize);
}
