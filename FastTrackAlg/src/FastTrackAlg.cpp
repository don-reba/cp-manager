#include "FastTrackAlg.h"
#include "GaudiKernel/AlgFactory.h"
#include "FastTrackSvc/IFastTrackSvc.h"

DECLARE_ALGORITHM_FACTORY(FastTrackAlg)

FastTrackAlg::FastTrackAlg(std::string name, ISvcLocator * pSL)
    : GaudiAlgorithm(name, pSL) {
}

FastTrackAlg::~FastTrackAlg() {
}

StatusCode FastTrackAlg::initialize() {
  info() << "FastTrackAlg::initialize" << endmsg;

  SmartIF<IFastTrackSvc> fastTrackSvc(svc<IFastTrackSvc>("FastTrackSvc", true));
  info() << "got FastTrackSvc" << endmsg;

  std::vector<int8_t> data;
  data.push_back(2);
  data.push_back(3);
  data.push_back(5);

  std::vector<GpuTrack> tracks;

  fastTrackSvc->searchByPair(data, tracks);

  for (size_t i = 0, size = tracks.size(); i != size; ++i) {
    const GpuTrack & t(tracks[i]);
    info() << "track " << i << endmsg;
    info() << "x0: " << t.X0 << "; tx: " << t.Tx << "; y0: " << t.Y0 << "; ty: " << t.Ty << endmsg;
    info() << "s0: " << t.S0 << "; sx: " << t.S0 << "; sz: " << t.Sz << "; sxz: " << t.Sxz << "; sz2: " << t.Sz2 << endmsg;
    info() << "u0: " << t.U0 << "; uy: " << t.Uy << "; uz: " << t.Uz << "; uyz: " << t.Uyz << "; uz2: " << t.Uyz << endmsg;
    info() << "hits:";
    for (size_t j = 0, size = t.Hits.size(); j != size; ++j)
      info() << " " << static_cast<int>(t.Hits[j]);
    info() << endmsg;
  }

  return StatusCode::SUCCESS;
}

StatusCode FastTrackAlg::execute() {
  info() << "FastTrackAlg::execute" << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode FastTrackAlg::finalize() {
  info() << "FastTrackAlg::finalize" << endmsg;
  return StatusCode::SUCCESS;
}
