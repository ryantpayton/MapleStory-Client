/*
	BASS multi-speaker example
	Copyright (c) 2003-2014 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include "bass.h"

HWND win=NULL;

DWORD flags[4]={BASS_SPEAKER_FRONT,BASS_SPEAKER_REAR,BASS_SPEAKER_CENLFE,BASS_SPEAKER_REAR2};
HSTREAM chan[4];

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))
#define ITEM(id) GetDlgItem(win,id)

void UpdateSpeakerFlags(DWORD speaker)
{
	int mono=MESS(30+speaker*2,BM_GETCHECK,0,0)|(MESS(30+speaker*2+1,BM_GETCHECK,0,0)<<1); // get mono switch states
	BASS_ChannelFlags(chan[speaker],flags[speaker]|(mono==1?BASS_SPEAKER_LEFT:mono==2?BASS_SPEAKER_RIGHT:0),BASS_SPEAKER_FRONT); // update speaker flags
}

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;

	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					return 1;
				case 10: // open a file to play on #1
				case 11: // open a file to play on #2
				case 12: // open a file to play on #3
				case 13: // open a file to play on #4
					{
						int speaker=LOWORD(w)-10;
						char file[MAX_PATH]="";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							BASS_StreamFree(chan[speaker]); // free old stream before opening new
							if (!(chan[speaker]=BASS_StreamCreateFile(FALSE,file,0,0,flags[speaker]|BASS_SAMPLE_LOOP))) {
								MESS(10+speaker,WM_SETTEXT,0,"click here to open a file...");
								Error("Can't play the file");
								return 1;
							}
							MESS(10+speaker,WM_SETTEXT,0,file);
							{ // reset mono speaker switches
								BASS_CHANNELINFO ci;
								BASS_ChannelGetInfo(chan[speaker],&ci);
								MESS(30+speaker*2,BM_SETCHECK,0,0);
								MESS(30+speaker*2+1,BM_SETCHECK,0,0);
								EnableWindow(ITEM(30+speaker*2),ci.chans==1);
								EnableWindow(ITEM(30+speaker*2+1),ci.chans==1);
							}
							BASS_ChannelPlay(chan[speaker],FALSE);
						}
					}
					return 1;
				case 20: // swap #1 & #2
				case 21: // swap #2 & #3
				case 22: // swap #3 & #4
					{
						int speaker=LOWORD(w)-20;
						{ // swap handles
							HSTREAM temp=chan[speaker];
							chan[speaker]=chan[speaker+1];
							chan[speaker+1]=temp;
						}
						{ // swap text
							char temp1[MAX_PATH],temp2[MAX_PATH];
							MESS(10+speaker,WM_GETTEXT,MAX_PATH,temp1);
							MESS(10+speaker+1,WM_GETTEXT,MAX_PATH,temp2);
							MESS(10+speaker,WM_SETTEXT,0,temp2);
							MESS(10+speaker+1,WM_SETTEXT,0,temp1);
						}
						{ // swap mono switch states
							int temp=MESS(30+speaker*2,BM_GETCHECK,0,0);
							MESS(30+speaker*2,BM_SETCHECK,MESS(32+speaker*2,BM_GETCHECK,0,0),0);
							MESS(32+speaker*2,BM_SETCHECK,temp,0);
							temp=MESS(31+speaker*2,BM_GETCHECK,0,0);
							MESS(31+speaker*2,BM_SETCHECK,MESS(33+speaker*2,BM_GETCHECK,0,0),0);
							MESS(33+speaker*2,BM_SETCHECK,temp,0);
							temp=IsWindowEnabled(ITEM(30+speaker*2));
							EnableWindow(ITEM(30+speaker*2),IsWindowEnabled(ITEM(32+speaker*2)));
							EnableWindow(ITEM(32+speaker*2),temp);
							temp=IsWindowEnabled(ITEM(31+speaker*2));
							EnableWindow(ITEM(31+speaker*2),IsWindowEnabled(ITEM(33+speaker*2)));
							EnableWindow(ITEM(33+speaker*2),temp);
						}
						// update speaker flags
						UpdateSpeakerFlags(speaker);
						UpdateSpeakerFlags(speaker+1);
					}
					return 1;
				case 30: // left #1
				case 31: // right #1
				case 32: // left #2
				case 33: // right #2
				case 34: // left #3
				case 35: // right #3
				case 36: // right #4
				case 37: // right #4
					{
						int speaker=(LOWORD(w)-30)/2;
						UpdateSpeakerFlags(speaker);
					}
					return 1;
			}
			break;

		case WM_INITDIALOG:
			win=h;
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			ofn.lpstrFilter="Streamable files\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
			// initialize BASS - default device
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				DestroyWindow(win);
				break;
			}
			{ // check how many speakers the device supports
				BASS_INFO i;
				BASS_GetInfo(&i);
				if (i.speakers<4 && LOBYTE(GetVersion())<6) { // no extra speakers detected, enable them anyway? (on older Windows than Vista)
					if (MessageBox(0,"Do you wish to enable \"speaker assignment\" anyway?","No extra speakers detected",MB_ICONQUESTION|MB_YESNO)==IDYES) {
						// reinitialize BASS - forcing speaker assignment
						BASS_Free();
						if (!BASS_Init(-1,44100,BASS_DEVICE_SPEAKERS,win,NULL)) {
							Error("Can't initialize device");
							DestroyWindow(win);
							break;
						}
						BASS_GetInfo(&i); // get info again
					}
				}
				if (i.speakers<8) {
					EnableWindow(GetDlgItem(h,13),FALSE);
					EnableWindow(GetDlgItem(h,22),FALSE);
				}
				if (i.speakers<6) {
					EnableWindow(GetDlgItem(h,12),FALSE);
					EnableWindow(GetDlgItem(h,21),FALSE);
				}
				if (i.speakers<4) {
					EnableWindow(GetDlgItem(h,11),FALSE);
					EnableWindow(GetDlgItem(h,20),FALSE);
				}
			}
			return 1;

		case WM_DESTROY:
			BASS_Free();
			break;
	}
	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	// main dialog
	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
