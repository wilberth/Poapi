import poapi, time

nr_trials = 2;

optotrak = poapi.Poapi()

optotrak.TransputerLoadSystem()
print(optotrak.TransputerInitializeSystem())
print(optotrak.OptotrakLoadCameraParameters("standard"))

#Settings ODAU:
OdauId          	="ODAU1";
NumChannels     	=12;          			#Number of channels in the collection.
Gain            	=1;          				#Gain to use for the analog channels (-10 to 10 V)
DigitalMode       =0;
FrameFrequency  	=500 ;       			 	#Frequency to collect data frames at.
ScanFrequency   	=90000 ;     				#Frequency to scan channels at.
StreamData      	=1;          				#Stream mode for the data buffers.
CollectionTime  	=5;          				#Number of seconds of data to collect.
PreTriggerTime  	=0;          				#Number of seconds to pre-trigger data by.

#Settings OPTOTRAK:
NumMarkers      	=4;          				#Number of markers in the collection.
FrameFrequency  	=500 ;       				#Frequency to collect data frames at.
MarkerFrequency	  =2500;       				#Marker frequency for marker maximum on-time.
Threshold       	=30;         				#Dynamic or Static Threshold value to use.
MinimumGain     	=160;        				#Minimum gain code amplification to use.
StreamData      	=1;          				#Stream mode for the data buffers.
DutyCycle       	=0.35;      				#Marker Duty Cycle to use.
Voltage         	=7.5;        				#Voltage to use when turning on markers.
CollectionTime  	=5;  					 	    #Number of seconds of data to collect.
PreTriggerTime  	=0;          				#Number of seconds to pre-trigger data by.


#### Setup optotrak and odau collection
print("Odau setup collection:{}".format(optotrak.OdauSetupCollection(OdauId,  NumChannels, Gain, DigitalMode,FrameFrequency, ScanFrequency, StreamData, CollectionTime, PreTriggerTime,0x0)))
print("Optotrak setup collection: {}".format(optotrak.OptotrakSetupCollection(NumMarkers,FrameFrequency, MarkerFrequency, Threshold, MinimumGain, StreamData,DutyCycle, Voltage, CollectionTime, PreTriggerTime,0x48000060)))
print("Optotrak activate markers: {}".format(optotrak.OptotrakActivateMarkers()))
print("Optotrak Get Status: {}".format(optotrak.OptotrakGetStatus()))
#print(optotrak.OdauGetStatus())


for i in range(nr_trials):

	print("Starting trial: {} ".format(str(i)))

	print("Odau data buffer initialize: {}".format(optotrak.DataBufferInitializeFile(OdauId, ("/tmp/O#00"+str(i)+".DAT"))))
	print("Optotrak data buffer initialize: {}".format(optotrak.DataBufferInitializeFile("OPTOTRAK","/tmp/C#00"+str(i)+".DAT")))
	
	print("Optotrak Data buffer start: {}\n".format(optotrak.DataBufferStart()))
	print('Collecting data file...')

	#### Test Data collection
	time.sleep(0.5)
	succeeded,framenr,elems,flags,data = optotrak.DataGetLatest3D()
	print(data)
	print(framenr)
	time.sleep(1)
	print("Optotrak Get Latest 3D: {}".format(optotrak.DataGetLatest3D()))
	

	# at each end of the trial the data is saved and written 
	print("Optotrak Data buffer stop: {}".format(optotrak.DataBufferStop()))
	time.sleep(1)
	spoolComplete = 0
	while not spoolComplete:
		succeeded, realTimeData, spoolComplete, spoolStatus, framesBuffered = optotrak.DataBufferWriteData()
		print(spoolComplete)
	time.sleep(2)

# end of trials

print('Exit experiment...')

#### End of experiment

#print"Optotrak last error: {}".format((optotrak.OptoGetLastError()))
	
#De-activate the optotrak markers. => not necessary anymore, apparently bufferstop automatically does it, as well as the transputershutdownsystem
#print("Optotrak de activate markers: {}".format(optotrak.OptotrakDeActivateMarkers()))

#Shutdown the transputer message passing system.
#print("Optotrak transputer shutdown: {}".format(optotrak.TransputerShutdownSystem()))


