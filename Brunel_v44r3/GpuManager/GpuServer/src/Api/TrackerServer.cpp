// Automatically generated file
#include "TrackerServer.h"
#include "TrackerID.h"
#include "GpuIpc/IProtocol.h"
// IProcess implementation
bool TrackerServer::process(IProtocol & protocol) const {
  int id = protocol.readInt32();
  switch (id) {
  case TrackerID_searchByPair:
    process_searchByPair(protocol);
    break;
  default:
    throw std::runtime_error("TrackerServer::process: unknown ID");
  }
  return true;
}
// service function wrappers
void TrackerServer::process_searchByPair(IProtocol & protocol) const {
  PixelEvent data;
  data.read(protocol);
  std::vector<GpuTrack> result;
  this->searchByPair(data, result);
  protocol.writeInt32(result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i].write(protocol);
  }
}
