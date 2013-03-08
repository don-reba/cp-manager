#include "GaudiAlg/GaudiAlgorithm.h"

class FastTrackAlg : public GaudiAlgorithm {
  public:
    FastTrackAlg(std::string name, ISvcLocator * pSL);
    virtual ~FastTrackAlg();

    // IAlgorithm implementation
    virtual StatusCode initialize();
    virtual StatusCode execute();
    virtual StatusCode finalize();
};
