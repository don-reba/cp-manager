##############################################################################
# File for running Brunel testing the trigger
#
# Syntax is:
#   gaudirun.py SftWithVP.py
##############################################################################

from Configurables import GpuService
GpuService().SocketPath = "/tmp/GpuManager"

from GaudiKernel.ProcessJobOptions import importOptions
importOptions("$APPCONFIGOPTS/Brunel/MC-WithTruth.py")

from Configurables import Brunel
Brunel().DataType  = "2011"
Brunel().Simulation = True
Brunel().EvtMax = 1000

from Configurables import DDDBConf, CondDB, LHCbApp 

CondDB().Upgrade = True
CondDB().AllLocalTagsByDataType=["VP_Compact_MicroChannel+UT","FT"] 
LHCbApp().DDDBtag    = "dddb-20130320"
LHCbApp().CondDBtag  = "simcond-20121001-vc-md100"

from Configurables import GaudiSequencer, RecMoniConf, TrackSys, PrDebugTrackingLosses, FastVeloTracking, PatPixelTracking
from Configurables import PrForwardTracking, PrForwardTool, PrForwardFromPointTool, ToolSvc, PrFTHitManager

Brunel().RecoSequence = [ "HLT" ]
RecMoniConf().MoniSequence = [ ]
Brunel().MCLinksSequence = [ "Unpack", "Tr" ]
Brunel().MCCheckSequence = [ "Pat" ]
Brunel().OutputType = "NONE"

from FstTools.Configuration import *
FstConf().TStationType = "FT"
FstConf().VeloType = "VP"
FstConf().TTType = "none"

from Gaudi.Configuration import *
from GaudiConf import IOHelper

# dcampora
from os import listdir

# containing_folder = "/scratch/dcampora/raw_brunel_pixel/"
containing_folder = "/home/dcampora/projects/gaudi-gpu-project/gaudi-gpu/common/VP/"
inputFiles = [containing_folder + a for a in listdir(containing_folder)[:1]]

# TODO: How to set these parameters?
#PatPixelTracking().TimingMeasurement = True
#PatPixelTracking().OutputLevel = DEBUG


#### VP + UT + FT, b->phi phi, nu = 6.8
IOHelper().inputFiles( inputFiles )

#### VP + UT + FT, b->phi phi, nu = 3.4
#IOHelper().inputFiles([
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000003_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000005_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000008_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000009_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000011_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000013_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000014_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000017_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000022_2.xdigi?svcClass=default',
#   'PFN:root://castorlhcb.cern.ch//castor/cern.ch/grid/lhcb/MC/Upgrade/XDIGI/00022624/0000/00022624_00000023_2.xdigi?svcClass=default'
#   ])


from Gaudi.Configuration import *
def doMyChanges():
   from Configurables import GaudiSequencer, PrTrackAssociator, PrChecker
   GaudiSequencer("CaloBanksHandler").Members = []
   GaudiSequencer("DecodeTriggerSeq").Members = []
   GaudiSequencer("MCLinksTrSeq").Members = [ "PrLHCbID2MCParticle", "PrTrackAssociator"]
   PrTrackAssociator().RootOfContainers = "/Event/Fst/Track"
   GaudiSequencer("CheckPatSeq" ).Members = [ "PrChecker" ]
   PrChecker().VeloTracks = "/Event/Fst/Track/Velo"
   PrChecker().ForwardTracks = "/Event/Fst/Track/Forward"
appendPostConfigAction( doMyChanges )

