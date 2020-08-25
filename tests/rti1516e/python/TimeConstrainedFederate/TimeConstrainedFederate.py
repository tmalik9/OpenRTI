import os
import sys
sys.path.append("E:\\vfs\\CANoe\\private\\mth\\12.0_DistSim_HLA\\bin\\Exec32_Debug")

import pyrti1516e as rti1516e

class MyAmb:
    def __init__(self):
        None

myamb=MyAmb()
rtiamb=rti1516e.RTIambassador()
print(rtiamb)
try:
    rtiamb.connect(myamb, "HLA_EVOKED", "127.0.0.1")
except Exception as e:
    print(e)

print("connected")
try:
    rtiamb.createFederationExecution("ExampleFederation", "E:\\vfs\\CANoe\\private\\mth\\12.0_DistSim_HLA\\Projects_Source\\OpenRTI-0.9.0\\tests\\rti1516e\\testfed\\testfom.xml", "HLAfloat64Time");
    print("created federation")
except rti1516e.FederationExecutionAlreadyExists as e:
    print("federation already exists")
except Exception as e:
    print(e)


try:
    rtiamb.joinFederationExecution("TimeConstrainedFederate", "ExampleFederation");
except Exception as e:
    print(e)
print("joined federation")

rtiamb.evokeCallback(2.0)
rtiamb.resignFederationExecution("NO_ACTION")
print("resigned from federation")
rtiamb.disconnect()
print("disconnected from rti")

del rtiamb