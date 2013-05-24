from Gaudi.Configuration import *

from Configurables import GpuService
GpuService().SocketPath = "/tmp/GpuManager"

from Configurables import GpuAlgorithm
ApplicationMgr().TopAlg += [ GpuAlgorithm() ]

