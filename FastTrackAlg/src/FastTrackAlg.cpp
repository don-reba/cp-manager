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

  fastTrackSvc->sayHelloWorld();

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
