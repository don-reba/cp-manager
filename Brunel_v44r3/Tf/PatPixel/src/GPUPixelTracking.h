#ifndef SRC_GPUPIXELTRACKING_H
#define SRC_GPUPIXELTRACKING_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "FastTrackSvc/IFastTrackSvc.h"
#include "GPUPixelDataSequencer.h"
#include <vector>

using namespace std;

static const InterfaceID IID_GPUPixelTracking ( "GPUPixelTracking", 1, 0 );

/** @class GPUPixelTracking GPUPixelTracking.h src/GPUPixelTracking.h
  * 
  * 
  * @author dcampora
  * @date 25/02/2013
  */

class GPUPixelTracking: public GaudiAlgorithm {
public:
  PatPixelHitManager* hitManager;

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_GPUPixelTracking; }

  /// Standard constructor
  GPUPixelTracking(const std::string& name, ISvcLocator* pSvcLocator);
  virtual ~GPUPixelTracking(); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

  void buildHits();

protected:
private:
  SmartIF<IFastTrackSvc> fastTrackSvc;
  pixelEvent event;

};

#endif // SRC_GPUPIXELTRACKING_H
