/*
	Copyright (C) 2015 Wilbert van Ham, Stichting Katholieke Universiteit, 
	KVK 41055629, Nijmegen

	This program is free software; you can redistribute it and/or modify 
	it under the terms of the GNU General Public License as published by 
	the Free Software Foundation; either version 3 of the License, or (at 
	your option) any later version.

	This program is distributed in the hope that it will be useful, but 
	WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
	General Public License for more details.

	You should have received a copy of the GNU General Public License 
	along with this program; if not, see <http://www.gnu.org/licenses>.

	Additional permission under GNU GPL version 3 section 7

	If you modify this Program, or any covered work, by linking or 
	combining it with NDI Optotrak API library (or a modified version 
	of that library), containing parts covered by the terms of its 
	proprietary license, the licensors of this Program grant you 
	additional permission to convey the resulting work. 
*/
#define NPY_NO_DEPRECATED_API 7
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// ND Library Files
#include "ndtypes.h"
#include "ndpack.h"
#include "ndopto.h"

#define LEN 1024
#define NUM_SENSORS 1
#define NUM_MARKERS 8

typedef struct SensorDataStruct{
	unsigned char ucPeak;
	unsigned char ucDRC;
	unsigned char ucCode;
	unsigned char ucPeakNibble;
} SensorDataType;

typedef struct FullRawDataStruct{
	float fCentroid[ NUM_SENSORS];
	SensorDataType SensorData[ NUM_SENSORS];
} FullRawDataType;
Position3d *pPosition3d;
FullRawDataType *pFullRawData;


/*
 * Receive NDI oapi commands from the standard input and return python
 * parsible tuples to the stdout
 */
int cmp(const char *a, const char *b){ return memcmp(a, b, strlen(b))==0; }
int main(){
	// Function to retrieve OAPI version
	size_t len = LEN;
	size_t read;
	char *line = malloc(len);
	assert(line);
	// return variables
	char s0[1024];
	int i0;
	int nFrameOld = -1;
	pPosition3d = calloc(NUM_MARKERS, sizeof(Position3d));
	assert(pPosition3d);
	pFullRawData = calloc(NUM_MARKERS, sizeof(FullRawDataType));
	assert(pFullRawData);

	
	while ((read = getline(&line, &len, stdin)) != -1) {
		//fprintf(stderr, "INFO: Retrieved line of length %zu (including NULL): %s", read, line);
		if (cmp(line, "OAPIGetVersionString")){
			i0 = OAPIGetVersionString(s0, len);
			printf("'%s'\n", s0);
		} else if (cmp(line, "OptoGetLastError")){
			i0 = OptoGetLastError(s0, len);
			printf("'%s'\n", s0);
		} else if (cmp(line, "OptotrakGetErrorString")){
			i0 = OptotrakGetErrorString(s0, len);
			printf("'%s'\n", s0);
		} else if (cmp(line, "OptoGetExtendedErrorCode")){
			i0 = OptoGetExtendedErrorCode();
			printf("%d\n", i0);
		} else if (cmp(line, "TransputerLoadSystem")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			i0 = TransputerLoadSystem(s);
			//i0 = TransputerLoadSystem("/opt/NDIoapi/ndigital/realtime/system.nif");
			printf("%d\n", i0);
		} else if (cmp(line, "TransputerInitializeSystem")){
			i0 = TransputerInitializeSystem(OPTO_LOG_ERRORS_FLAG);
			printf("%d\n", i0);
		} else if (cmp(line, "TransputerShutdownSystem")){
			i0 = TransputerShutdownSystem();
			printf("%d\n", i0);
		} else if (cmp(line, "TransputerDetermineSystemCfg")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			i0 = TransputerDetermineSystemCfg(s);
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakLoadCameraParameters")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			OptotrakLoadCameraParameters(s);
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakLoadAutoScale")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			OptotrakLoadAutoScale(s);
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakLockTemperatures")){
			i0 = OptotrakLockTemperatures();
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakSetCollectionFile")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			OptotrakSetCollectionFile(s);
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakSetupCollectionFromFile")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\""); // may be null
			OptotrakSetupCollectionFromFile(s);
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakSetupCollection")){
			int nMarkers;
			float fFrameFrequency, fMarkerFrequency;
			int nThreshold, nMinimumGain, nStreamData;
			float fDutyCycle, fVoltage, fCollectionTime, fPreTriggerTime;
			int nFlags;
			
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			int i = sscanf(s, "%d, %f, %f, %d, %d, %d, %f, %f, %f, %f, %d", 
				&nMarkers, 
				&fFrameFrequency, &fMarkerFrequency,
				&nThreshold, &nMinimumGain, &nStreamData,
				&fDutyCycle, &fVoltage, &fCollectionTime, &fPreTriggerTime,
				&nFlags);
			if(i==11){
				i0 = OptotrakSetupCollection(nMarkers, 
					fFrameFrequency, fMarkerFrequency, 
					nThreshold, nMinimumGain, nStreamData,
					fDutyCycle, fVoltage, fCollectionTime, fPreTriggerTime, 
					nFlags);
				printf("%d\n", i0);
			} else
				printf("ERROR: %d arguments read (11 expected)\n", i);
		} else if (cmp(line, "OptotrakActivateMarkers")){
			i0 = OptotrakActivateMarkers();
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakDeActivateMarkers")){
			i0 = OptotrakDeActivateMarkers();
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakGetStatus")){
			int nNumSensors, nNumOdaus, nNumRigidBodies, nMarkers;
			float fFrameFrequency, fMarkerFrequency;
			int nThreshold, nMinimumGain, nStreamData;
			float fDutyCycle, fVoltage, fCollectionTime, fPreTriggerTime;
			int nFlags;
			i0 = OptotrakGetStatus(&nNumSensors, &nNumOdaus, &nNumRigidBodies, &nMarkers,
				&fFrameFrequency, &fMarkerFrequency,
				&nThreshold, &nMinimumGain, &nStreamData, 
				&fDutyCycle, &fVoltage, &fCollectionTime, &fPreTriggerTime,
				&nFlags);
			printf("(%d, %d, %d, %d, %f, %f, %d, %d, %d, %f, %f, %f, %f, 0x%x)\n", 
				nNumSensors, nNumOdaus, nNumRigidBodies, nMarkers,
				fFrameFrequency, fMarkerFrequency,
				nThreshold, nMinimumGain, nStreamData, 
				fDutyCycle, fVoltage, fCollectionTime, fPreTriggerTime,
				nFlags);
		} else if (cmp(line, "OptotrakSetStroberPortTable")){
			int nPort1, nPort2, nPort3, nPort4;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %d, %d, %d", &nPort1, &nPort2, &nPort3, &nPort4) == 4){
				i0 = OptotrakSetStroberPortTable(nPort1, nPort2, nPort3, nPort4);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading 4 arguments\n");
		} else if (cmp(line, "OptotrakSaveCollectionToFile")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\"");
			if (s)
				i0 = OptotrakSaveCollectionToFile(s);
			else
				i0 = OptotrakSaveCollectionToFile("");
			printf("%d\n", i0);
		} else if (cmp(line, "OptotrakSetCameraParameters")){
			int nMarkerType, nWaveLength, nModelType;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %d, %d", &nMarkerType, &nWaveLength, &nModelType) == 3){
				i0 = OptotrakSetCameraParameters(nMarkerType, nWaveLength, nModelType);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading 3 arguments\n");
		} else if (cmp(line, "OptotrakGetCameraParameterStatus")){
			int curMarkerType, nCurWaveLength, nCurModelType;
			char szStatus[len];
			i0 = OptotrakGetCameraParameterStatus(&curMarkerType, &nCurWaveLength, &nCurModelType, szStatus, len);
			printf("(%d, %d, %d, '%s')\n", curMarkerType, nCurWaveLength, nCurModelType, szStatus);
		} else if (cmp(line, "OdauSaveCollectionToFile")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\"");
			if (s){
				i0 = OptotrakSaveCollectionToFile(s);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading filename\n");
		} else if (cmp(line, "OdauSetupCollectionFromFile")){
			strtok(line, "\"");
			char* s = strtok(NULL, "\"");
			if (s){
				i0 = OdauSetupCollectionFromFile(s);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading filename\n");
		} else if (cmp(line, "OdauSetTimer")){
			//int OdauSetTimer( int nOdauId, unsigned uTimer, unsigned uMode, unsigned long ulVal );
			int nOdauId;
			unsigned uTimer, uMode;
			unsigned long ulVal;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %u, %u, %lu", &nOdauId, &uTimer, &uMode, &ulVal) == 4){
				i0 = OdauSetTimer(ODAU1+nOdauId-1, uMode, uTimer, ulVal);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading 4 arguments\n");
		} else if (cmp(line, "OdauSetAnalogOutputs")){
			//int OdauSetAnalogOutputs( int nOdauId, float *pfVoltage1, float *pfVoltage2, unsigned uChangeMask );
			int nOdauId; // 1-4 -> ODAU1-ODAU4
			float fVoltage1;
			float fVoltage2;
			unsigned uChangeMask;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %f, %f, %u", &nOdauId, &fVoltage1, &fVoltage2, &uChangeMask) == 4){
				i0 = OdauSetAnalogOutputs(ODAU1+nOdauId-1, &fVoltage1, &fVoltage2, uChangeMask);
				printf("(%f, %f)\n", fVoltage1, fVoltage1);
			} else
				printf("ERROR: reading 4 arguments\n");
		} else if (cmp(line, "OdauSetDigitalOutputs")){
			//int OdauSetDigitalOutputs( int nOdauId, unsigned *puDigitalOut, unsigned uUpdateMask );
			int nOdauId; // 1-4 -> ODAU1-ODAU4
			unsigned uDigitalOut, uUpdateMask;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %u, %u", &nOdauId, &uDigitalOut, &uUpdateMask) == 4){
				i0 = OdauSetDigitalOutputs(ODAU1+nOdauId-1, &uDigitalOut, uUpdateMask);
				printf("0x%x\n", uDigitalOut);
			} else
				printf("ERROR: reading 3 arguments\n");
		} else if (cmp(line, "OdauSetupCollection")){
			//OdauSetupCollection( int nOdauId, int nChannels, int nGain, int nDigitalMode, float fFrameFreq, float fScanFreq, int nStreamData, float fCollectionTime, float fPreTriggerTime, unsigned uFlags );
			int nOdauId, nChannels, nGain, nDigitalMode;
			float fFrameFreq, fScanFreq;
			int nStreamData;
			float fCollectionTime, fPreTriggerTime;
			unsigned uFlags;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %d, %d, %d, %f, %f, %d, %f, %f, %u", 
					&nOdauId, &nChannels, &nGain, &nDigitalMode, 
					&fFrameFreq, &fScanFreq, &nStreamData,
					&fCollectionTime, &fPreTriggerTime, &uFlags) == 10){
				i0 = OdauSetupCollection(ODAU1+nOdauId-1, nChannels, nGain, nDigitalMode, 
					fFrameFreq, fScanFreq, nStreamData,
					fCollectionTime, fPreTriggerTime, uFlags);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading 10 arguments\n");
		} else if (cmp(line, "OptotrakStopCollection")){
			i0 = OptotrakStopCollection();
			printf("%d\n", i0);
		} else if (cmp(line, "OdauGetStatus")){
			 //int OdauGetStatus( int nOdauId, int *pnChannels, int *pnGain, int *pnDigitalMode, float *pfFrameFrequency, float *pfScanFrequency, int *pnStreamData, float *pfCollectionTime, float *pfPreTriggerTime, unsigned  *puCollFlags, int *pnFlags );
			int nOdauId, nChannels, nGain, nDigitalMode;
			float fFrameFrequency, fScanFrequency;
			int nStreamData;
			float fCollectionTime, fPreTriggerTime;
			unsigned  uCollFlags;
			int nFlags;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d", &nOdauId) == 1){
				i0 = OdauGetStatus(ODAU1+nOdauId-1, &nChannels, &nGain, &nDigitalMode, 
					&fFrameFrequency, &fScanFrequency, &nStreamData, &fCollectionTime, &fPreTriggerTime, 
					&uCollFlags, &nFlags);
				printf("(%d, %d, %d, %f, %f, %d, %f, %f, 0x%04x, %d)\n", 
					nChannels, nGain, nDigitalMode, 
					fFrameFrequency, fScanFrequency, nStreamData, fCollectionTime, fPreTriggerTime, 
					uCollFlags, nFlags);
			} else
				printf("ERROR: reading argument\n");
		} else if (cmp(line, "RigidBodyAddFromFile")){
			int nRigidBodyId, nStartMarker;
			char szRigFile[len];
			int nFlags;
			strtok(line, "(");
			char* s = strtok(NULL, ")");
			if(s && sscanf(s, "%d, %d, %s, %d", &nRigidBodyId, &nStartMarker, szRigFile, &nFlags) != 4){
				i0 = RigidBodyAddFromFile(nRigidBodyId, nStartMarker, szRigFile, nFlags);
				printf("%d\n", i0);
			} else
				printf("ERROR: reading 4 arguments\n");
		} else if (cmp(line, "RigidBodyAdd")){
			//int ( int nRigidBodyId, int nStartMarker, int nNumMarkers, float *pRigidCoordinates, float *pNormalCoordinates, int nFlags );
			int nRigidBodyId, nStartMarker, nNumMarkers;
			float *rigidCoordinates, *normalCoordinates;
			int nFlags;
			char* s = strtok(line, "(");
			
			if(s && sscanf(line+strlen(s)+1, "%d, %d, %d", &nRigidBodyId, &nStartMarker, &nNumMarkers)==3){
				rigidCoordinates = calloc(3*nNumMarkers, sizeof(float));
				normalCoordinates = calloc(3*nNumMarkers, sizeof(float));
				s = strtok(NULL, "[("); // up to first list/tuple
				if(!s){
					printf("ERROR: reading rigid values\n");
					goto end;
				}
				for(int i=0; i<3*nNumMarkers; i++){
					char* s = strtok(NULL, ",");
					if(!s){
						printf("ERROR: reading rigid value %d\n", i);
						goto end;
					}
					if(sscanf(s, "%f", (float*)&rigidCoordinates[i]) != 1){
						printf("ERROR: parsing rigid value %d\n", i);
						goto end;
					}
				}

				s = strtok(NULL, "[("); // up to first list/tuple
				if(!s){
					printf("ERROR: reading normal values\n");
					goto end;
				}
				for(int i=0; i<3*nNumMarkers; i++){
					char* s = strtok(NULL, ",");
					if(!s){
						printf("ERROR: reading normal value %d\n", i);
						goto end;
					}
					if(sscanf(s, "%f", (float*)&normalCoordinates[i]) != 1){
						printf("ERROR: parsing normal value %d\n", i);
						goto end;
					}
				}
				s = strtok(NULL, "],) ");
				//s = strtok(NULL, ")");
				if(s && sscanf(s, "%d", &nFlags)==1){
					i0 = RigidBodyAdd(nRigidBodyId, nStartMarker, nNumMarkers, rigidCoordinates, normalCoordinates, nFlags);
					printf("%d\n", i0);
				} else
					printf("ERROR: reading last argument\n");
			} else
				printf("ERROR: reading first 3 arguments\n");
			//int RigidBodyAddFromFile( int nRigidBodyId, int nStartMarker, char *pszRigFile, int nFlags );
			//int RigidBodyChangeSettings( int nRigidBodyId, int nMinMarkers, int nMaxMarkersAngle, float fMax3dError, float fMaxSensorError, float fMax3dRmsError, float fMaxSensorRmsError, int nFlags );
			//int RigidBodyDelete( int nRigidBodyId );
			//int RigidBodyChangeFOR( int nRigidId, int nRotationMethod );
		} else if (cmp(line, "DataGetLatestRaw")){
			uint uFrameNumber=999, uElements=999, uFlags=999;
			//FullRawDataType pFullRawData[NUM_MARKERS];
			i0 = DataGetLatestRaw(&uFrameNumber, &uElements, &uFlags, pFullRawData);
			printf("Frame Number: %8u\n", uFrameNumber);
			printf("Elements    : %8u\n", uElements);
			printf("Flags       :     0x%04x\n", uFlags);
			for(uint uMarkerCnt=0; uMarkerCnt<NUM_MARKERS; uMarkerCnt++){
				// Print out the current marker number.
				fprintf( stdout, "Marker %u, ", uMarkerCnt + 1 );
				//Print out the information for each sensor.
				for(uint uSensorCnt = 0; uSensorCnt<NUM_SENSORS; ++uSensorCnt ){
					//Print out the current sensor number.
					fprintf( stdout, "Sensor %u, ", uSensorCnt + 1 );
					// Print out the centroid. If it is bad print out *the string	'missing'.
					//if( pFullRawData[uMarkerCnt].fCentroid[uSensorCnt] < MAX_NEGATIVE )
						//printf("data missing, " );
					//else 
						printf("data: %8.2g, ", (float)pFullRawData[uMarkerCnt].fCentroid[uSensorCnt] );
					//Print out the rest of this sensor’s information.
					printf("peak: %4d, DRC: %4d, sensorCode: %u\n",
						pFullRawData[uMarkerCnt].SensorData[uSensorCnt].ucPeak,
						pFullRawData[uMarkerCnt].SensorData[uSensorCnt].ucDRC,
						(uint)pFullRawData[uMarkerCnt].SensorData[uSensorCnt].ucCode);
				} /* for */
			} /* for */
		} else if (cmp(line, "DataGetLatest3D")){
			//int DataGetLatest3D( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );
			uint nFrame=999, elems=999, flags=999;
			//Position3d dataDest[NUM_MARKERS];
			i0 = DataGetLatest3D(&nFrame, &elems, &flags, pPosition3d);
			printf("%d, %d, %d, %d\n", i0, nFrame, elems, flags);
			for(uint i=0; i<NUM_MARKERS; i++){
				printf("(%10.3g, %10.3g, %10.3g)\n", pPosition3d[i].x, pPosition3d[i].y, pPosition3d[i].z);
			}
		} else if (cmp(line, "RequestLatest3D")){
			i0 = RequestLatest3D();
			printf("%d\n", i0);
		} else if (cmp(line, "DataIsReady")){
			i0 = DataIsReady();
			printf("%d\n", i0);
		} else if (cmp(line, "DataReceiveLatest3D")){
			uint nFrame=999, elems=999, flags=999;
			Position3d dataDest;
			i0 = DataReceiveLatest3D(&nFrame, &elems, &flags, &dataDest);
			printf("%d, %d, %d, %d\n", i0, nFrame, elems, flags);
		} else if (cmp(line, "DataReceiveLatestRaw")){
			uint nFrame, elems, flags;
			void *dataDest;
			i0 = DataReceiveLatestRaw(&nFrame, &elems, &flags, &dataDest);
			printf("%d\n", i0);
		} else if (cmp(line, "DataBufferInitializeFile")){
			//unsigned uDataId;
			char pszFileName[len];
			strtok(line, "\"");
			char* s = strtok(NULL, "\"");
			if(sscanf(s, "%s", pszFileName)==1){
				printf("filename: %s\n", pszFileName);
				i0 = DataBufferInitializeFile(OPTOTRAK, s);
				printf("%d\n", i0);
			} else
				printf("ERROR: could not read filename\n");
		} else if (cmp(line, "DataBufferSpoolData")){
			int i = 999;
			i0 = DataBufferSpoolData(&i);
			printf("%d spool status %d\n", i0, i);
		} else if (cmp(line, "DataBufferStart")){
			i0 = DataBufferStart();
			printf("%d\n", i0);
		} else if (cmp(line, "DataBufferStop")){
			i0 = DataBufferStop();
			printf("%d\n", i0);
		} else {
			printf("ERROR: no such command: %s\n", line);
		}
		end:
		fflush(stdout);
	}
	exit(EXIT_SUCCESS);
}
	/*
// Function to retrieve OAPI version
xint OAPIGetVersionString( char* szVersionString, int nBufferSize );
xint OptoGetLastError( char* buf, int len );
xint OptoGetExtendedErrorCode();

// System initialization routines
xint TransputerLoadSystem( char *pszNifFile );
xint TransputerInitializeSystem( unsigned uFlags );
xint TransputerShutdownSystem( void );
xint TransputerDetermineSystemCfg( char *pszInputLogFile );

xint OptotrakLoadCameraParameters( char *pszCamFile );
xint OptotrakLoadAutoScale( char *pszAutoScaleFile );
xint OptotrakLockTemperatures( );
xint OptotrakSetCollectionFile( char *pszCollectFile );

xint OptotrakSetupCollectionFromFile( char *pszCollectFile );
xint OptotrakSetupCollection( int nMarkers, float fFrameFrequency, float fMarkerFrequency, int nThreshold, int nMinimumGain, int nStreamData, float fDutyCycle, float fVoltage, float fCollectionTime, float fPreTriggerTime, int nFlags );
xint OptotrakActivateMarkers( void );
xint OptotrakDeActivateMarkers( void );
xint OptotrakGetStatus( int *pnNumSensors, int *pnNumOdaus, int *pnNumRigidBodies, int *pnMarkers, float *pfFrameFrequency, float *pfMarkerFrequency, int *pnThreshold, int *pnMinimumGain, int *pnStreamData, float *pfDutyCycle, float *pfVoltage, float *pfCollectionTime, float *pfPreTriggerTime, int *pnFlags );
xint OptotrakSetStroberPortTable( int nPort1, int nPort2, int nPort3, int nPort4 );
xint OptotrakSaveCollectionToFile( char *pszCollectFile );
xint OptotrakGetErrorString( char* szErrorString, int nBufferSize );
xint OptotrakSetCameraParameters( int nMarkerType, int nWaveLength, int nModelType );
xint OptotrakGetCameraParameterStatus( int *pCurMarkerType, int *pnCurWaveLength, int *pnCurModelType, char *szStatus, int nStatusLength );
xint OdauSaveCollectionToFile( char *pszCollectFile );
xint OdauSetupCollectionFromFile( char *pszCollectFile );
xint OdauSetTimer( int nOdauId, unsigned uTimer, unsigned uMode, unsigned long ulVal );
xint OdauSetAnalogOutputs( int nOdauId, float *pfVoltage1, float *pfVoltage2, unsigned uChangeMask );
xint OdauSetDigitalOutputs( int nOdauId, unsigned *puDigitalOut, unsigned uUpdateMask );
xint OdauSetupCollection( int nOdauId, int nChannels, int nGain, int nDigitalMode, float fFrameFreq, float fScanFreq, int nStreamData, float fCollectionTime, float fPreTriggerTime, unsigned uFlags );
xint OptotrakStopCollection( void );
xint OdauGetStatus( int nOdauId, int *pnChannels, int *pnGain, int *pnDigitalMode, float *pfFrameFrequency, float *pfScanFrequency, int *pnStreamData, float *pfCollectionTime, float *pfPreTriggerTime, unsigned *puCollFlags, int *pnFlags );
int RigidBodyAdd( int nRigidBodyId, int nStartMarker, int nNumMarkers, float *pRigidCoordinates, float *pNormalCoordinates, int nFlags );
int RigidBodyAddFromFile( int nRigidBodyId, int nStartMarker, char *pszRigFile, int nFlags );
int RigidBodyChangeSettings( int nRigidBodyId, int nMinMarkers, int nMaxMarkersAngle, float fMax3dError, float fMaxSensorError, float fMax3dRmsError, float fMaxSensorRmsError, int nFlags );
int RigidBodyDelete( int nRigidBodyId );
//int RigidBodyChangeFOR( int nRigidId, int nRotationMethod );
int DataGetLatest3D( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );

// Luc: RAW is positions in system coordinates; Centroid is positions in system coordinates PLUS status information for debugging
// The Centroid has no experimental use/priority, but may be useful for debugging!
//int DataGetLatestCentroid( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );
int DataGetLatestRaw( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );
int DataGetLatestTransforms( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );
int DataGetLatestOdauRaw( int nOdauId, unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );

// Luc: These are needed for non-blocking realtime applications, so please implement
int RequestLatest3D( void );
//int RequestLatestRaw( void );
//int RequestLatestCentroid( void );
//int RequestLatestTransforms( void );
//int RequestLatestOdauRaw( int nOdauId );


// Luc: These are only useful if you also have the ones above
int  DataIsReady( void );
int  DataIsReadyExt( unsigned retryCount );
int  ReceiveLatestData( uint *uFrameNumber, uint *uElements, uint *uFlags, void *pDataDest );

// Luc: And these you do need as well to actually get the data
int  DataReceiveLatest3D( uint *pnFrame, uint *pElems, uint *pFlags, Position3d *pDataDest );
int DataReceiveLatestOdauRaw( unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, int *pDataDest );

// FM I assume we want for wireless markers if we have any (might be in the future)
int OptotrakEnableWireless( int nDevice, boolean bEnable );

int RequestNext3D( void );
int RequestNextOdauRaw( int nOdauId );
int DataGetNextOdauRaw( int nOdauId, unsigned *pnFrame, unsigned *pElems, unsigned *pFlags, void *pDataDest );

// FM I've used these before
int DataBufferInitializeFile( unsigned uDataId, const char *pszFileName );
int DataBufferInitializeMem( unsigned uDataId, SpoolPtrType pMemory );
int DataBufferStart( void );
int DataBufferStop( void );
int DataBufferSpoolData( unsigned *puSpoolStatus );
int DataBufferWriteData( unsigned *puRealtimeData, unsigned *puSpoolComplete, unsigned *puSpoolStatus, unsigned long *pulFramesBuffered );

int DataBufferAbortSpooling( void );

int  FileConvert( char *pszInputFilename, char *pszOutputFilename, unsigned uFileType int  OptoFileOpen(	char* pszFilename, uint uFileId, uint uFileMode, int* pnItems, int* pnSubItems, long int *plnFrames, float* pfFrequency, char* pszComments, void** pFileHeader );
int  OptoFileRead( uint uFileId, long int lnStartFrame, uint uNumberOfFrames, void *pDataDest );
int  OptoFileWrite( uint uFileId, long int lnStartFrame, uint uNumberOfFrames, void *pDataSrc );
int  OptoFileClose( uint uFileId );
int  OptoFileOpenAll(	char *pszFilename, uint uFileId, uint uFileMode, int *pnItems, int *pnSubItems, int *pnCharSubItems, int *pnIntSubItems, int *pnDoubleSubItems, long int *plnFrames, float *pfFrequency, char *pszComments, void **pFileHeader );
int  OptoFileReadAll(uint uFileId, long int lnStartFrame, uint uNumberOfFrames, void *pDataDestFloat, void *pDataDestChar, void *pDataDestInt, void *pDataDestDouble );
int  OptoFileWriteAll(	uint uFileId, long int lnStartFrame, uint uNumberOfFrames, void *pDataSrcFloat, void *pDataSrcChar, void *pDataSrcInt, void *pDataSrcDouble );
int  OptoFileCloseAll( uint uFileId );
*/