#include "Protocol.h"

#include "ITransport.h"

using namespace std;

Protocol::Protocol(ITransport & transport) :
    m_transport(transport) {
}

Protocol::~Protocol() {
}

float Protocol::readFloat() {
  return 0.0f;
}

void Protocol::writeFloat(float f) {
}

int Protocol::readInt() {
  return 0;
}

void Protocol::writeInt(int n) {
}

std::string Protocol::readString() {
  return "";
}

void Protocol::writeString(const string & str) {
}
