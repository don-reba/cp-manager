#include "DataSender.h"

#include "GpuIpc/LocalSocketClient.h"
#include "GpuIpc/Protocol.h"
#include "Timer.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdint.h>
#include <vector>

using namespace boost;
using namespace boost::filesystem;
using namespace std;

void readStream(ifstream & stream, void * data, size_t size) {
  stream.read(reinterpret_cast<char *>(data), size);
}

DataSender::DataSender(
    int          index,
    const char * servicePath,
    Data       & data,
    mutex      & mutex,
    PerfLog    & perfLog) :
    m_index    (index),
    m_data     (data),
    m_mutex    (mutex),
    m_perfLog  (perfLog),
    m_transport (new LocalSocketClient(servicePath)),
    m_protocol  (new Protocol(*m_transport)) {
}

void DataSender::operator() ()
try {
  for (;;) {
    // load and send the next item from the data collection
    Item item;
    scoped_lock lock(m_mutex);
    if (m_data.empty())
      return;
    item = m_data.back();
    m_data.pop_back();

    // generate random data vector of given size
    const char *  handlerName = "SampleGpuHandler";
    vector<uint8_t> data(item.size);

    default_random_engine rand(0);
    uniform_int_distribution<uint8_t> distribution(0x00, 0xFF);
    for (int i = 0, size = item.size; i != size; ++i)
      data[i] = distribution(rand);

    // start the clock
    Timer timer;
    timer.start();

    // send the name of the addressee
    m_protocol->writeString(handlerName);

    // send data
    m_protocol->writeUInt32(data.size());
    if (!data.empty())
      m_protocol->writeData(data.data(), data.size());

    // receive the result and handle errors
    const size_t resultSize = m_protocol->readUInt32();

    const size_t FAIL_FLAG = 0xFFFFFFFF;
    if (resultSize == FAIL_FLAG) {
      string message = m_protocol->readString();
      ostringstream msg;
      msg << m_index << ": " << item.trial << item.size << item.size;
      msg << " | " << handlerName << " | error: " << message << "\n";
      cerr << msg.str();
      return;
    }

    vector<uint8_t> result(resultSize);
    if (!result.empty())
      m_protocol->readData(result.data(), resultSize);

    timer.stop();
    m_perfLog.addRecord(time(0), item.trial, item.size, timer.secondsElapsed());

    // receive performance information ­ seconds elapsed
    m_protocol->readDouble();
  }
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << endl;
} catch (...) {
  cout << "Unkonwn unrecoverable error." << endl;
}
