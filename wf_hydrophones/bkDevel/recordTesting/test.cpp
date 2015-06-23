#include "sample.h"
#include <chrono>
#include <iostream>
using namespace std;
using chrono::high_resolution_clock;
#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)

int main(int carg, char **szarg){    
    double hzAcq = 200000.0;
    double incBy = 1000;
    int nSamples = (int)hzAcq*2, k = 0, errs = 0;
    HDWF hdwf, hdwf2;
    char szError[512] = {0};

    printf("Open automatically the first available device\n");
    if(!FDwfDeviceOpen(-1, &hdwf)) {
        FDwfGetLastErrorMsg(szError);
        printf("Device open failed\n\t%s", szError);
        return 0;
    }

    printf("Open automatically the second available device\n");
    if(!FDwfDeviceOpen(-1, &hdwf2)) {
        FDwfGetLastErrorMsg(szError);
        printf("Device open failed\n\t%s", szError);
        return 0;
    }

    FDwfAnalogInChannelEnableSet(hdwf, 0, true);
    
    FDwfAnalogInChannelRangeSet(hdwf, 0, 5);

    FDwfAnalogInChannelEnableSet(hdwf, 1, true);
    
    FDwfAnalogInChannelRangeSet(hdwf, 1, 5);

    FDwfAnalogInChannelEnableSet(hdwf2, 0, true);
    
    FDwfAnalogInChannelRangeSet(hdwf2, 0, 5);

    FDwfAnalogInChannelEnableSet(hdwf2, 1, true);
    
    FDwfAnalogInChannelRangeSet(hdwf2, 1, 5);

    // recording rate for more samples than the device buffer is limited by device communication
    FDwfAnalogInAcquisitionModeSet(hdwf, acqmodeRecord);
    FDwfAnalogInFrequencySet(hdwf, hzAcq);
    FDwfAnalogInRecordLengthSet(hdwf, 1.0*nSamples/hzAcq);

    FDwfAnalogInAcquisitionModeSet(hdwf2, acqmodeRecord);
    FDwfAnalogInFrequencySet(hdwf2, hzAcq);
    FDwfAnalogInRecordLengthSet(hdwf2, 1.0*nSamples/hzAcq);

    while(true)
    {

    STS sts, sts2;
    nSamples = (int)(hzAcq * 2);
    double* rgdSamples = new double[nSamples];
    double* rgdSamples1 = new double[nSamples];
    double* rgdSamples2 = new double[nSamples];
    double* rgdSamples3 = new double[nSamples];
    int cSamples = 0, cSamples1 = 0;
    int cAvailable, cLost=0, cLost2=0, cCorrupted, totalSamplesLost, totalCorrupted, cAvailable1;
    bool fLost = false, fCorrupted = false;
    
    FDwfAnalogInAcquisitionModeSet(hdwf, acqmodeRecord);
    FDwfAnalogInFrequencySet(hdwf, hzAcq);
    FDwfAnalogInRecordLengthSet(hdwf, 1.0*nSamples/hzAcq);
    FDwfAnalogInAcquisitionModeSet(hdwf2, acqmodeRecord);
    FDwfAnalogInFrequencySet(hdwf2, hzAcq);
    FDwfAnalogInRecordLengthSet(hdwf2, 1.0*nSamples/hzAcq);
    
    // wait at least 2 seconds with Analog Discovery for the offset to stabilize, before the first reading after device open or offset/range change
    //Wait(2);
    
    // start
    FDwfAnalogInConfigure(hdwf, false, true);
    FDwfAnalogInConfigure(hdwf2, false, true);
    
    cout << k + 1 << "Recording at: " << hzAcq << endl;
    //high_resolution_clock::period = ratio<1,1000000000>();
    auto start = high_resolution_clock::now();
    while(cSamples < nSamples && cSamples1 < nSamples){
        if(!FDwfAnalogInStatus(hdwf, true, &sts)) {
            printf("error");
            return 0;
        }

        if(cSamples == 0 && (sts == stsCfg || sts == stsPrefill || sts == stsArm)){
            // Acquisition not yet started.
            continue;
        }

        if(!FDwfAnalogInStatus(hdwf2, true, &sts2)) {
            printf("error");
            return 0;
        }

        if(cSamples == 0 && (sts2 == stsCfg || sts2 == stsPrefill || sts2 == stsArm)){
            // Acquisition not yet started.
            continue;
        }

        FDwfAnalogInStatusRecord(hdwf, &cAvailable, &cLost, &cCorrupted);
        FDwfAnalogInStatusRecord(hdwf2, &cAvailable1, &cLost2, &cCorrupted);

        cSamples += cLost + cLost2;
        totalSamplesLost += cLost + cLost2;

	totalCorrupted += cCorrupted;
        
        if(cLost || cLost2) fLost = true;
        if(cCorrupted) fCorrupted = true;

        if(!cAvailable) continue;

        // get samples
        FDwfAnalogInStatusData(hdwf, 0, &rgdSamples[cSamples], cAvailable);
        FDwfAnalogInStatusData(hdwf, 1, &rgdSamples1[cSamples], cAvailable);
        FDwfAnalogInStatusData(hdwf2, 0, &rgdSamples[cSamples], cAvailable1);
        FDwfAnalogInStatusData(hdwf2, 1, &rgdSamples1[cSamples], cAvailable1);

        cSamples += cAvailable;
        cSamples1 += cAvailable1;
    }
    auto end = high_resolution_clock::now();
    //double difference = end.rep;// - start.rep);
    cout << "done in " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
    
    if(fLost){
        printf("Samples were lost! Reduce frequency\n");
	printf("Lost %d samples\n", totalSamplesLost);
        if (k == 50) break;
        else ++errs;
    }else if(fCorrupted){
        printf("%d samples could be corrupted! Reduce frequency\n", totalCorrupted);
        if (k == 50) break;
        else ++errs;
    }
    
    if (k == 50)
    {
        if (errs > 0) break;
        hzAcq += incBy;
        k = 0;
    }
    ++k;
    delete[] rgdSamples;
    delete[] rgdSamples1;
    delete[] rgdSamples2;
    delete[] rgdSamples3;
    }
    cout << "Had " << errs << " at " << hzAcq << " Hz" << endl;
    cout << "Final freq: " << hzAcq - incBy << endl;

    // close the device
    FDwfDeviceCloseAll();
}
