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

  std::vector<Track> tracks(2);
  tracks.at(0).Hits.push_back(2);
  tracks.at(0).Hits.push_back(3);
  tracks.at(1).Hits.push_back(5);
  tracks.at(1).Hits.push_back(7);

  std::vector<int8_t> result;

  fastTrackSvc->searchByPair(tracks, result);

  info() << "searchByPair result: ";
  for (size_t i = 0, size = result.size(); i != size; ++i)
    info() << static_cast<int>(result[i]) << " ";
  info() << "(expected: 6 35)" << endmsg;

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
