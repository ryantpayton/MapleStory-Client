/*
	BASS device list example
	Copyright (c) 2014-2019 Un4seen Developments Ltd.
*/

#include <stdio.h>
#include "bass.h"

void DisplayDeviceInfo(BASS_DEVICEINFO *di)
{
#if 0//def _WIN32
	const char *path=di->driver+strlen(di->driver)+1;
	if (path[0])
		printf("%s\n\tdriver: %s\n\tpath: %s\n\ttype: ",di->name,di->driver,path);
	else
#endif
	printf("%s\n\tdriver: %s\n\ttype: ",di->name,di->driver);
	switch (di->flags&BASS_DEVICE_TYPE_MASK) {
		case BASS_DEVICE_TYPE_NETWORK:
			printf("Remote Network");
			break;
		case BASS_DEVICE_TYPE_SPEAKERS:
			printf("Speakers");
			break;
		case BASS_DEVICE_TYPE_LINE:
			printf("Line");
			break;
		case BASS_DEVICE_TYPE_HEADPHONES:
			printf("Headphones");
			break;
		case BASS_DEVICE_TYPE_MICROPHONE: 
			printf("Microphone");
			break;
		case BASS_DEVICE_TYPE_HEADSET:
			printf("Headset");
			break;
		case BASS_DEVICE_TYPE_HANDSET:
			printf("Handset");
			break;
		case BASS_DEVICE_TYPE_DIGITAL:
			printf("Digital");
			break;
		case BASS_DEVICE_TYPE_SPDIF:
			printf("SPDIF");
			break;
		case BASS_DEVICE_TYPE_HDMI:
			printf("HDMI");
			break;
		case BASS_DEVICE_TYPE_DISPLAYPORT:
			printf("DisplayPort");
			break;
		default:
			printf("Unknown");
	}
	printf("\n\tflags:");
	if (di->flags&BASS_DEVICE_LOOPBACK) printf(" loopback");
	if (di->flags&BASS_DEVICE_ENABLED) printf(" enabled");
	if (di->flags&BASS_DEVICE_DEFAULT) printf(" default");
	printf(" (%x)\n",di->flags);
}

void main()
{
	BASS_DEVICEINFO di;
	int a;
	printf("Output Devices\n");
	for (a=1;BASS_GetDeviceInfo(a,&di);a++) {
		printf("%d: ",a);
		DisplayDeviceInfo(&di);
	}
	printf("\nInput Devices\n");
	for (a=0;BASS_RecordGetDeviceInfo(a,&di);a++) {
		printf("%d: ",a);
		DisplayDeviceInfo(&di);
	}
}
