/*
	BASS recording example
	Copyright (c) 2002-2019 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include "bass.h"

HWND win=NULL;

#define FREQ 44100
#define CHANS 2
#define BUFSTEP 200000	// memory allocation unit

int input;				// current input source
BYTE *recbuf=NULL;		// recording buffer
DWORD reclen;			// recording length
HRECORD rchan=0;		// recording channel
HSTREAM chan=0;			// playback channel

void StopRecording();

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// messaging macros
#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))
#define DLGITEM(id) GetDlgItem(win,id)

// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	// increase buffer size if needed
	if ((reclen%BUFSTEP)+length>=BUFSTEP) {
		recbuf=realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
		if (!recbuf) {
			rchan=0;
			Error("Out of memory!");
			MESS(10,WM_SETTEXT,0,"Record");
			return FALSE; // stop recording
		}
	}
	// buffer the data
	memcpy(recbuf+reclen,buffer,length);
	reclen+=length;
	return TRUE; // continue recording
}

void StartRecording()
{
	WAVEFORMATEX *wf;
	if (recbuf) { // free old recording
		BASS_StreamFree(chan);
		chan=0;
		free(recbuf);
		recbuf=NULL;
		EnableWindow(DLGITEM(11),FALSE);
		EnableWindow(DLGITEM(12),FALSE);
	}
	// allocate initial buffer and make space for WAVE header
	recbuf=malloc(BUFSTEP);
	reclen=44;
	// fill the WAVE header
	memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0",20);
	memcpy(recbuf+36,"data\0\0\0\0",8);
	wf=(WAVEFORMATEX*)(recbuf+20);
	wf->wFormatTag=1;
	wf->nChannels=CHANS;
	wf->wBitsPerSample=16;
	wf->nSamplesPerSec=FREQ;
	wf->nBlockAlign=wf->nChannels*wf->wBitsPerSample/8;
	wf->nAvgBytesPerSec=wf->nSamplesPerSec*wf->nBlockAlign;
	// start recording
	rchan=BASS_RecordStart(FREQ,CHANS,0,RecordingCallback,0);
	if (!rchan) {
		Error("Can't start recording");
		free(recbuf);
		recbuf=0;
		return;
	}
	MESS(10,WM_SETTEXT,0,"Stop");
}

void StopRecording()
{
	BASS_ChannelStop(rchan);
	rchan=0;
	MESS(10,WM_SETTEXT,0,"Record");
	// complete the WAVE header
	*(DWORD*)(recbuf+4)=reclen-8;
	*(DWORD*)(recbuf+40)=reclen-44;
	// enable "save" button
	EnableWindow(DLGITEM(12),TRUE);
	// create a stream from the recording
	if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,0))
		EnableWindow(DLGITEM(11),TRUE); // enable "play" button
}

// write the recorded data to disk
void WriteToDisk()
{
	FILE *fp;
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrFilter="WAV files\0*.wav\0All files\0*.*\0\0";
	ofn.lpstrDefExt="wav";
	if (!GetSaveFileName(&ofn)) return;
	if (!(fp=fopen(file,"wb"))) {
		Error("Can't create the file");
		return;
	}
	fwrite(recbuf,reclen,1,fp);
	fclose(fp);
}

void UpdateInputInfo()
{
	char *type;
	float level;
	int it=BASS_RecordGetInput(input,&level); // get info on the input
	if (it==-1 || level<0) { // failed to get level
		it=BASS_RecordGetInput(-1,&level); // try master input instead
		if (it==-1 || level<0) { // that failed too
			level=1; // just display 100%
			EnableWindow(DLGITEM(14),FALSE);
		} else
			EnableWindow(DLGITEM(14),TRUE);
	} else
		EnableWindow(DLGITEM(14),TRUE);
	MESS(14,TBM_SETPOS,TRUE,level*100); // set the level slider
	switch (it&BASS_INPUT_TYPE_MASK) {
		case BASS_INPUT_TYPE_DIGITAL:
			type="digital";
			break;
		case BASS_INPUT_TYPE_LINE:
			type="line-in";
			break;
		case BASS_INPUT_TYPE_MIC:
			type="microphone";
			break;
		case BASS_INPUT_TYPE_SYNTH:
			type="midi synth";
			break;
		case BASS_INPUT_TYPE_CD:
			type="analog cd";
			break;
		case BASS_INPUT_TYPE_PHONE:
			type="telephone";
			break;
		case BASS_INPUT_TYPE_SPEAKER:
			type="pc speaker";
			break;
		case BASS_INPUT_TYPE_WAVE:
			type="wave/pcm";
			break;
		case BASS_INPUT_TYPE_AUX:
			type="aux";
			break;
		case BASS_INPUT_TYPE_ANALOG:
			type="analog";
			break;
		default:
			type="undefined";
			{
				// check if it's a loopback device
				BASS_DEVICEINFO info;
				BASS_RecordGetDeviceInfo(BASS_RecordGetDevice(),&info);
				if (info.flags&BASS_DEVICE_LOOPBACK) type="loopback";
			}
	}
	MESS(15,WM_SETTEXT,0,type); // display the type
}

BOOL InitDevice(int device)
{
	BASS_RecordFree(); // free current device (and recording channel) if there is one
	// initalize new device
	if (!BASS_RecordInit(device)) {
		Error("Can't initialize recording device");
		return FALSE;
	}
	{ // get list of inputs
		int c;
		const char *i;
		MESS(13,CB_RESETCONTENT,0,0);
		input=0;
		for (c=0;i=BASS_RecordGetInputName(c);c++) {
			MESS(13,CB_ADDSTRING,0,i);
			if (!(BASS_RecordGetInput(c,NULL)&BASS_INPUT_OFF)) { // this one is currently "on"
				input=c;
				MESS(13,CB_SETCURSEL,input,0);
			}
		}
		UpdateInputInfo();
	}
	return TRUE;
}

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_TIMER:
			{ // update the recording/playback counter
				char text[30]="";
				if (rchan) { // recording
					if (rchan!=1 && !BASS_ChannelIsActive(rchan)) { // the recording has stopped, eg. unplugged device
						StopRecording();
						Error("The recording stopped");
						break;
					}
					sprintf(text,"%d",reclen-44);
				} else if (chan) {
					if (BASS_ChannelIsActive(chan)) // playing
						sprintf(text,"%I64d / %I64d",BASS_ChannelGetPosition(chan,BASS_POS_BYTE),BASS_ChannelGetLength(chan,BASS_POS_BYTE));
					else
						sprintf(text,"%I64d",BASS_ChannelGetLength(chan,BASS_POS_BYTE));
				}
				MESS(20,WM_SETTEXT,0,text);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10:
					if (!rchan)
						StartRecording();
					else
						StopRecording();
					break;
				case 11:
					BASS_ChannelPlay(chan,TRUE); // play the recorded data
					break;
				case 12:
					WriteToDisk();
					break;
				case 13:
					if (HIWORD(w)==CBN_SELCHANGE) { // input selection changed
						int i;
						input=MESS(13,CB_GETCURSEL,0,0); // get the selection
						// enable the selected input
						for (i=0;BASS_RecordSetInput(i,BASS_INPUT_OFF,-1);i++) ; // 1st disable all inputs, then...
						BASS_RecordSetInput(input,BASS_INPUT_ON,-1); // enable the selected
						UpdateInputInfo();
					}
					break;
				case 16:
					if (HIWORD(w)==CBN_SELCHANGE) { // device selection changed
						int i=MESS(16,CB_GETCURSEL,0,0); // get the selection
						if (rchan) rchan=1; // special handle (real handles always have highest bit set) to prevent timer ending the recording
						// initialize the selected device
						if (InitDevice(i)) {
							if (rchan) { // continue recording on the new device...
								HRECORD newrchan=BASS_RecordStart(FREQ,CHANS,0,RecordingCallback,NULL);
								if (!newrchan)
									Error("Couldn't start recording");
								else
									rchan=newrchan;
							}
						}
					}
					break;
			}
			break;

		case WM_HSCROLL:
			if (l) { // set input source level
				float level=SendMessage((HWND)l,TBM_GETPOS,0,0)/100.f;
				if (!BASS_RecordSetInput(input,0,level)) // failed to set input level
					BASS_RecordSetInput(-1,0,level); // try master level instead
			}
			break;

		case WM_INITDIALOG:
			win=h;
			MESS(14,TBM_SETRANGE,FALSE,MAKELONG(0,100));
			{ // get list of recording devices
				int c,def;
				BASS_DEVICEINFO di;
				for (c=0;BASS_RecordGetDeviceInfo(c,&di);c++) {
					MESS(16,CB_ADDSTRING,0,di.name);
					if (di.flags&BASS_DEVICE_DEFAULT) { // got the default device
						MESS(16,CB_SETCURSEL,c,0);
						def=c;
					}
				}
				InitDevice(def); // initialize default recording device
			}
			// initialize default output device
			if (!BASS_Init(-1,FREQ,0,win,NULL))
				Error("Can't initialize output device");
			SetTimer(h,0,200,0); // timer to update the position display
			return 1;

		case WM_DESTROY:
			// release all BASS stuff
			BASS_RecordFree();
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

	{ // enable trackbar support (for the level control)
		INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_BAR_CLASSES};
		InitCommonControlsEx(&cc);
	}

	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
