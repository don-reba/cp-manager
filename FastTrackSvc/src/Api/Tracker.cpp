// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
}
// service function wrappers
bool Tracker::isPrime(int32_t n) {
  protocol.writeInt32(TrackerID_isPrime);
  protocol.writeInt32(n);
  bool result = protocol.readBool();
  return result;
}
void Tracker::factor(int32_t n, FactorizationMethod method, std::vector<int32_t> & result) {
  protocol.writeInt32(TrackerID_factor);
  protocol.writeInt32(n);
  protocol.writeInt32(method);
  result.resize(protocol.readInt32());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i] = protocol.readInt32();
  }
}
void Tracker::searchByPair(const std::vector<int8_t> & data, std::vector<int8_t> & result) {
  protocol.writeInt32(TrackerID_searchByPair);
  protocol.writeInt32(data.size());
  for (int i = 0, size = data.size(); i != size; ++i) {
    protocol.writeInt8(data[i]);
  }
  result.resize(protocol.readInt32());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i] = protocol.readInt8();
  }
}
