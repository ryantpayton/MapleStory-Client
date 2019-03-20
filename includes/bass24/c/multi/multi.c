/*
	BASS multiple output example
	Copyright (c) 2001-2008 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "bass.h"

HWND win=NULL;

DWORD outdev[2];	// output devices
DWORD latency[2];	// latencies
HSTREAM chan[2];	// the streams

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// Cloning DSP function
void CALLBACK CloneDSP(HDSP handle, DWORD channel, void *buffer, DWORD length, void *user)
{
	BASS_StreamPutData((HSTREAM)user,buffer,length); // user = clone
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;

	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10: // open a file to play on device #1
				case 11: // open a file to play on device #2
					{
						int devn=LOWORD(w)-10;
						char file[MAX_PATH]="";
						ofn.lpstrFilter="streamable files\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							BASS_StreamFree(chan[devn]); // free old stream
							BASS_SetDevice(outdev[devn]); // set the device to create stream on
							if (!(chan[devn]=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))) {
								MESS(10+devn,WM_SETTEXT,0,"click here to open a file...");
								Error("Can't play the file");
								break;
							}
							BASS_ChannelPlay(chan[devn],FALSE); // play new stream
							MESS(10+devn,WM_SETTEXT,0,file);
						}
					}
					break;
				case 15: // clone on device #1
				case 16: // clone on device #2
					{
						int devn=LOWORD(w)-15;
						BASS_CHANNELINFO i;
						if (!BASS_ChannelGetInfo(chan[devn^1],&i)) {
							Error("Nothing to clone");
							break;
						}
						BASS_StreamFree(chan[devn]); // free old stream
						BASS_SetDevice(outdev[devn]); // set the device to create stream on
						if (!(chan[devn]=BASS_StreamCreate(i.freq,i.chans,i.flags,STREAMPROC_PUSH,0))) { // create a "push" stream
							MESS(10+devn,WM_SETTEXT,0,"click here to open a file...");
							Error("Can't create clone");
							break;
						}
						BASS_ChannelLock(chan[devn^1],TRUE); // lock source stream to synchonise buffer contents
						BASS_ChannelSetDSP(chan[devn^1],CloneDSP,(void*)chan[devn],0); // set DSP to feed data to clone
						{ // copy buffered data to clone
							DWORD d=BASS_ChannelSeconds2Bytes(chan[devn],latency[devn]/1000.f); // playback delay
							DWORD c=BASS_ChannelGetData(chan[devn^1],0,BASS_DATA_AVAILABLE);
							BYTE *buf=(BYTE*)malloc(c);
							c=BASS_ChannelGetData(chan[devn^1],buf,c);
							if (c>d) BASS_StreamPutData(chan[devn],buf+d,c-d);
							free(buf);
						}
						BASS_ChannelLock(chan[devn^1],FALSE); // unlock source stream
						BASS_ChannelPlay(chan[devn],FALSE); // play clone
						MESS(10+devn,WM_SETTEXT,0,"clone");
					}
					break;
				case 30: // swap channel devices
					{
						{ // swap handles
							HSTREAM temp=chan[0];
							chan[0]=chan[1];
							chan[1]=temp;
						}
						{ // swap text
							char temp1[MAX_PATH],temp2[MAX_PATH];
							MESS(10,WM_GETTEXT,MAX_PATH,temp1);
							MESS(11,WM_GETTEXT,MAX_PATH,temp2);
							MESS(10,WM_SETTEXT,0,temp2);
							MESS(11,WM_SETTEXT,0,temp1);
						}
						// update the channel devices
						BASS_ChannelSetDevice(chan[0],outdev[0]);
						BASS_ChannelSetDevice(chan[1],outdev[1]);
					}
					break;
			}
			break;

		case WM_INITDIALOG:
			win=h;
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			{ // initialize output devices
				BASS_INFO info;
				if (!BASS_Init(outdev[0],44100,BASS_DEVICE_LATENCY,win,NULL)) {
					Error("Can't initialize device 1");
					DestroyWindow(win);
				}
				BASS_GetInfo(&info);
				latency[0]=info.latency;
				if (!BASS_Init(outdev[1],44100,BASS_DEVICE_LATENCY,win,NULL)) {
					Error("Can't initialize device 2");
					DestroyWindow(win);
				}
				BASS_GetInfo(&info);
				latency[1]=info.latency;
			}
			{
				BASS_DEVICEINFO i;
				BASS_GetDeviceInfo(outdev[0],&i);
				MESS(20,WM_SETTEXT,0,i.name);
				BASS_GetDeviceInfo(outdev[1],&i);
				MESS(21,WM_SETTEXT,0,i.name);
			}
			return 1;

		case WM_DESTROY:
			// release both devices
			BASS_SetDevice(outdev[0]);
			BASS_Free();
			BASS_SetDevice(outdev[1]);
			BASS_Free();
			break;
	}
	return 0;
}


// Simple device selector dialog stuff begins here
INT_PTR CALLBACK devicedialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case 10:
					if (HIWORD(w)!=LBN_DBLCLK) break;
				case IDOK:
					{
						int device=SendDlgItemMessage(h,10,LB_GETCURSEL,0,0);
						device=SendDlgItemMessage(h,10,LB_GETITEMDATA,device,0); // get device #
						EndDialog(h,device);
					}
					break;
			}
			break;

		case WM_INITDIALOG:
			{
				char text[30];
				BASS_DEVICEINFO i;
				int c;
				sprintf(text,"Select output device #%d",l);
				SetWindowText(h,text);
				for (c=1;BASS_GetDeviceInfo(c,&i);c++) { // device 1 = 1st real device
					if (i.flags&BASS_DEVICE_ENABLED) { // enabled, so add it...
						int idx=SendDlgItemMessage(h,10,LB_ADDSTRING,0,(LPARAM)i.name);
						SendDlgItemMessage(h,10,LB_SETITEMDATA,idx,c); // store device #
					}
				}
				SendDlgItemMessage(h,10,LB_SETCURSEL,0,0);
			}
			return 1;
	}
	return 0;
}
// Device selector stuff ends here

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	// Let the user choose the output devices
	outdev[0]=DialogBoxParam(hInstance,(char*)2000,0,&devicedialogproc,1);
	outdev[1]=DialogBoxParam(hInstance,(char*)2000,0,&devicedialogproc,2);

	// main dialog
	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}

