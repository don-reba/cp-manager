// Include files
// From Gaudi
#include "GaudiKernel/AlgFactory.h"
// local
#include "GPUPixelTracking.h"

// ----------------------------------------------------------------------------
// Implementation file for class: GPUPixelTracking
// 
// 25/02/2013: dcampora
// ----------------------------------------------------------------------------
DECLARE_ALGORITHM_FACTORY(GPUPixelTracking)

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
GPUPixelTracking::GPUPixelTracking(const std::string& name, ISvcLocator* pSvcLocator)
  : GaudiAlgorithm(name, pSvcLocator)
{
    // declareInterface<GPUPixelTracking>(this);
}

// ============================================================================
// Destructor
// ============================================================================
GPUPixelTracking::~GPUPixelTracking() {}

// ============================================================================
// Initialization
// ============================================================================
StatusCode GPUPixelTracking::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  
  hitManager = tool<PatPixelHitManager>("PatPixelHitManager");
  // fastTrackSvc = svc<IFastTrackSvc>("FastTrackSvc", true);

  cout << "Initializing GPUPixelTracking" << endl;

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;
  return StatusCode::SUCCESS;
}

// ============================================================================
// Main execution
// ============================================================================
StatusCode GPUPixelTracking::execute() {
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  cout << "Executing GPUPixelTracking" << endl;

  vector<char> dataPointer;

  cout << "Getting data from event..." << endl;

  // The Data Sequencer gathers the data in a char* vector of size size.
  GPUPixelDataSequencer* pds = new GPUPixelDataSequencer(hitManager);
  pds->get(dataPointer);

  cout << "Server: We have a packet of size " << dataPointer.size() << " prepared to be sent." << endl;

  // fastTrackSvc->sayHelloWorld();
  // fastTrackSvc->sendData(dataPointer, 4);

  return StatusCode::SUCCESS;
}

// ============================================================================
// Finalize
// ============================================================================
StatusCode GPUPixelTracking::finalize() {
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;
  
  cout << "Finalizing GPUPixelTracking" << endl;

  // delete fastTrackSvc;

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

// ============================================================================

