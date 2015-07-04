#ifndef HYDROPHONES
#define HYDROPHONES

#include <stdio.h>
#include <stdlib.h>
#include <iostream> //For cout and endl
#include <cstring> //For strerror()
#include <vector> //For vector
#include <queue> //For queue

using namespace std;

//Include the DWF library
#include <digilent/waveforms/dwf.h>

//Define the Wait macro
#include <unistd.h>
#include <sys/time.h>
#define Wait(ts) usleep(1000000*ts)

//Define our functions
void sigint_handler(int sig);
vector<int> enumDevs();
void openDev(int dev, HDWF * handle);
double analogReadSingleDataDev(HDWF handle, int channel);
void setupAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq);
void setupRecordAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq, int sample_size);
void beginRecord(HDWF handle);
void *readDevice(void * arg);
void *crossCorrelation(void * arg);

#endif
