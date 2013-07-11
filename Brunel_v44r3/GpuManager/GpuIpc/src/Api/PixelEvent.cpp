// Automatically generated file
#include "Api/PixelEvent.h"
#include "IProtocol.h"
void PixelEvent::read(IProtocol & protocol) {
  noSensors = protocol.readInt32();
  noHits = protocol.readInt32();
  sensorZs.resize(protocol.readInt32());
  for (int i = 0, size = sensorZs.size(); i != size; ++i) {
    sensorZs[i] = protocol.readInt32();
  }
  sensorHitStarts.resize(protocol.readInt32());
  for (int i = 0, size = sensorHitStarts.size(); i != size; ++i) {
    sensorHitStarts[i] = protocol.readInt32();
  }
  sensorHitsNums.resize(protocol.readInt32());
  for (int i = 0, size = sensorHitsNums.size(); i != size; ++i) {
    sensorHitsNums[i] = protocol.readInt32();
  }
  hitIDs.resize(protocol.readInt32());
  for (int i = 0, size = hitIDs.size(); i != size; ++i) {
    hitIDs[i] = protocol.readInt32();
  }
  hitXs.resize(protocol.readInt32());
  for (int i = 0, size = hitXs.size(); i != size; ++i) {
    hitXs[i] = protocol.readFloat();
  }
  hitYs.resize(protocol.readInt32());
  for (int i = 0, size = hitYs.size(); i != size; ++i) {
    hitYs[i] = protocol.readFloat();
  }
  hitZs.resize(protocol.readInt32());
  for (int i = 0, size = hitZs.size(); i != size; ++i) {
    hitZs[i] = protocol.readInt32();
  }
}
void PixelEvent::write(IProtocol & protocol) const {
  protocol.writeInt32(noSensors);
  protocol.writeInt32(noHits);
  protocol.writeInt32(sensorZs.size());
  for (int i = 0, size = sensorZs.size(); i != size; ++i) {
    protocol.writeInt32(sensorZs[i]);
  }
  protocol.writeInt32(sensorHitStarts.size());
  for (int i = 0, size = sensorHitStarts.size(); i != size; ++i) {
    protocol.writeInt32(sensorHitStarts[i]);
  }
  protocol.writeInt32(sensorHitsNums.size());
  for (int i = 0, size = sensorHitsNums.size(); i != size; ++i) {
    protocol.writeInt32(sensorHitsNums[i]);
  }
  protocol.writeInt32(hitIDs.size());
  for (int i = 0, size = hitIDs.size(); i != size; ++i) {
    protocol.writeInt32(hitIDs[i]);
  }
  protocol.writeInt32(hitXs.size());
  for (int i = 0, size = hitXs.size(); i != size; ++i) {
    protocol.writeFloat(hitXs[i]);
  }
  protocol.writeInt32(hitYs.size());
  for (int i = 0, size = hitYs.size(); i != size; ++i) {
    protocol.writeFloat(hitYs[i]);
  }
  protocol.writeInt32(hitZs.size());
  for (int i = 0, size = hitZs.size(); i != size; ++i) {
    protocol.writeInt32(hitZs[i]);
  }
}
