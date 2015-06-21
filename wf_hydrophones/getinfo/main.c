#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <sys/time.h>
#include <digilent/waveforms/dwf.h>

int main (void)
{
	int num_dev;
	char tmpstr[32];
	BOOL open;
	int i, j;
	HDWF devh;
	int tmpint, tmpint2;
	double tmpf, tmpf2, tmpf3;
	double farr[32];

	FDwfEnum(enumfilterAll, &num_dev);
	printf("%d device(s) found\n", num_dev);

	if(num_dev < 1)
		exit(1);

	//list all devices
	for(i=0; i < num_dev; ++i)
	{
		FDwfEnumDeviceIsOpened(i, &open);
		FDwfEnumDeviceName(i, tmpstr);
		printf("device %d: %s\n", i, tmpstr);

		FDwfEnumSN(i, tmpstr);
		printf("device id: %s\n", tmpstr);

		printf("open: %d\n", open);

		FDwfEnumConfigInfo(i, DECIAnalogInChannelCount, &tmpint);
		printf("analog channels: %d\n", tmpint);

		FDwfEnumConfigInfo(i, DECIAnalogInBufferSize, &tmpint);
		printf("analog buffer size: %d\n", tmpint);

		printf("opening device...");
		fflush(stdout);
		FDwfDeviceOpen(i, &devh);
		printf("done!\n");

		FDwfAnalogInFrequencyInfo(devh, &tmpf, &tmpf2);
		printf("min/max frequency: %lf/%lf\n", tmpf, tmpf2);

		FDwfAnalogInBitsInfo(devh, &tmpint);
		printf("resolution: %d bits\n", tmpint);

		FDwfAnalogInBufferSizeInfo(devh, &tmpint, &tmpint2);
		printf("min/max buffer size: %d/%d\n", tmpint, tmpint2);

		FDwfAnalogInNoiseSizeInfo(devh, &tmpint);
		printf("max noise buffer size: %d\n", tmpint);

		FDwfAnalogInAcquisitionModeInfo(devh, &tmpint);
		printf("supported acquisition modes:\n");
		if(IsBitSet(tmpint, acqmodeSingle))
			printf("\tSingle\n");
		if(IsBitSet(tmpint, acqmodeScanShift))
			printf("\tScanShift\n");
		if(IsBitSet(tmpint, acqmodeScanScreen))
			printf("\tScanScreen\n");
		if(IsBitSet(tmpint, acqmodeRecord))
			printf("\tRecord\n");

		FDwfAnalogInChannelCount(devh, &tmpint);
		printf("analog channels: %d\n", tmpint);

		FDwfAnalogInChannelFilterInfo(devh, &tmpint);
		printf("supported filter modes:\n");
		if(IsBitSet(tmpint, filterDecimate))
			printf("\tDecimate\n");
		if(IsBitSet(tmpint, filterAverage))
			printf("\tAverage\n");
		if(IsBitSet(tmpint, filterMinMax))
			printf("\tMinMax\n");

		FDwfAnalogInChannelRangeInfo(devh, &tmpf, &tmpf2, &tmpf3);
		printf("voltage range, steps: %lf-%lf, %lf\n", tmpf, tmpf2, tmpf3);

		FDwfAnalogInChannelRangeSteps(devh, farr, &tmpint);
		printf("range steps:\n");
		for(j=0; j < tmpint; ++j)
			printf("\t%lf\n", farr[j]);


		printf("closing device...");
		fflush(stdout);
		FDwfDeviceClose(devh);
		printf("done!\n");

		putchar('\n');
		putchar('\n');
	}

	return 0;
}
