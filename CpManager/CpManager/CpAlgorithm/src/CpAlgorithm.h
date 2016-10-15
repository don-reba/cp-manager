#include "GaudiAlg/GaudiAlgorithm.h"

#include <GaudiKernel/Property.h>

// forward declarations
class ICpService;

/// CpAlgorithm
class CpAlgorithm : public GaudiAlgorithm {
  public:
    CpAlgorithm(std::string name, ISvcLocator * pSL);
    virtual ~CpAlgorithm();

    // IAlgorithm implementation
    virtual StatusCode initialize();
    virtual StatusCode execute();
    virtual StatusCode finalize();

  private:
    SmartIF<ICpService> m_gpuService;

    IntegerProperty m_dataAmount;
};
