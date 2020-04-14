/*
	BASS DX8 effects test
	Copyright (c) 2001-2017 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include "bass.h"

HWND win=NULL;

DWORD chan;			// channel handle
DWORD fxchan=0;		// output stream handle
DWORD fxchansync;	// output stream FREE sync
HFX fx[4];			// 3 eq bands + reverb

OPENFILENAME ofn;

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

void UpdateFX(int b)
{
	int v=MESS(20+b,TBM_GETPOS,0,0);
	if (b<3) {
		BASS_DX8_PARAMEQ p;
		BASS_FXGetParameters(fx[b],&p);
		p.fGain=10.0-v;
		BASS_FXSetParameters(fx[b],&p);
	} else {
		BASS_DX8_REVERB p;
		BASS_FXGetParameters(fx[3],&p);
		p.fReverbMix=(v<20?log(1-v/20.0)*20:-96);
		BASS_FXSetParameters(fx[3],&p);
	}
}

void SetupFX()
{
	// setup the effects
	BASS_DX8_PARAMEQ p;
	DWORD ch=fxchan?fxchan:chan; // set on output stream if enabled, else file stream
	fx[0]=BASS_ChannelSetFX(ch,BASS_FX_DX8_PARAMEQ,0);
	fx[1]=BASS_ChannelSetFX(ch,BASS_FX_DX8_PARAMEQ,0);
	fx[2]=BASS_ChannelSetFX(ch,BASS_FX_DX8_PARAMEQ,0);
	fx[3]=BASS_ChannelSetFX(ch,BASS_FX_DX8_REVERB,0);
	p.fGain=0;
	p.fBandwidth=18;
	p.fCenter=125;
	BASS_FXSetParameters(fx[0],&p);
	p.fCenter=1000;
	BASS_FXSetParameters(fx[1],&p);
	p.fCenter=8000;
	BASS_FXSetParameters(fx[2],&p);
	UpdateFX(0);
	UpdateFX(1);
	UpdateFX(2);
	UpdateFX(3);
}

void CALLBACK DeviceFreeSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	// the device output stream has been freed due to format change, get a new one with new format
	if (!fxchan) return;
	fxchan=BASS_StreamCreate(0,0,0,STREAMPROC_DEVICE,0);
	fxchansync=BASS_ChannelSetSync(fxchan,BASS_SYNC_FREE,0,DeviceFreeSync,0);
	SetupFX();
}

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10:
					{
						char file[MAX_PATH]="";
						ofn.lpstrFilter="playable files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							// free both MOD and stream, it must be one of them! :)
							BASS_MusicFree(chan);
							BASS_StreamFree(chan);
							if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP|BASS_SAMPLE_FLOAT))
								&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMP|BASS_SAMPLE_FLOAT,1))) {
								// whatever it is, it ain't playable
								MESS(10,WM_SETTEXT,0,"click here to open a file...");
								Error("Can't play the file");
								break;
							}
							MESS(10,WM_SETTEXT,0,file);
							if (!fxchan) SetupFX(); // set effects on file if not using output stream
							BASS_ChannelPlay(chan,FALSE);
						}
					}
					break;
				case 30:
					{
						// remove current effects
						DWORD ch=fxchan?fxchan:chan;
						BASS_ChannelRemoveFX(ch,fx[0]);
						BASS_ChannelRemoveFX(ch,fx[1]);
						BASS_ChannelRemoveFX(ch,fx[2]);
						BASS_ChannelRemoveFX(ch,fx[3]);
						if (MESS(30,BM_GETCHECK,0,0)) {
							fxchan=BASS_StreamCreate(0,0,0,STREAMPROC_DEVICE,0); // get device output stream
							fxchansync=BASS_ChannelSetSync(fxchan,BASS_SYNC_FREE,0,DeviceFreeSync,0); // sync when device output stream is freed (format change)
						} else {
							BASS_ChannelRemoveSync(fxchan,fxchansync); // remove sync from device output stream
							fxchan=0; // stop using device output stream
						}
						SetupFX();
					}
					break;
			}
			break;

		case WM_VSCROLL:
			if (l) {
				UpdateFX(GetDlgCtrlID((HWND)l)-20);
			}
			break;

		case WM_INITDIALOG:
			win=h;
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			// initialize default device
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				EndDialog(win,0);
				return 0;
			}
			{
				// check that DX8 features are available
				BASS_INFO bi={sizeof(bi)};
				BASS_GetInfo(&bi);
				if (bi.dsver<8) {
					BASS_Free();
					Error("DirectX 8 is not installed");
					EndDialog(win,0);
					return 0;
				}
				// disable output stream option if using DirectSound output
				if (bi.initflags&BASS_DEVICE_DSOUND)
					EnableWindow(GetDlgItem(win,30),FALSE);
			}
			// initialize eq/reverb sliders
			MESS(20,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(20,TBM_SETPOS,TRUE,10);
			MESS(21,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(21,TBM_SETPOS,TRUE,10);
			MESS(22,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(22,TBM_SETPOS,TRUE,10);
			MESS(23,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(23,TBM_SETPOS,TRUE,20);
			return 1;

		case WM_DESTROY:
			if (fxchan) BASS_ChannelRemoveSync(fxchan,fxchansync); // remove sync from device output stream
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

	{ // enable trackbar support
		INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_BAR_CLASSES};
		InitCommonControlsEx(&cc);
	}

	DialogBox(hInstance,(char*)1000,0,&dialogproc);

	return 0;
}
