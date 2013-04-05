// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
};
// service function wrappers
bool Tracker::isPrime(int32_t n) {
  protocol.writeInt32(TrackerID_isPrime);
  protocol.writeInt32(n);
  bool result = protocol.readBool();
  return result;
};
std::vector<int32_t> Tracker::factor(int32_t n, FactorizationMethod method) {
  protocol.writeInt32(TrackerID_factor);
  protocol.writeInt32(n);
  protocol.writeInt32(method);
  std::vector<int32_t> result(protocol.readInt32());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i] = protocol.readInt32();
  };
  return result;
};
