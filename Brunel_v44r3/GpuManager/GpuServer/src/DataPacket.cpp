#include "DataPacket.h"

#include <boost/thread/locks.hpp>

using namespace boost;
using namespace std;

DataPacket::DataPacket(const string & name, const ::Data * data, ::Data * result) :
    m_name            (name),
    m_data            (data),
    m_result          (result),
    m_seconds         (0.0),
    m_exceptionThrown (false),
    m_signalled       (false) {
}

void DataPacket::Signal() {
  {
    scoped_lock lock(m_mutex);
    m_signalled = true;
  }
  m_condition.notify_one();
}

void DataPacket::Wait() {
  scoped_lock lock(m_mutex);
  while (!m_signalled)
    m_condition.wait(lock);
  m_signalled = false;
}
void DataPacket::SetExceptionMessage(const std::string & message) {
  m_exceptionThrown  = true;
  m_exceptionMessage = message;
}
