// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "GpuIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
}
// service function wrappers
void Tracker::searchByPair(const PixelEvent & data, std::vector<GpuTrack> & result) {
  protocol.writeInt32(TrackerID_searchByPair);
  data.write(protocol);
}
