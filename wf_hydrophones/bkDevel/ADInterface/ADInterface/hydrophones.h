#ifndef HYDROPHONES
#define HYDROPHONES

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

//Include the DWF library
#ifdef WIN32
#include "dwf.h"
#else
#include <digilent/waveforms/dwf.h>
#endif

//Define the Wait macro
#ifdef WIN32
#include <windows.h>
#define Wait(ts) Sleep(1000*ts)
#else
#include <unistd.h>
#include <sys/time.h>
#define Wait(ts) usleep(1000000*ts)
#endif

//#Defines to clarify which Analog Discovery is which
#define AD1 "SN:210244659676"//Insert SN here
#define AD2 "SN:210244449426"//Insert SN here

//Define our functions
vector<int> enumDevs();
void openDevs(int dev, HDWF * handle);
double analogReadSingleDataDev(HDWF handle, int channel);
void setupAnalogRead(HDWF handle, int channel, double range, double offset);
void setupRecordAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq, int sample_size);
void *readDevice(void * arg);
void *crossCorrelation(void * arg);

#endif
