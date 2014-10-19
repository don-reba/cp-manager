###############################################################################
# File for running Brunel testing the trigger
#
# Syntax is:
#   gaudirun.py SftWithVP.py
##############################################################################

from Configurables import GpuService
GpuService().SocketPath = "/tmp/GpuManager"

from Brunel.Configuration import *
from Configurables import PrChecker, TrackEffChecker, PrPixelTracking
from Configurables import PrLHCbID2MCParticle, PrTrackAssociator
from Configurables import PrChecker
from Configurables import ProcessPhase, GaudiSequencer, NTupleSvc, OutputStream

from GaudiKernel.ProcessJobOptions import importOptions
importOptions("$APPCONFIGOPTS/Brunel/MC-WithTruth.py")
importOptions("$APPCONFIGOPTS/Brunel/Upgrade-RichPmt.py")
importOptions("$APPCONFIGOPTS/Brunel/patchUpgrade1.py")
importOptions("$APPCONFIGOPTS/Brunel/xdst.py")

from Configurables import Brunel
Brunel().DataType  = "Upgrade"
Brunel().WithMC    = True
Brunel().InputType = "XDST"
Brunel().Detectors = ['VP', 'UT', 'FT', 'Rich1Pmt', 'Rich2Pmt', 'Spd', 'Prs', 'Ecal', 'Hcal', 'Muon', 'Magnet', 'Tr' ]
# Brunel().RecoSequence = [ "HLT" ]
# Brunel().MCLinksSequence = [ "Unpack", "Tr" ]
# Brunel().MCCheckSequence = [ "Pat" ]
Brunel().OutputType = "NONE"
RecMoniConf().MoniSequence = [ ]

# Database
from Configurables import CondDB, LHCbApp
LHCbApp().EvtMax = 100
LHCbApp().Simulation = True
LHCbApp().DDDBtag = "dddb-20131025"
LHCbApp().CondDBtag = "sim-20130830-vc-md100"
CondDB().AllLocalTagsByDataType=["VP_UVP+RICH_2019+UT"]
CondDB().Upgrade = True

# from Configurables import GaudiSequencer, RecMoniConf, TrackSys, PrDebugTrackingLosses, PrPixelTracking
# from Configurables import PrForwardTracking, PrForwardTool, PrForwardFromPointTool, ToolSvc, PrFTHitManager

# dcampora
from os import listdir

# containing_folder = "/scratch/dcampora/raw_brunel_pixel/"
containing_folder = "/home/dcampora/git_projects/gaudi-gpu/common/xdst/d0pikspipi_decprodcut/"
inputFiles = [containing_folder + a for a in listdir(containing_folder) if ".xdst" in a]
EventSelector().Input = inputFiles

# PrPixel options
PrPixelTracking().TimingMeasurement = True
PrPixelTracking().OutputLevel = DEBUG

# PrChecker options
#PrChecker().VeloTracks = "/Event/Fst/Track/Velo"
#PrChecker().ForwardTracks = "/Event/Fst/Track/Forward"

#PrTrackAssociator options
#PrTrackAssociator().RootOfContainers = "/Event/Fst/Track"

def track_checker():
    GaudiSequencer("MCLinksTrSeq").Members = [PrLHCbID2MCParticle(), PrTrackAssociator()]
    GaudiSequencer("CheckPatSeq").Members = [PrChecker()]

def quick():
    GaudiSequencer("RecoCALOSeq").Members      = []
    GaudiSequencer("RecoMUONSeq").Members      = []
    GaudiSequencer("RecoPROTOSeq").Members     = []
    GaudiSequencer("RecoRICHSeq").Members      = []
    GaudiSequencer("MCLinksUnpackSeq").Members = []
    GaudiSequencer("CaloBanksHandler").Members = []
    GaudiSequencer("MCLinksTrSeq").Members     = []
    GaudiSequencer("CheckPatSeq").Members      = []
    GaudiSequencer("MCLinksCaloSeq").Members   = []
    GaudiSequencer("TrUpSeq").Members          = []
    GaudiSequencer("TrMatchSeq").Members       = []
    GaudiSequencer("TrDownSeq").Members        = []
    GaudiSequencer("RecoVertexSeq").Members    = []
    GaudiSequencer("TrForwardSeq").Members     = []
    GaudiSequencer("TrSeedingSeq").Members     = []
    GaudiSequencer("TrBestSeq").Members        = []
    GaudiSequencer("TrFitSeq").Members         = []
    GaudiSequencer("CheckMUONSeq").Members     = []
    GaudiSequencer("CheckRICHSeq").Members     = []
    GaudiSequencer("OutputDSTSeq").Members     = []
    GaudiSequencer("RecoSUMMARYSeq").Members   = []

appendPostConfigAction(quick)
appendPostConfigAction(track_checker)

