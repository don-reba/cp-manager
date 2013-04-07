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

bool Protocol::readBool() {
  bool b = false;
  m_transport.readBytes(reinterpret_cast<char*>(&b), 1);
  return b;
}

void Protocol::writeBool(bool b) {
  m_transport.writeBytes(reinterpret_cast<char*>(&b), 1);
}

double Protocol::readDouble() {
  double f = 0.0f;
  m_transport.readBytes(reinterpret_cast<char*>(&f), 8);
  return f;
}

void Protocol::writeDouble(double f) {
  m_transport.writeBytes(reinterpret_cast<char*>(&f), 8);
}

int16_t Protocol::readInt16() {
  int16_t n = 0;
  m_transport.readBytes(reinterpret_cast<char*>(&n), 2);
  return n;
}

void Protocol::writeInt16(int16_t n) {
  m_transport.writeBytes(reinterpret_cast<char*>(&n), 2);
}

int32_t Protocol::readInt32() {
  int32_t n = 0;
  m_transport.readBytes(reinterpret_cast<char*>(&n), 4);
  return n;
}

void Protocol::writeInt32(int32_t n) {
  m_transport.writeBytes(reinterpret_cast<char*>(&n), 4);
}

int64_t Protocol::readInt64() {
  int64_t n = 0;
  m_transport.readBytes(reinterpret_cast<char*>(&n), 8);
  return n;
}

void Protocol::writeInt64(int64_t n) {
  m_transport.writeBytes(reinterpret_cast<char*>(&n), 8);
}

string Protocol::readString() {
  int32_t length = readInt32();
  vector<char> text(length);
  m_transport.readBytes(&text[0], length);
  return string(&text[0], length);
}

void Protocol::writeString(const string & str) {
  writeInt32(str.size());
  m_transport.writeBytes(str.c_str(), str.size());
}
