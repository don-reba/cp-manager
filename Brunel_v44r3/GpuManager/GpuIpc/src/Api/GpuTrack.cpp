// Automatically generated file
#include "Api/GpuTrack.h"
#include "IProtocol.h"
void GpuTrack::read(IProtocol & protocol) {
  x0 = protocol.readFloat();
  tx = protocol.readFloat();
  y0 = protocol.readFloat();
  ty = protocol.readFloat();
  s0 = protocol.readFloat();
  sx = protocol.readFloat();
  sz = protocol.readFloat();
  sxz = protocol.readFloat();
  sz2 = protocol.readFloat();
  u0 = protocol.readFloat();
  uy = protocol.readFloat();
  uz = protocol.readFloat();
  uyz = protocol.readFloat();
  uz2 = protocol.readFloat();
  trackHitsNum = protocol.readInt32();
  hits.resize(protocol.readInt32());
  for (int i = 0, size = hits.size(); i != size; ++i) {
    hits[i] = protocol.readInt32();
  }
}
void GpuTrack::write(IProtocol & protocol) const {
  protocol.writeFloat(x0);
  protocol.writeFloat(tx);
  protocol.writeFloat(y0);
  protocol.writeFloat(ty);
  protocol.writeFloat(s0);
  protocol.writeFloat(sx);
  protocol.writeFloat(sz);
  protocol.writeFloat(sxz);
  protocol.writeFloat(sz2);
  protocol.writeFloat(u0);
  protocol.writeFloat(uy);
  protocol.writeFloat(uz);
  protocol.writeFloat(uyz);
  protocol.writeFloat(uz2);
  protocol.writeInt32(trackHitsNum);
  protocol.writeInt32(hits.size());
  for (int i = 0, size = hits.size(); i != size; ++i) {
    protocol.writeInt32(hits[i]);
  }
}
