/*
	BASS 3D test
	Copyright (c) 1999-2017 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include <stdio.h>
#include "bass.h"

HWND win=NULL;

// channel (sample/music) info structure
typedef struct {
	DWORD channel;			// the channel
	BASS_3DVECTOR pos,vel;	// position,velocity
} Channel;

Channel *chans=NULL;		// the channels
int chanc=0,chan=-1;		// number of channels, current channel

#define TIMERPERIOD	50		// timer period (ms)
#define MAXDIST		50		// maximum distance of the channels (m)
#define SPEED		12		// speed of the channels' movement (m/s)

// Display error dialogs
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// Messaging macros
#define ITEM(id) GetDlgItem(win,id)
#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))
#define LM(m,w,l) MESS(10,m,w,l)
#define EM(m,w,l) MESS(22,m,w,l)

void UpdateDisplay()
{
	HDC dc;
	RECT r;
	int c,x,y,cx,cy;
	HBRUSH red=CreateSolidBrush(0xff);

	HWND w=ITEM(30);
	dc=GetDC(w);
	GetClientRect(w,&r);
	cx=r.right/2;
	cy=r.bottom/2;

	// clear the display
	FillRect(dc,&r,(HBRUSH)GetStockObject(WHITE_BRUSH));

	// Draw the listener
	SelectObject(dc,GetStockObject(GRAY_BRUSH));
	Ellipse(dc,cx-4,cy-4,cx+4,cy+4);

	for (c=0;c<chanc;c++) {
		// If the channel's playing then update it's position
		if (BASS_ChannelIsActive(chans[c].channel)==BASS_ACTIVE_PLAYING) {
			// Check if channel has reached the max distance
			if (chans[c].pos.z>=MAXDIST || chans[c].pos.z<=-MAXDIST)
				chans[c].vel.z=-chans[c].vel.z;
			if (chans[c].pos.x>=MAXDIST || chans[c].pos.x<=-MAXDIST)
				chans[c].vel.x=-chans[c].vel.x;
			// Update channel position
			chans[c].pos.z+=chans[c].vel.z*TIMERPERIOD/1000;
			chans[c].pos.x+=chans[c].vel.x*TIMERPERIOD/1000;
			BASS_ChannelSet3DPosition(chans[c].channel,&chans[c].pos,NULL,&chans[c].vel);
		}
		// Draw the channel position indicator
		x=cx+(int)((cx-10)*chans[c].pos.x/MAXDIST);
		y=cy-(int)((cy-10)*chans[c].pos.z/MAXDIST);
		SelectObject(dc,chan==c?red:GetStockObject(WHITE_BRUSH));
		Ellipse(dc,x-4,y-4,x+4,y+4);
	}
	// Apply the 3D changes
	BASS_Apply3D();

	ReleaseDC(w,dc);
	DeleteObject(red);
}

// Update the button states
void UpdateButtons()
{
	int a;
	for (a=12;a<=17;a++)
		EnableWindow(ITEM(a),chan==-1?FALSE:TRUE);
	if (chan!=-1) {
		SetDlgItemInt(win,15,abs((int)chans[chan].vel.x),FALSE);
		SetDlgItemInt(win,16,abs((int)chans[chan].vel.z),FALSE);
	}
}

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;

	switch (m) {
		case WM_TIMER:
			UpdateDisplay();
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case 10: // change the selected channel
					if (HIWORD(w)!=LBN_SELCHANGE) break;
					chan=LM(LB_GETCURSEL,0,0);
					if (chan==LB_ERR) chan=-1;
					UpdateButtons();
					break;
				case 11: // add a channel
					{
						char file[MAX_PATH]="";
						DWORD newchan;
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							// Load a music or sample from "file"
							if ((newchan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP|BASS_SAMPLE_3D,1))
								|| (newchan=BASS_SampleLoad(FALSE,file,0,0,1,BASS_SAMPLE_LOOP|BASS_SAMPLE_3D|BASS_SAMPLE_MONO))) {
								Channel *c;
								chanc++;
								chans=(Channel*)realloc((void*)chans,chanc*sizeof(Channel));
								c=chans+chanc-1;
								memset(c,0,sizeof(Channel));
								c->channel=newchan;
								BASS_SampleGetChannel(newchan,FALSE); // initialize sample channel
								LM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't load file (note samples must be mono)");
						}
					}
					break;
				case 12: // remove a channel
					{
						Channel *c=chans+chan;
						BASS_SampleFree(c->channel);
						BASS_MusicFree(c->channel);
						memmove(c,c+1,(chanc-chan-1)*sizeof(Channel));
						chanc--;
						LM(LB_DELETESTRING,chan,0);
						chan=-1;
						UpdateButtons();
					}
					break;
				case 13:
					BASS_ChannelPlay(chans[chan].channel,FALSE);
					break;
				case 14:
					BASS_ChannelPause(chans[chan].channel);
					break;
				case 15: // X velocity
					if (HIWORD(w)==EN_CHANGE) {
						int v=GetDlgItemInt(win,15,0,FALSE);
						if (abs((int)chans[chan].vel.x)!=v) chans[chan].vel.x=v;
					}
					break;
				case 16: // Z velocity
					if (HIWORD(w)==EN_CHANGE) {
						int v=GetDlgItemInt(win,16,0,FALSE);
						if (abs((int)chans[chan].vel.z)!=v) chans[chan].vel.z=v;
					}
					break;
				case 17: // reset the position and velocity to 0
					memset(&chans[chan].pos,0,sizeof(chans[chan].pos));
					memset(&chans[chan].vel,0,sizeof(chans[chan].vel));
					UpdateButtons();
					break;
				case IDCANCEL:
					DestroyWindow(h);
					break;
			}
			break;

		case WM_HSCROLL:
			if (l) {
				int pos=SendMessage((HWND)l,TBM_GETPOS,0,0);
				switch (GetDlgCtrlID((HWND)l)) {
					case 20: // change the rolloff factor
						BASS_Set3DFactors(-1,pow(2,(pos-10)/5.0),-1);
						break;
					case 21: // change the doppler factor
						BASS_Set3DFactors(-1,-1,pow(2,(pos-10)/5.0));
						break;
				}
			}
			break;

		case WM_INITDIALOG:
			win=h;

			EM(CB_ADDSTRING,0,"Off");
			EM(CB_ADDSTRING,0,"Generic");
			EM(CB_ADDSTRING,0,"Padded Cell");
			EM(CB_ADDSTRING,0,"Room");
			EM(CB_ADDSTRING,0,"Bathroom");
			EM(CB_ADDSTRING,0,"Living Room");
			EM(CB_ADDSTRING,0,"Stone Room");
			EM(CB_ADDSTRING,0,"Auditorium");
			EM(CB_ADDSTRING,0,"Concert Hall");
			EM(CB_ADDSTRING,0,"Cave");
			EM(CB_ADDSTRING,0,"Arena");
			EM(CB_ADDSTRING,0,"Hangar");
			EM(CB_ADDSTRING,0,"Carpeted Hallway");
			EM(CB_ADDSTRING,0,"Hallway");
			EM(CB_ADDSTRING,0,"Stone Corridor");
			EM(CB_ADDSTRING,0,"Alley");
			EM(CB_ADDSTRING,0,"Forest");
			EM(CB_ADDSTRING,0,"City");
			EM(CB_ADDSTRING,0,"Mountains");
			EM(CB_ADDSTRING,0,"Quarry");
			EM(CB_ADDSTRING,0,"Plain");
			EM(CB_ADDSTRING,0,"Parking Lot");
			EM(CB_ADDSTRING,0,"Sewer Pipe");
			EM(CB_ADDSTRING,0,"Under Water");
			EM(CB_ADDSTRING,0,"Drugged");
			EM(CB_ADDSTRING,0,"Dizzy");
			EM(CB_ADDSTRING,0,"Psychotic");
			EM(CB_SETCURSEL,0,0);

			MESS(20,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(20,TBM_SETPOS,TRUE,10);
			MESS(21,TBM_SETRANGE,FALSE,MAKELONG(0,20));
			MESS(21,TBM_SETPOS,TRUE,10);

			SetTimer(h,1,TIMERPERIOD,NULL);
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			ofn.lpstrFilter="wav/aif/mo3/xm/mod/s3m/it/mtm/umx\0*.wav;*.aif;*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx\0"
				"All files\0*.*\0\0";
			return 1;

		case WM_DESTROY:
			KillTimer(h,1);
			if (chans) free(chans);
			PostQuitMessage(0);
			break;
	}
	return 0;
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	{ // enable trackbar support
		INITCOMMONCONTROLSEX cc={sizeof(cc),ICC_BAR_CLASSES};
		InitCommonControlsEx(&cc);
	}

	// Create the main window
	if (!CreateDialog(hInstance,MAKEINTRESOURCE(1000),NULL,dialogproc)) {
		Error("Can't create window");
		return 0;
	}

	// Initialize the default output device with 3D support
	if (!BASS_Init(-1,44100,BASS_DEVICE_3D,win,NULL)) {
		Error("Can't initialize output device");
		EndDialog(win,0);
		return 0;
	}

	// Use meters as distance unit, real world rolloff, real doppler effect
	BASS_Set3DFactors(1,1,1);

	while (GetMessage(&msg,NULL,0,0)>0) {
		if (!IsDialogMessage(win,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	BASS_Free();

	return 0;
}
