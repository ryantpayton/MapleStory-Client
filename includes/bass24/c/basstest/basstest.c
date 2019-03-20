/*
	BASS simple playback test
	Copyright (c) 1999-2019 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "bass.h"

HWND win=NULL;

HSTREAM *strs=NULL;
int strc=0;
HMUSIC *mods=NULL;
int modc=0;
HSAMPLE *sams=NULL;
int samc=0;

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// messaging macros
#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))
#define STLM(m,w,l) MESS(10,m,w,l)
#define MLM(m,w,l) MESS(20,m,w,l)
#define SLM(m,w,l) MESS(30,m,w,l)
#define GETSTR() STLM(LB_GETCURSEL,0,0)
#define GETMOD() MLM(LB_GETCURSEL,0,0)
#define GETSAM() SLM(LB_GETCURSEL,0,0)

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;

	switch (m) {
		case WM_TIMER:
			{
				// update the CPU usage display
				char text[10];
				sprintf(text,"%.2f",BASS_GetCPU());
				MESS(40,WM_SETTEXT,0,text);
				// update volume slider in case it's been changed outside of the app
				MESS(43,TBM_SETPOS,1,BASS_GetVolume()*100);
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;

				case 14:
					{
						char file[MAX_PATH]="";
						HSTREAM str;
						ofn.lpstrFilter="Streamable files (wav/aif/mp3/mp2/mp1/ogg)\0*.wav;*.aif;*.mp3;*.mp2;*.mp1;*.ogg\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							if (str=BASS_StreamCreateFile(FALSE,file,0,0,0)) {
								strc++;
								strs=(HSTREAM*)realloc((void*)strs,strc*sizeof(*strs));
								strs[strc-1]=str;
								STLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't open stream");
						}
					}
					break;
				case 15:
					{
						int s=GETSTR();
						if (s!=LB_ERR) {
							STLM(LB_DELETESTRING,s,0);
							BASS_StreamFree(strs[s]); // free the stream
							strc--;
							memcpy(strs+s,strs+s+1,(strc-s)*sizeof(*strs));
						}
					}
					break;
				case 11:
					{
						int s=GETSTR();
						if (s!=LB_ERR)
							if (!BASS_ChannelPlay(strs[s],FALSE)) // play the stream (continue from current position)
								Error("Can't play stream");
					}
					break;
				case 12:
					{
						int s=GETSTR();
						if (s!=LB_ERR) BASS_ChannelStop(strs[s]); // stop the stream
					}
					break;
				case 13:
					{
						int s=GETSTR();
						if (s!=LB_ERR) BASS_ChannelPlay(strs[s],TRUE); // play the stream from the start
					}
					break;

				case 24:
					{
						char file[MAX_PATH]="";
						HMUSIC mod;
						ofn.lpstrFilter="MOD music files (mo3/xm/mod/s3m/it/mtm/umx)\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							// load a music from "file" with ramping enabled
							if (mod=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMPS,1)) {
								modc++;
								mods=(HMUSIC*)realloc((void*)mods,modc*sizeof(*mods));
								mods[modc-1]=mod;
								MLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't load music");
						}
					}
					break;
				case 25:
					{
						int s=GETMOD();
						if (s!=LB_ERR) {
							MLM(LB_DELETESTRING,s,0);
							BASS_MusicFree(mods[s]); // free the music
							modc--;
							memcpy(mods+s,mods+s+1,(modc-s)*sizeof(*mods));
						}
					}
					break;
				case 21:
					{
						int s=GETMOD();
						if (s!=LB_ERR)
							if (!BASS_ChannelPlay(mods[s],FALSE)) // play the music (continue from current position)
								Error("Can't play music");
					}
					break;
				case 22:
					{
						int s=GETMOD();
						if (s!=LB_ERR) BASS_ChannelStop(mods[s]); // stop the music
					}
					break;
				case 23:
					{
						int s=GETMOD();
						if (s!=LB_ERR) BASS_ChannelPlay(mods[s],TRUE); // play the music from the start
					}
					break;

				case 32:
					{
						char file[MAX_PATH]="";
						HSAMPLE sam;
						ofn.lpstrFilter="Sample files (wav/aif)\0*.wav;*.aif\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							/* Load a sample from "file" and give it a max of 3 simultaneous
								playings using playback position as override decider */
							if (sam=BASS_SampleLoad(FALSE,file,0,0,3,BASS_SAMPLE_OVER_POS)) {
								samc++;
								sams=(HSAMPLE*)realloc((void*)sams,samc*sizeof(*sams));
								sams[samc-1]=sam;
								SLM(LB_ADDSTRING,0,strrchr(file,'\\')+1);
							} else
								Error("Can't load sample");
						}
					}
					break;
				case 33:
					{
						int s=GETSAM();
						if (s!=LB_ERR) {
							SLM(LB_DELETESTRING,s,0);
							BASS_SampleFree(sams[s]); // free the sample
							samc--;
							memcpy(sams+s,sams+s+1,(samc-s)*sizeof(*sams));
						}
					}
					break;
				case 31:
					{
						int s=GETSAM();
						if (s!=LB_ERR) {
							// play the sample (at default rate, volume=50%, random pan position)
							HCHANNEL ch=BASS_SampleGetChannel(sams[s],FALSE);
							BASS_ChannelSetAttribute(ch,BASS_ATTRIB_VOL,0.5f);
							BASS_ChannelSetAttribute(ch,BASS_ATTRIB_PAN,((rand()%201)-100)/100.f);
							if (!BASS_ChannelPlay(ch,FALSE))
								Error("Can't play sample");
						}
					}
					break;

				case 41:
					BASS_Pause(); // pause output
					break;
				case 42:
					BASS_Start(); // resume output
					break;
				case 44:
					BASS_SetConfig(BASS_CONFIG_UPDATETHREADS,MESS(44,BM_GETCHECK,0,0)?2:1); // set 1 or 2 update threads
					break;
			}
			break;

		case WM_HSCROLL:
			if (l && LOWORD(w)!=SB_THUMBPOSITION && LOWORD(w)!=SB_ENDSCROLL) {
				int p=SendMessage((HWND)l,TBM_GETPOS,0,0);
				switch (GetDlgCtrlID((HWND)l)) {
					case 16:
						BASS_SetConfig(BASS_CONFIG_GVOL_STREAM,p*100); // global stream volume (0-10000)
						break;
					case 26:
						BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC,p*100); // global MOD volume (0-10000)
						break;
					case 34:
						BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE,p*100); // global sample volume (0-10000)
						break;
					case 43:
						BASS_SetVolume(p/100.f); // output volume (0-1)
						break;
				}
			}
			break;

		case WM_INITDIALOG:
			win=h;
			// initialize default output device
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				EndDialog(win,0);
				return 0;
			}
			// initialize volume sliders
			MESS(16,TBM_SETRANGE,1,MAKELONG(0,100));
			MESS(16,TBM_SETPOS,1,100);
			MESS(26,TBM_SETRANGE,1,MAKELONG(0,100));
			MESS(26,TBM_SETPOS,1,100);
			MESS(34,TBM_SETRANGE,1,MAKELONG(0,100));
			MESS(34,TBM_SETPOS,1,100);
			MESS(43,TBM_SETRANGE,1,MAKELONG(0,100));
			MESS(43,TBM_SETPOS,1,BASS_GetVolume()*100);
			SetTimer(h,1,500,NULL);
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			return 1;

		case WM_DESTROY:
			KillTimer(h,1);
			BASS_Free(); // close output
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

	// enable "Default" device that follows default device changes
	BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT,1);

	// display the window
	DialogBox(hInstance,MAKEINTRESOURCE(1000),NULL,&dialogproc);

	return 0;
}
