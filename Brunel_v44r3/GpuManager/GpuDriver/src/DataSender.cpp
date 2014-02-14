#include "DataSender.h"
#include "Timer.h"

#include "GpuIpc/SocketClient.h"
#include "GpuIpc/Protocol.h"

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
    int                       index,
    const char              * servicePath,
    vector<directory_entry> & paths,
    mutex                   & pathsMutex) :
    m_index     (index),
    m_paths     (paths),
    m_mutex     (pathsMutex),
    m_transport (new SocketClient(servicePath)),
    m_protocol  (new Protocol(*m_transport)) {
}

void DataSender::operator() () {
  Timer timer(true);
  timer.start();

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
    vector<uint8_t> data;
    readData(path.c_str(), handlerName, data);

    // send the name of the addressee
    m_protocol->writeString(handlerName);

    // send a flag indicating whether profiling is eanbled
    m_protocol->writeBool(true);

    // send the data package
    m_protocol->writeUInt32(data.size());
    if (!data.empty())
      m_protocol->writeData(&data[0], data.size());

    // receive the result
    size_t resultSize = m_protocol->readUInt32();

    // handle errors
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
      m_protocol->readData(&result[0], resultSize);

    timer.stop();

    // send performance data
    m_protocol->writeDouble(timer.secondsElapsed());
  }
}

void DataSender::readData(
    const char      * path,
    string          & handlerName,
    vector<uint8_t> & data) {
  ifstream stream(path, ios_base::binary);

  uint32_t handlerSize;
  readStream(stream, &handlerSize, 4);

  vector<char> handlerChars(handlerSize + 1); // +1 for terminating zero
  readStream(stream, &handlerChars[0], handlerSize);
  handlerName = &handlerChars[0];

  uint32_t dataSize;
  readStream(stream, &dataSize, 4);

  data.resize(dataSize);
  if (!data.empty())
    readStream(stream, &data[0], dataSize);
}
