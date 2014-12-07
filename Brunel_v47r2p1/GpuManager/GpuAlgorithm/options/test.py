from Gaudi.Configuration import *

from Configurables import GpuService
GpuService().SocketPath = "/tmp/GpuManager"

from Configurables import GpuAlgorithm
GpuAlgorithm().DataAmount = 1024
ApplicationMgr().TopAlg += [ GpuAlgorithm() ]

