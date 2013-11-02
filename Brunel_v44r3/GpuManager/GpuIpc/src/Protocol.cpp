#include "Protocol.h"

#include "ITransport.h"

#include <vector>

using namespace std;

//-----------------
// public interface
//-----------------

Protocol::Protocol(ITransport & transport) :
    m_transport(transport) {
}

Protocol::~Protocol() {
}

//-------------------------
// IProtocol implementation
//-------------------------

uint32_t Protocol::readUInt32() {
  uint32_t n = 0;
  m_transport.readBytes(reinterpret_cast<void*>(&n), 4);
  return n;
}

void Protocol::writeUInt32(uint32_t n) {
  m_transport.writeBytes(reinterpret_cast<void*>(&n), 4);
}

string Protocol::readString() {
  uint32_t length = readUInt32();
  vector<char> text(length);
  m_transport.readBytes(&text[0], length);
  return &text[0];
}

void Protocol::writeString(const string & str) {
  writeUInt32(str.size());
  m_transport.writeBytes(str.c_str(), str.size());
}

void Protocol::readData(void * data, size_t size) {
  m_transport.readBytes(data, size);
}

void Protocol::writeData(const void * data, size_t size) {
  m_transport.writeBytes(data, size);
}
