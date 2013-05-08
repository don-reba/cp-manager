#include "GaudiAlg/GaudiAlgorithm.h"

class GpuAlgorithm : public GaudiAlgorithm {
  public:
    GpuAlgorithm(std::string name, ISvcLocator * pSL);
    virtual ~GpuAlgorithm();

    // IAlgorithm implementation
    virtual StatusCode initialize();
    virtual StatusCode execute();
    virtual StatusCode finalize();
};
