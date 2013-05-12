// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "GpuIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
};
// service function wrappers
void Tracker::searchByPair(std::vector<int8_t> data, std::vector<GpuTrack> & result) {
  protocol.writeInt32(TrackerID_searchByPair);
  protocol.writeInt32(data.size());
  for (int i = 0, size = data.size(); i != size; ++i) {
    protocol.writeInt8(data[i]);
  }
  result.resize(protocol.readInt32());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i].read(protocol);
  }
};
