#include "GpuAlgorithm.h"
#include "GaudiKernel/AlgFactory.h"
#include "GpuService/IGpuService.h"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

DECLARE_ALGORITHM_FACTORY(GpuAlgorithm)

void * simpleAlloc(size_t size, IGpuService::AllocParam param) {
  if (size != 4) {
    ostringstream msg;
    msg << "Expected to receive a 4-byte result, received " << size << ".";
    throw runtime_error(msg.str());
  }
  return param;
}

GpuAlgorithm::GpuAlgorithm(std::string name, ISvcLocator * pSL) :
    GaudiAlgorithm(name, pSL),
    m_dataAmount(0) {
  declareProperty("DataAmount", m_dataAmount);
}

GpuAlgorithm::~GpuAlgorithm() {
}

StatusCode GpuAlgorithm::initialize() {
  info() << "GpuAlgorithm::initialize" << endmsg;

  m_gpuService.reset(svc<IGpuService>("GpuService", true));
  info() << "got GpuService" << endmsg;

  vector<uint8_t> data(m_dataAmount);

  uint32_t result;
  m_gpuService->submitData("SampleGpuHandler", data.data(), data.size(), simpleAlloc, &result);
  cout << "Received: " << result << endl;

  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::execute() {
  info() << "GpuAlgorithm::execute" << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode GpuAlgorithm::finalize() {
  info() << "GpuAlgorithm::finalize" << endmsg;

  m_gpuService.reset();

  return StatusCode::SUCCESS;
}
