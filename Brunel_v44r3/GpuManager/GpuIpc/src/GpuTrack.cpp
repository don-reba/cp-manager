// Automatically generated file
#include "GpuIpc/api/GpuTrack.h"
#include "IProtocol.h"
void GpuTrack::read(IProtocol & protocol) {
  x0 = protocol.readDouble();
  tx = protocol.readDouble();
  y0 = protocol.readDouble();
  ty = protocol.readDouble();
  s0 = protocol.readDouble();
  sx = protocol.readDouble();
  sz = protocol.readDouble();
  sxz = protocol.readDouble();
  sz2 = protocol.readDouble();
  u0 = protocol.readDouble();
  uy = protocol.readDouble();
  uz = protocol.readDouble();
  uyz = protocol.readDouble();
  uz2 = protocol.readDouble();
  trackHitsNum = protocol.readInt32();
  hits.resize(protocol.readInt32());
  for (int i = 0, size = hits.size(); i != size; ++i) {
    hits[i] = protocol.readInt32();
  }
}
void GpuTrack::write(IProtocol & protocol) const {
  protocol.writeDouble(x0);
  protocol.writeDouble(tx);
  protocol.writeDouble(y0);
  protocol.writeDouble(ty);
  protocol.writeDouble(s0);
  protocol.writeDouble(sx);
  protocol.writeDouble(sz);
  protocol.writeDouble(sxz);
  protocol.writeDouble(sz2);
  protocol.writeDouble(u0);
  protocol.writeDouble(uy);
  protocol.writeDouble(uz);
  protocol.writeDouble(uyz);
  protocol.writeDouble(uz2);
  protocol.writeInt32(trackHitsNum);
  protocol.writeInt32(hits.size());
  for (int i = 0, size = hits.size(); i != size; ++i) {
    protocol.writeInt32(hits[i]);
  }
}
