// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"
// IProcess implementation
bool Tracker::process(IProtocol & protocol) const {
  int id = protocol.readInt32();
  switch (id) {
  case TrackerID_isPrime:
    process_isPrime(protocol);
    break;
  case TrackerID_factor:
    process_factor(protocol);
    break;
  default:
    throw std::runtime_error("Tracker::process: unknown ID");
  };
  return true;
};
// service function wrappers
void Tracker::process_isPrime(IProtocol & protocol) const {
  int32_t n = protocol.readInt32();
  bool result = this->isPrime(n);
  protocol.writeBool(result);
};
void Tracker::process_factor(IProtocol & protocol) const {
  int32_t n = protocol.readInt32();
  FactorizationMethod method = static_cast<FactorizationMethod>(protocol.readInt32());
  std::vector<int32_t> result = this->factor(n, method);
  protocol.writeInt32(result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    protocol.writeInt32(result[i]);
  };
};
