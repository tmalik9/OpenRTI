pushd D:\Scratch\openrti\bin
start cmd /C d:\Scratch\openrti\bin\rtinode.exe -i localhost
start cmd /K TimeConstrainedFederate1516e_d.exe --name TimeConstrainedFederate --address 127.0.0.1 --fom d:\vfs\CANoe\private\mth\12.0_DistSim_HLA\Projects_Source\OpenRTI-0.9.0\tests\rti1516e\testfed\testfom.xml
start cmd /K TimeRegulatingFederate1516e_d.exe --name TimeRegulatingFederate --address 127.0.0.1 --fom d:\vfs\CANoe\private\mth\12.0_DistSim_HLA\Projects_Source\OpenRTI-0.9.0\tests\rti1516e\testfed\testfom.xml
popd
