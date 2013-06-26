// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "GpuIpc/IProtocol.h"
Tracker::Tracker(IProtocol & protocol) : protocol(protocol) {
}
// service function wrappers
void Tracker::searchByPair(const PixelEvent & event, std::vector<GpuTrack> & tracks) {
  protocol.writeInt32(TrackerID_searchByPair);
  // Only write one
  /*protocol.writeInt32(event.size());
  for (int i = 0, size = event.size(); i != size; ++i) {
    pixelIO.write(protocol, event[i]);
  }*/
  event.write(protocol);
  tracks.resize(protocol.readInt32());
  for (int i = 0, size = tracks.size(); i != size; ++i) {
    tracks[i].read(protocol);
  }
}
