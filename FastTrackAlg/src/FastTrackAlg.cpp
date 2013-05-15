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
    info() << "x0: " << t.x0 << "; tx: " << t.tx << "; y0: " << t.y0 << "; ty: " << t.ty << endmsg;
    info() << "s0: " << t.s0 << "; sx: " << t.s0 << "; sz: " << t.sz << "; sxz: " << t.sxz << "; sz2: " << t.sz2 << endmsg;
    info() << "u0: " << t.u0 << "; uy: " << t.uy << "; uz: " << t.uz << "; uyz: " << t.uyz << "; uz2: " << t.uyz << endmsg;
    info() << "hits:";
    for (size_t j = 0, size = t.hits.size(); j != size; ++j)
      info() << " " << static_cast<int>(t.hits[j]);
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
