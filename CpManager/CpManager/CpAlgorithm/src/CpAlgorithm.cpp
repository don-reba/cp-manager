#include "CpAlgorithm.h"
#include "GaudiKernel/AlgFactory.h"
#include "CpService/ICpService.h"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

DECLARE_ALGORITHM_FACTORY(CpAlgorithm)

void * simpleAlloc(size_t size, ICpService::AllocParam param) {
  if (size != 4) {
    ostringstream msg;
    msg << "Expected to receive a 4-byte result, received " << size << ".";
    throw runtime_error(msg.str());
  }
  return param;
}

CpAlgorithm::CpAlgorithm(std::string name, ISvcLocator * pSL) :
    GaudiAlgorithm(name, pSL),
    m_dataAmount(0) {
  declareProperty("DataAmount", m_dataAmount);
}

CpAlgorithm::~CpAlgorithm() {
}

StatusCode CpAlgorithm::initialize() {
  info() << "CpAlgorithm::initialize" << endmsg;

  m_gpuService.reset(svc<ICpService>("CpService", true));
  info() << "got CpService" << endmsg;

  vector<uint8_t> data(m_dataAmount);

  uint32_t result;
  m_gpuService->submitData("SampleCpHandler", data.data(), data.size(), simpleAlloc, &result);
  cout << "Received: " << result << endl;

  return StatusCode::SUCCESS;
}

StatusCode CpAlgorithm::execute() {
  info() << "CpAlgorithm::execute" << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode CpAlgorithm::finalize() {
  info() << "CpAlgorithm::finalize" << endmsg;

  m_gpuService.reset();

  return StatusCode::SUCCESS;
}
