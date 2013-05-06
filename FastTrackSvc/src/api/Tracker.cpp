// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "FastTrackIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
}
// service function wrappers
void Tracker::searchByPair(const std::vector<Track> & data, std::vector<int8_t> & result) {
  protocol.writeInt32(TrackerID_searchByPair);
  protocol.writeInt32(data.size());
  for (int i = 0, size = data.size(); i != size; ++i) {
    data[i].write(protocol);
  }
  result.resize(protocol.readInt32());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i] = protocol.readInt8();
  }
}
