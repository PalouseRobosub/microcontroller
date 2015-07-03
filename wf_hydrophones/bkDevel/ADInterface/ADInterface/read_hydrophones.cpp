#include "sys.h"
#include <pthread.h> //For threading

/*  General DWF Lib usage

1. Call enumeration functions to discover connected hardware devices.
2. Call FDwfDeviceOpen function to establish a connection to specific hardware device.
3. Call function to enable instrument within hardware device.
4. Call functions to configure instrument and acquire/generate signals.
5. Call function to disable instrument.
6. Call FDwfDeviceClose function to disconnect from device

*/

//TODO: Develop a main that manages the data passing

//Global Variables to manage threading
pthread_t threadId[3];
queue<double*> qdev1ch1;
queue<double*> qdev1ch2;
queue<double*> qdev2ch1;
queue<double*> qdev2ch2;

int main(int carg, char **szarg){
    vector<int> cDevice;
    vector<HDWF> hdwfs(2,-1);

    //Enumerate the Devices
    cDevice = enumDevs();
    cout << "AD1 is at: " << cDevice[0] << endl;
    cout << "AD2 is at: " << cDevice[1] << endl;

    openDev(cDevice[0], &(hdwfs[0]));
    openDev(cDevice[1], &(hdwfs[1]));
    //cout << hdwfs.size() << endl;
    cout << "openDev() returned [" << (hdwfs)[0] << ", " << (hdwfs)[1] << "]" << endl;

    //Open the devices to record with
    if (hdwfs[0] != -1)
    {
        setupRecordAnalogRead(hdwfs[0], true, true, 5, 0, frequency, samplesPerBuf);
    }
    if (hdwfs[1] != -1)
    {
        setupRecordAnalogRead(hdwfs[1], true, true, 5, 0, frequency, samplesPerBuf);
    }

    /*if (hdwfs[0] == -1 || hdwfs[1] == -1)
    {
        cout << "One or more of the specified devices could not be connected..." << endl;
        cout << "EXITING!" << endl;
        FDwfDeviceCloseAll();
        return 0;
    }*/

    // wait at least 2 seconds with Analog Discovery for the offset to stabilize, before the first reading after device open or offset/range change
    Wait(2);

    //TODO: Add thread inits here
    for (int i = 0; i < 2; ++i)
    {
      int err = pthread_create(&(threadId[i]), NULL, &readDevice, (void *)&(hdwfs[i]));
      if (err != 0) cout << "Thread " << i << " could not be created: " << strerror(err) << endl;
      else cout << "Thread " << i << " created." << endl;
    }

    //TODO: Start Cross Correlation Thread
    int err = pthread_create(&(threadId[2]), NULL, &crossCorrelation, NULL);
    if (err != 0) cout << "Cross Correlation Thread was not created: " << strerror(err) << endl;
    else cout << "Cross Correlation Thread Running." << endl;

    pthread_join(threadId[0], NULL);
    pthread_join(threadId[1], NULL);

    while(true)
    {
      //TODO: Manage the threads here to search for exit status
      break;
    }
    // before application exit make sure to close all opened devices by this process
    FDwfDeviceCloseAll();

    return 0;
}

//Pass the device hardware definition as an argument
//This is the thread that will read data from a device
//TODO: Determine best way to read the devices in parallel
void *readDevice(void * arg)
{
    if (arg == NULL) return (void*)-1; //TODO: Is this right?
    HDWF handle = *((HDWF *)arg);
    pthread_t id = pthread_self();
    while (true)
    {
        //update a buffer with the latest set of data
        int cSamples = 0, cAvailable, cLost = 0, cCorrupted;
        double* buffCH1 = new double[samplesPerBuf];
        double* buffCH2 = new double[samplesPerBuf];
        bool fLost = false, fCorrupted = false;
        STS sts;

        //Start to record
        beginRecord(handle);

        while (cSamples < samplesPerBuf)
        {
            if (!FDwfAnalogInStatus(handle, true, &sts))
            {
              char szError[512];
              FDwfGetLastErrorMsg(szError);
              cout << "Error: " << szError << endl;
              break;
            }

            if (cSamples == 0 && (sts == stsCfg || sts == stsPrefill || sts == stsArm)) continue;

            FDwfAnalogInStatusRecord(handle, &cAvailable, &cLost, &cCorrupted);

            //Increment the counter by the number of samples lost
            //This ensures that our interval remains a constant time
            cSamples += cLost;

            //Set flag if lost or corrupted
            if (cLost) fLost = true;
            if(cCorrupted) fCorrupted = true;

            //Get the data
            FDwfAnalogInStatusData(handle, 0, &buffCH1[cSamples], cAvailable);
            FDwfAnalogInStatusData(handle, 1, &buffCH2[cSamples], cAvailable);

            //Increment counter to the next data point to acquire
            cSamples += cAvailable;
        }

        if (fLost) cout << "Samples were lost, reduce frequency" << endl;
        if (fCorrupted) cout << "Samples may be corrupted, reduce frequency" << endl;
        cout << buffCH1[0] << endl;
        cout << buffCH2[1] << endl;

        //TODO: Pass buffCH1 and buffCH2 into the shared memory queue
        if (id == threadId[0]) //Add to the first pair of buffers
        {
            qdev1ch1.push(buffCH1);
            qdev1ch2.push(buffCH2);
        }
        else if(id == threadId[1]) //Add to the second pair of buffers
        {
            qdev2ch1.push(buffCH1);
            qdev2ch2.push(buffCH2);
        }
        break;
    }
    return NULL;
}

//TODO: Add cross correlation threading
void *crossCorrelation(void * arg)
{
    while(true)
    {
        if (qdev1ch1.size() > 0)
        {
            double * b1 = qdev1ch1.front();
            qdev1ch1.pop();
            double * b2 = qdev1ch2.front();
            qdev1ch2.pop();

            int i = 0;
            cout << "[";
            for (i = 0; i < samplesPerBuf; ++i)
            {
                cout << b1[i] << ",";
            }
            cout << "] " << endl;
            delete[] b1;
            cout << "[";
            for (i = 0; i < samplesPerBuf; ++i)
            {
                cout << b2[i] << ",";
            }
            cout << "]" << endl;
            delete[] b2;

        }

        if (qdev2ch1.size() > 0)
        {            
            double * b3 = qdev2ch1.front();
            qdev2ch1.pop();
            double * b4 = qdev2ch2.front();
            qdev2ch2.pop();

            int i = 0;
            cout << "[";
            for (i = 0; i < samplesPerBuf; ++i)
            {
                cout << b3[i] << ",";
            }
            cout << "]" << endl;
            delete[] b3;
            cout << "[";
            for (i = 0; i < samplesPerBuf; ++i)
            {
                cout << b4[i] << ",";
            }
            cout << "]" << endl;
            delete[] b4;
        }
    }

    return (void *)0;
}
