// Automatically generated file
#include "api/GpuTrack.h"
#include "IProtocol.h"
void GpuTrack::read(IProtocol & protocol) {
  X0 = protocol.readDouble();
  Tx = protocol.readDouble();
  Y0 = protocol.readDouble();
  Ty = protocol.readDouble();
  S0 = protocol.readDouble();
  Sx = protocol.readDouble();
  Sz = protocol.readDouble();
  Sxz = protocol.readDouble();
  Sz2 = protocol.readDouble();
  U0 = protocol.readDouble();
  Uy = protocol.readDouble();
  Uz = protocol.readDouble();
  Uyz = protocol.readDouble();
  Uz2 = protocol.readDouble();
  TrackHitsNum = protocol.readInt32();
  Hits.resize(protocol.readInt32());
  for (int i = 0, size = Hits.size(); i != size; ++i) {
    Hits[i] = protocol.readInt32();
  }
}
void GpuTrack::write(IProtocol & protocol) const {
  protocol.writeDouble(X0);
  protocol.writeDouble(Tx);
  protocol.writeDouble(Y0);
  protocol.writeDouble(Ty);
  protocol.writeDouble(S0);
  protocol.writeDouble(Sx);
  protocol.writeDouble(Sz);
  protocol.writeDouble(Sxz);
  protocol.writeDouble(Sz2);
  protocol.writeDouble(U0);
  protocol.writeDouble(Uy);
  protocol.writeDouble(Uz);
  protocol.writeDouble(Uyz);
  protocol.writeDouble(Uz2);
  protocol.writeInt32(TrackHitsNum);
  protocol.writeInt32(Hits.size());
  for (int i = 0, size = Hits.size(); i != size; ++i) {
    protocol.writeInt32(Hits[i]);
  }
}
