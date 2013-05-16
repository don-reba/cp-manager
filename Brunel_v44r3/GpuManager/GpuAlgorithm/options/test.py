from Gaudi.Configuration import *
from Configurables import GpuAlgorithm

alg = GpuAlgorithm()
ApplicationMgr().TopAlg += [ alg ]

