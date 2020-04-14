/*
	BASS simple synth
	Copyright (c) 2001-2017 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "bass.h"

BASS_INFO info;
HSTREAM stream; // the stream

// display error messages
void Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
	ExitProcess(0);
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define KEYS 20
const WORD keys[KEYS]={
	'Q','2','W','3','E','R','5','T','6','Y','7','U',
	'I','9','O','0','P',219,187,221
};
#define MAXVOL (0.22*32768)
#define DECAY (MAXVOL/4000)
float vol[KEYS]={0},pos[KEYS]; // keys' volume and pos

DWORD CALLBACK WriteStream(HSTREAM handle, short *buffer, DWORD length, void *user)
{
	int k,c,s;
	float omega;
	memset(buffer,0,length);
	for (k=0;k<KEYS;k++) {
		if (!vol[k]) continue;
		omega=2*M_PI*pow(2.0,(k+3)/12.0)*440.0/info.freq;
		for (c=0;c<length/sizeof(short);c+=2) {
			s=buffer[c]+sin(pos[k])*vol[k];
			if (s>32767) s=32767;
			else if (s<-32768) s=-32768;
			buffer[c+1]=buffer[c]=s; // left and right channels are the same
			pos[k]+=omega;
			if (vol[k]<MAXVOL) {
				vol[k]-=DECAY;
				if (vol[k]<=0) { // faded-out
					vol[k]=0;
					break;
				}
			}
		}
		pos[k]=fmod(pos[k],2*M_PI);
	}
	return length;
}

void main(int argc, char **argv)
{
	const char *fxname[9]={"CHORUS","COMPRESSOR","DISTORTION","ECHO",
		"FLANGER","GARGLE","I3DL2REVERB","PARAMEQ","REVERB"};
	HFX fx[9]={0}; // effect handles
	INPUT_RECORD keyin;
	DWORD r,buflen;

	printf("BASS Simple Sinewave Synth\n"
			"--------------------------\n");

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS.DLL was loaded");
		return;
	}

	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10); // 10ms update period

	// initialize default output device (and measure latency)
	if (!BASS_Init(-1,44100,BASS_DEVICE_LATENCY,0,NULL))
		Error("Can't initialize device");

	BASS_SetConfig(BASS_CONFIG_BUFFER, 200); // set default/maximum buffer length to 200ms
	BASS_GetInfo(&info);
	if (!info.freq) info.freq=44100; // if the device's output rate is unknown, default to 44100 Hz
	stream=BASS_StreamCreate(info.freq,2,0,(STREAMPROC*)WriteStream,0); // create a stream (stereo for effects)
	buflen=10+info.minbuf+1; // default buffer size = update period + 'minbuf' + 1ms extra margin
	BASS_ChannelSetAttribute(stream,BASS_ATTRIB_BUFFER,buflen/1000.f); // apply it
	BASS_ChannelPlay(stream,FALSE); // start it

	printf("device latency: %dms\n",info.latency);
	printf("device minbuf: %dms\n",info.minbuf);
	printf("ds version: %d (effects %s)\n",info.dsver,info.dsver<8?"disabled":"enabled");
	printf("press these keys to play:\n\n"
		"  2 3  5 6 7  9 0  =\n"
		" Q W ER T Y UI O P[ ]\n\n"
		"press -/+ to de/increase the buffer\n"
		"press spacebar to quit\n\n");
	if (info.dsver>=8) // DX8 effects available
		printf("press F1-F9 to toggle effects\n\n");
	printf("using a %dms buffer\r",buflen);

	while (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),&keyin,1,&r)) {
		int key;
		if (keyin.EventType!=KEY_EVENT) continue;
		if (keyin.Event.KeyEvent.wVirtualKeyCode==VK_SPACE) break;
		if (keyin.Event.KeyEvent.bKeyDown) {
			if (keyin.Event.KeyEvent.wVirtualKeyCode==VK_SUBTRACT
				|| keyin.Event.KeyEvent.wVirtualKeyCode==VK_ADD) {
				// change the buffer size
				if (keyin.Event.KeyEvent.wVirtualKeyCode==VK_SUBTRACT) { // smaller buffer
					if (buflen>11)
						buflen--;
					else if (!(info.initflags&BASS_DEVICE_DSOUND))
						buflen=0; // no buffer (not available with DirectSound)
				} else { // bigger buffer
					if (!buflen)
						buflen=11;
					else if (buflen<200)
						buflen++;
				}
				BASS_ChannelSetAttribute(stream,BASS_ATTRIB_BUFFER,buflen/1000.f); // apply it
				if (buflen)
					printf("using a %dms buffer\t\t\r",buflen);
				else
					printf("using no buffer\t\t\r");
			}
			if (keyin.Event.KeyEvent.wVirtualKeyCode>=VK_F1
				&& keyin.Event.KeyEvent.wVirtualKeyCode<=VK_F9) {
				r=keyin.Event.KeyEvent.wVirtualKeyCode-VK_F1;
				if (fx[r]) {
					BASS_ChannelRemoveFX(stream,fx[r]);
					fx[r]=0;
					printf("effect %s = OFF\t\t\r",fxname[r]);
				} else {
					// set the effect, not bothering with parameters (use defaults)
					if (fx[r]=BASS_ChannelSetFX(stream,BASS_FX_DX8_CHORUS+r,0))
						printf("effect %s = ON\t\t\r",fxname[r]);
				}
			}
		}
		for (key=0;key<KEYS;key++)
			if (keyin.Event.KeyEvent.wVirtualKeyCode==keys[key]) {
				if (keyin.Event.KeyEvent.bKeyDown && vol[key]<MAXVOL) {
					pos[key]=0;
					vol[key]=MAXVOL+DECAY/2; // start key (setting "vol" slightly higher than MAXVOL to cover any rounding-down)
				} else if (!keyin.Event.KeyEvent.bKeyDown && vol[key])
					vol[key]-=DECAY; // trigger key fadeout
				break;
			}
	}

	BASS_Free();
}
