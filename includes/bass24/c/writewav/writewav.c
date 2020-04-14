/*
	BASS console WAV writer
	Copyright (c) 2002-2008 Un4seen Developments Ltd.
*/

#include <stdlib.h>
#include <stdio.h>
#include "bass.h"

#ifdef _WIN32 // Windows
#include <conio.h>
#else // OSX
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>
#include <string.h>

int _kbhit()
{
	int r;
	fd_set rfds;
	struct timeval tv;
	struct termios term,oterm;
	tcgetattr(0,&oterm);
	memcpy(&term,&oterm,sizeof(term));
	cfmakeraw(&term);
	tcsetattr(0,TCSANOW,&term);
	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	tv.tv_sec=tv.tv_usec=0;
	r=select(1,&rfds,NULL,NULL,&tv);
	tcsetattr(0,TCSANOW,&oterm);
	return r;
}
#endif

#ifdef _BIG_ENDIAN
inline DWORD le_32(DWORD v)
{
	return (v>>24)|((v>>8)&0xff00)|((v&0xff00)<<8)|(v<<24);
}
inline WORD le_16(WORD v)
{
	return (v>>8)|(v<<8);
}
#else
#define le_32(v) (v)
#define le_16(v) (v)
#endif

// display error messages
void Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	exit(0);
}

void main(int argc, char **argv)
{
	BASS_CHANNELINFO info;
	DWORD chan,p;
	QWORD pos;
	FILE *fp;
	short buf[10000];
	WAVEFORMATEX wf;

	printf("BASS WAV writer example : MOD/MPx/OGG -> BASS.WAV\n"
			"-------------------------------------------------\n");

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS was loaded");
		return;
	}

	if (argc!=2) {
		printf("\tusage: writewav <file>\n");
		return;
	}

	// initalize "no sound" device
	if (!BASS_Init(0,44100,0,0,NULL))
		Error("Can't initialize device");

	// try streaming the file/url
	if ((chan=BASS_StreamCreateFile(FALSE,argv[1],0,0,BASS_STREAM_DECODE))
		|| (chan=BASS_StreamCreateURL(argv[1],0,BASS_STREAM_DECODE|BASS_STREAM_BLOCK,0,0))) {
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		if (pos==-1) // length not available
			printf("streaming file");
		else
#ifdef _WIN32
			printf("streaming file [%I64u bytes]",pos);
#else
			printf("streaming file [%llu bytes]",pos);
#endif
	} else {
		// try loading the MOD (with sensitive ramping, and calculate the duration)
		if (!(chan=BASS_MusicLoad(FALSE,argv[1],0,0,BASS_MUSIC_DECODE|BASS_MUSIC_RAMP|BASS_MUSIC_PRESCAN,0)))
			// not a MOD either
			Error("Can't play the file");
		{ // count channels
			float dummy;
			for (p=0;BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MUSIC_VOL_CHAN+p,&dummy);p++);
		}
		printf("MOD music \"%s\" [%u chans, %u orders]",
			BASS_ChannelGetTags(chan,BASS_TAG_MUSIC_NAME),p,(DWORD)BASS_ChannelGetLength(chan,BASS_POS_MUSIC_ORDER));
		pos=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
	}

	// display the time length
	if (pos && pos!=-1) {
		p=(DWORD)BASS_ChannelBytes2Seconds(chan,pos);
		printf(" %u:%02u\n",p/60,p%60);
	} else // no time length available
		printf("\n");

	if (!(fp=fopen("bass.wav","wb"))) Error("Can't create file");
	printf("writing to BASS.WAV file... press a key to stop\n");
	// write WAV header
	BASS_ChannelGetInfo(chan,&info);
	wf.wFormatTag=1;
	wf.nChannels=info.chans;
	wf.wBitsPerSample=(info.flags&BASS_SAMPLE_8BITS?8:16);
	wf.nBlockAlign=wf.nChannels*wf.wBitsPerSample/8;
	wf.nSamplesPerSec=info.freq;
	wf.nAvgBytesPerSec=wf.nSamplesPerSec*wf.nBlockAlign;
#ifdef _BIG_ENDIAN // swap byte order
	wf.wFormatTag=le_16(wf.wFormatTag);
	wf.nChannels=le_16(wf.nChannels);
	wf.wBitsPerSample=le_16(wf.wBitsPerSample);
	wf.nBlockAlign=le_16(wf.nBlockAlign);
	wf.nSamplesPerSec=le_32(wf.nSamplesPerSec);
	wf.nAvgBytesPerSec=le_32(wf.nAvgBytesPerSec);
#endif
	fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0",20,1,fp);
	fwrite(&wf,16,1,fp);
	fwrite("data\0\0\0\0",8,1,fp);

	while (!_kbhit() && BASS_ChannelIsActive(chan)) {
		int c=BASS_ChannelGetData(chan,buf,20000);
#ifdef _BIG_ENDIAN
		if (!(info.flags&BASS_SAMPLE_8BITS)) // swap 16-bit byte order
			for (p=0;p<c/2;p++) buf[p]=le_16(buf[p]);
#endif
		fwrite(buf,1,c,fp);
		pos=BASS_ChannelGetPosition(chan,BASS_POS_BYTE);
#ifdef _WIN32
		printf("pos %09I64u\r",pos);
#else
		printf("pos %09llu\r",pos);
#endif
		fflush(stdout);
	}
	// complete WAV header
	fflush(fp);
	p=ftell(fp);
	fseek(fp,4,SEEK_SET);
	putw(le_32(p-8),fp);
	fflush(fp);
	fseek(fp,40,SEEK_SET);
	putw(le_32(p-44),fp);
	fflush(fp);
	fclose(fp);

	BASS_Free();
}
