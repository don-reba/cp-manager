#include "Protocol.h"

#include "ITransport.h"

#include <vector>

using namespace std;

Protocol::Protocol(ITransport & transport) :
    m_transport(transport) {
}

Protocol::~Protocol() {
}

float Protocol::readFloat() {
  float f = 0.0f;
  m_transport.readBytes(reinterpret_cast<char*>(&f), 4);
  return f;
}

void Protocol::writeFloat(float f) {
  m_transport.writeBytes(reinterpret_cast<char*>(&f), 4);
}

int Protocol::readInt() {
  int n = 0;
  m_transport.readBytes(reinterpret_cast<char*>(&n), 4);
  return n;
}

void Protocol::writeInt(int n) {
  m_transport.writeBytes(reinterpret_cast<char*>(&n), 4);
}

string Protocol::readString() {
  int length = readInt();
  vector<char> text(length);
  m_transport.readBytes(&text[0], length);
  return string(&text[0], length);
}

void Protocol::writeString(const string & str) {
  writeInt(str.size());
  m_transport.writeBytes(str.c_str(), str.size());
}
