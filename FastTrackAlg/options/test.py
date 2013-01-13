from Gaudi.Configuration import *
from Configurables import FastTrackAlg

alg = FastTrackAlg()
ApplicationMgr().TopAlg += [ alg ]

