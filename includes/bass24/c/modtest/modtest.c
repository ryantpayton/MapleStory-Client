/*
	BASS MOD music test
	Copyright (c) 1999-2014 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include "bass.h"

HWND win=NULL;

DWORD music;	// the HMUSIC channel

OPENFILENAME ofn;

// display error messages
void Error(char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,"Error",0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)

DWORD GetFlags()
{
	DWORD flags=BASS_MUSIC_POSRESET; // stop notes when seeking
	switch (MESS(21,CB_GETCURSEL,0,0)) {
		case 0:
			flags|=BASS_MUSIC_NONINTER; // no interpolation
			break;
		case 2:
			flags|=BASS_MUSIC_SINCINTER; // sinc interpolation
			break;
	}
	switch (MESS(22,CB_GETCURSEL,0,0)) {
		case 1:
			flags|=BASS_MUSIC_RAMP; // ramping
			break;
		case 2:
			flags|=BASS_MUSIC_RAMPS; // "sensitive" ramping
			break;
	}
	switch (MESS(23,CB_GETCURSEL,0,0)) {
		case 1:
			flags|=BASS_MUSIC_SURROUND; // surround
			break;
		case 2:
			flags|=BASS_MUSIC_SURROUND2; // "mode2"
			break;
	}
	return flags;
}

INT_PTR CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_TIMER:
			{ // update display
				char text[16];
				QWORD pos=BASS_ChannelGetPosition(music,BASS_POS_MUSIC_ORDER);
				if (pos!=(QWORD)-1) {
					MESS(20,TBM_SETPOS,1,LOWORD(pos));
					sprintf(text,"%03d.%03d",LOWORD(pos),HIWORD(pos));
					MESS(15,WM_SETTEXT,0,text);
				}
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					break;
				case 10:
					{
						char file[MAX_PATH]="";
						ofn.lpstrFilter="mo3/it/xm/s3m/mtm/mod/umx files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							BASS_MusicFree(music); // free the current music
							music=BASS_MusicLoad(FALSE,file,0,0,GetFlags(),1); // load the new music
							if (music) { // success
								DWORD length=BASS_ChannelGetLength(music,BASS_POS_MUSIC_ORDER); // get the order length
								MESS(10,WM_SETTEXT,0,file);
								{
									char text[100],*ctype="";
									BASS_CHANNELINFO info;
									int channels=0;
									while (BASS_ChannelGetAttributeEx(music,BASS_ATTRIB_MUSIC_VOL_CHAN+channels,0,0)) channels++; // count channels
									BASS_ChannelGetInfo(music,&info);
									switch (info.ctype&~BASS_CTYPE_MUSIC_MO3) {
										case BASS_CTYPE_MUSIC_MOD:
											ctype="MOD";
											break;
										case BASS_CTYPE_MUSIC_MTM:
											ctype="MTM";
											break;
										case BASS_CTYPE_MUSIC_S3M:
											ctype="S3M";
											break;
										case BASS_CTYPE_MUSIC_XM:
											ctype="XM";
											break;
										case BASS_CTYPE_MUSIC_IT:
											ctype="IT";
											break;
									}
									_snprintf(text,sizeof(text),"name: %s, format: %dch %s%s",BASS_ChannelGetTags(music,BASS_TAG_MUSIC_NAME),channels,ctype,info.ctype&BASS_CTYPE_MUSIC_MO3?" (MO3)":"");
									MESS(11,WM_SETTEXT,0,text);
								}
								MESS(20,TBM_SETRANGEMAX,1,length-1); // update scroller range
								BASS_ChannelPlay(music,FALSE); // start it
							} else { // failed
								MESS(10,WM_SETTEXT,0,"click here to open a file...");
								MESS(11,WM_SETTEXT,0,"");
								MESS(15,WM_SETTEXT,0,"");
								Error("Can't play the file");
							}
						}
					}
					break;
				case 12:
					if (BASS_ChannelIsActive(music)==BASS_ACTIVE_PLAYING)
						BASS_ChannelPause(music);
					else
						BASS_ChannelPlay(music,FALSE);
					break;
				case 21:
				case 22:
				case 23:
					BASS_ChannelFlags(music,GetFlags(),-1); // update flags
					break;
			}
			break;

		case WM_HSCROLL:
			if (l && LOWORD(w)!=SB_THUMBPOSITION && LOWORD(w)!=SB_ENDSCROLL) {
				int pos=SendMessage((HWND)l,TBM_GETPOS,0,0);
				BASS_ChannelSetPosition(music,pos,BASS_POS_MUSIC_ORDER); // set the position
			}
			break;

		case WM_INITDIALOG:
			win=h;
			// initialize default output device
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				DestroyWindow(win);
				break;
			}
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.nMaxFile=MAX_PATH;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			MESS(21,CB_ADDSTRING,0,"off");
			MESS(21,CB_ADDSTRING,0,"linear");
			MESS(21,CB_ADDSTRING,0,"sinc");
			MESS(21,CB_SETCURSEL,1,0);
			MESS(22,CB_ADDSTRING,0,"off");
			MESS(22,CB_ADDSTRING,0,"normal");
			MESS(22,CB_ADDSTRING,0,"sensitive");
			MESS(22,CB_SETCURSEL,2,0);
			MESS(23,CB_ADDSTRING,0,"off");
			MESS(23,CB_ADDSTRING,0,"mode1");
			MESS(23,CB_ADDSTRING,0,"mode2");
			MESS(23,CB_SETCURSEL,0,0);
			SetTimer(win,1,100,NULL);
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

	DialogBox(hInstance,(char*)1000,0,dialogproc);

	return 0;
}
