#include "GpuAlgorithm.h"
#include "GaudiKernel/AlgFactory.h"
#include "GpuService/IGpuService.h"

DECLARE_ALGORITHM_FACTORY(GpuAlgorithm)

GpuAlgorithm::GpuAlgorithm(std::string name, ISvcLocator * pSL)
    : GaudiAlgorithm(name, pSL) {
}

GpuAlgorithm::~GpuAlgorithm() {
}

StatusCode GpuAlgorithm::initialize() {
  info() << "GpuAlgorithm::initialize" << endmsg;

  gpuService.reset(svc<IGpuService>("GpuService", true));
  info() << "got GpuService" << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::execute() {
  info() << "GpuAlgorithm::execute" << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::finalize() {
  info() << "GpuAlgorithm::finalize" << endmsg;

  gpuService.reset();

  return StatusCode::SUCCESS;
}
