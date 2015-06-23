#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <digilent/waveforms/dwf.h>


enum {
	FALSE = 0,
	TRUE = 1
};

typedef struct DEV_INFO {
	double* data_buffer;
	int buffer_size;
	HDWF handle;
	char SN[32];
	DwfState status;

}Dev_Info;

#define READ_FREQUENCY 200000.0

int main (void)
{
	int num_dev;
	Dev_Info *devices;
	int i, j;
	int tmpint;
	double tmpf;
	DwfState status;
	int available_count, lost_count, corrupted_count;

	FDwfEnum(enumfilterAll, &num_dev);
	printf("%d device(s) found\n", num_dev);

	if(num_dev < 1)
		exit(1);

	//allocate structures for devices
	devices = (Dev_Info*)malloc(num_dev*sizeof(Dev_Info));

	printf("testing at %.0lfkHz\n", READ_FREQUENCY/1000);

	//configure the devices
	for(i=0; i < num_dev; ++i)
	{
		printf("opening and configuring device %d...", i);
		fflush(stdout);

		//open the device 
		FDwfDeviceOpen(i, &(devices[i].handle));
		//get info
		FDwfEnumSN(i, devices[i].SN);

		//configure devices for continuous acquisition
		FDwfAnalogInBufferSizeInfo(devices[i].handle, NULL, &(devices[i].buffer_size)); //get max buffer size
		devices[i].data_buffer = malloc(devices[i].buffer_size*sizeof(double)); //allocate memory to store the data	
		FDwfAnalogInBufferSizeSet(devices[i].handle, devices[i].buffer_size);
		FDwfAnalogInBufferSizeGet(devices[i].handle, &tmpint);
		if(tmpint != devices[i].buffer_size)
			fprintf(stderr, "buffer set to %d, wanted %d\n", tmpint, devices[i].buffer_size); 


		//set the frequency	
		FDwfAnalogInFrequencySet(devices[i].handle, READ_FREQUENCY);
		FDwfAnalogInFrequencyGet(devices[i].handle, &tmpf);
		if(tmpf != READ_FREQUENCY)
			fprintf(stderr, "frequency set to %lf, wanted %lf\n", tmpf, READ_FREQUENCY);

		FDwfAnalogInAcquisitionModeSet(devices[i].handle, acqmodeRecord); //set for record mode	
		FDwfAnalogInRecordLengthSet(devices[i].handle, -1.0);
			
		//configure all the channels
		FDwfAnalogInChannelCount(devices[i].handle, &tmpint);
		for(j=0; j < tmpint; ++j)
		{
			FDwfAnalogInChannelEnableSet(devices[i].handle, j, TRUE); 
			FDwfAnalogInChannelFilterSet(devices[i].handle, j, filterAverage);
			FDwfAnalogInChannelRangeSet(devices[i].handle, j, 5);
			FDwfAnalogInChannelOffsetSet(devices[i].handle, j, 0);
		}
		//FDwfAnalogInChannelEnableSet(devices[i].handle, 1, FALSE); 

		printf("done!\n");
	}

	printf("waiting for configuration changes to stabilize...");
	fflush(stdout);
	sleep(2); //sleep for two seconds to allow configuration changes to stabilize
	printf("done\n");


	//start devices sampling
	printf("starting sampling...\n");
	for(i=0; i < num_dev; ++i)
	{
		FDwfAnalogInConfigure(devices[i].handle, FALSE, TRUE);
	}

	//read the devices
	while(TRUE)
	{
		for(i=0; i < num_dev; ++i) //get device statuses
		{
			FDwfAnalogInStatus(devices[i].handle, TRUE, &status); //update status
			if(status == stsCfg || status == stsPrefill || status == stsArm)
			{
            	// Acquisition not yet started.
            	continue;
        	}
        	
			FDwfAnalogInStatusRecord(devices[i].handle, &available_count, &lost_count, &corrupted_count);

			

			if(available_count)
			{
				FDwfAnalogInStatusData(devices[i].handle, 0, devices[i].data_buffer, available_count); //read ch1 buffer
				FDwfAnalogInStatusData(devices[i].handle, 1, devices[i].data_buffer, available_count); //read ch2 buffer
			}
			else //we could sleep here if no data is available, may save some cpu
			{
				//usleep(1000);
			}

			//printf("avail: %d\n", available_count);

			if(lost_count || corrupted_count)
			{
				printf("data samples were lost!\n");
				printf("avail: %d\n", available_count);
				printf("lost: %d\n", lost_count);
				printf("corrupted: %d\n\n", corrupted_count);
				FDwfDeviceCloseAll();
				exit(1); 
			} 
		}
	}

	FDwfDeviceCloseAll();
	return 0;
}
