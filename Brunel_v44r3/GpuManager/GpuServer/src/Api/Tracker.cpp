// Automatically generated file
#include "Tracker.h"
#include "TrackerID.h"
#include "GpuIpc/IProtocol.h"
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
  std::vector<int8_t> data;
  data.resize(protocol.readInt32());

  std::cout << "[GPU - Server] Processing chunk of " << data.size() << "B..." << std::endl;

  for (int i = 0, size = data.size(); i != size; ++i) {
    data[i] = protocol.readInt8();
  }
  std::vector<GpuTrack> result;
  this->searchByPair(data, result);

  std::cout << "[GPU - Server] Tracks formed. Sending back " << result.size() << "B..." << std::endl;

  protocol.writeInt32(result.size());
  for (int i = 0, size = result.size(); i != size; ++i) {
    result[i].write(protocol);
  }
}
