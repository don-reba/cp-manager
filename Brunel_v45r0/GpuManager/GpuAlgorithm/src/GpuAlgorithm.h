#include "GaudiAlg/GaudiAlgorithm.h"

#include <GaudiKernel/Property.h>

// forward declarations
class IGpuService;

/// GpuAlgorithm
class GpuAlgorithm : public GaudiAlgorithm {
  public:
    GpuAlgorithm(std::string name, ISvcLocator * pSL);
    virtual ~GpuAlgorithm();

    // IAlgorithm implementation
    virtual StatusCode initialize();
    virtual StatusCode execute();
    virtual StatusCode finalize();

  private:
    SmartIF<IGpuService> m_gpuService;

    IntegerProperty m_dataAmount;
};
