#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <digilent/waveforms/dwf.h>

enum {
	FALSE = 0,
	TRUE  = 1
};

typedef struct DEV_INFO {
	double* data_buffer;
	int buffer_size;
	HDWF handle;
	char SN[32];
	DwfState status;

}Dev_Info;

#define READ_FREQUENCY 10000.0

int main (void)
{
	int num_dev;
	Dev_Info *devices;
	int i, j;
	int tmpint;
	double tmpf;

	FDwfEnum(enumfilterAll, &num_dev);
	printf("%d device(s) found\n", num_dev);

	if(num_dev < 1)
		exit(1);

	//allocate structures for devices
	devices = (Dev_Info*)malloc(num_dev*sizeof(Dev_Info));

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

		FDwfAnalogInAcquisitionModeSet(devices[i].handle, acqmodeScanShift); //set for FIFO mode	

		//configure all the channels
		FDwfAnalogInChannelCount(devices[i].handle, &tmpint);
		for(j=0; j < tmpint; ++j)
		{
			FDwfAnalogInChannelEnableSet(devices[i].handle, j, TRUE); 
			FDwfAnalogInChannelFilterSet(devices[i].handle, j, filterAverage);
			FDwfAnalogInChannelRangeSet(devices[i].handle, j, 5);
			FDwfAnalogInChannelOffsetSet(devices[i].handle, j, 0);
		}

		sleep(2); //sleep for two seconds to allow configuration changes to stabilize
		printf("done!\n");
	}

	


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
			FDwfAnalogInStatus(devices[i].handle, TRUE, &(devices[i].status)); //update status
			FDwfAnalogInStatusSamplesValid(devices[i].handle, &tmpint); //get how many samples are in buffer
			FDwfAnalogInStatusData(devices[i].handle, 0, devices[i].data_buffer, tmpint); //read ch1 buffer
			FDwfAnalogInStatusData(devices[i].handle, 1, devices[i].data_buffer, tmpint); //read ch2 buffer
			printf("buffer size: %d\n", tmpint);
			if(tmpint > devices[i].buffer_size-1)
			{
				printf("buffer full!\n");
				FDwfDeviceCloseAll();
				exit(1); 
			} 
		}
	}

	FDwfDeviceCloseAll();
	return 0;
}
