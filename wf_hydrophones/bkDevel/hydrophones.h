#ifndef HYDROPHONES
#define HYDROPHONES

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

//Include the DWF library
#include <digilent/waveforms/dwf.h>

//Define the wait macro
#include <unistd.h>
#include <sys/time.h>
#define Wait(ts) usleep(1000000*ts)

//#Defines to clarify which Analog Discovery is which
#define AD1 "SN:210244659676"//Insert SN here
#define AD2 "SN:210244449426"//Insert SN here

//Define our functions
vector<int> enumDevs();
void openDevs(int dev1, int dev2, vector<HDWF> *handles);
double analogReadDev(HDWF handle, int channel);
void setupAnalogRead(HDWF handle, int channel, double range, double offset);

#endif
