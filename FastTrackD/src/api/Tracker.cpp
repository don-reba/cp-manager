// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"
// IProcess implementation
bool Tracker::process(IProtocol & protocol) const {
  int id = protocol.readInt32();
  switch (id) {
  case TrackerID_searchByPair:
    process_searchByPair(protocol);
    break;
  default:
    throw std::runtime_error("Tracker::process: unknown ID");
  }
  return true;
}
// service function wrappers
void Tracker::process_searchByPair(IProtocol & protocol) const {
  std::vector<GpuTrack> data;
  data.resize(protocol.readInt32());
  for (int i = 0, size = data.size(); i != size; ++i) {
    data[i].read(protocol);
  }
  std::vector<int8_t> result;
  this->searchByPair(data, result);
  protocol.writeInt32(result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    protocol.writeInt8(result[i]);
  }
}
