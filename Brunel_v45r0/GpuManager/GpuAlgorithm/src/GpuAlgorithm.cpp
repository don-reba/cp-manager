#include "GpuAlgorithm.h"
#include "GaudiKernel/AlgFactory.h"
#include "GpuService/IGpuService.h"

#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

DECLARE_ALGORITHM_FACTORY(GpuAlgorithm)

void * simpleAlloc(size_t size, IGpuService::AllocParam param) {
  if (size != 4) {
    ostringstream msg;
    msg << "Expected to receive 4 bytes, received " << size << ".";
    throw runtime_error(msg.str());
  }
  return param;
}

GpuAlgorithm::GpuAlgorithm(std::string name, ISvcLocator * pSL)
    : GaudiAlgorithm(name, pSL) {
}

GpuAlgorithm::~GpuAlgorithm() {
}

StatusCode GpuAlgorithm::initialize() {
  info() << "GpuAlgorithm::initialize" << endmsg;

  gpuService.reset(svc<IGpuService>("GpuService", true));
  info() << "got GpuService" << endmsg;

  uint32_t result;
  gpuService->submitData("SampleGpuHandler", NULL, 0, simpleAlloc, &result);
  cout << "Received: " << result << endl;

  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::execute() {
  info() << "GpuAlgorithm::execute" << endmsg;

  //gpuService->submitData("test", NULL, 0, noAlloc, NULL);

  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::finalize() {
  info() << "GpuAlgorithm::finalize" << endmsg;

  gpuService.reset();

  return StatusCode::SUCCESS;
}
