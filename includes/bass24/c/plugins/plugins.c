/*
	BASS plugin test
	Copyright (c) 2005-2011 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include "bass.h"

HWND win=NULL;

DWORD chan;	// the channel

OPENFILENAME ofn;
char filter[2000];

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

// translate a CTYPE value to text
const char *GetCTypeString(DWORD ctype, HPLUGIN plugin)
{
	if (plugin) { // using a plugin
		const BASS_PLUGININFO *pinfo=BASS_PluginGetInfo(plugin); // get plugin info
		int a;
		for (a=0;a<pinfo->formatc;a++) {
			if (pinfo->formats[a].ctype==ctype) // found a "ctype" match...
				return pinfo->formats[a].name; // return its name
		}
	}
	// check built-in stream formats...
	if (ctype==BASS_CTYPE_STREAM_OGG) return "Ogg Vorbis";
	if (ctype==BASS_CTYPE_STREAM_MP1) return "MPEG layer 1";
	if (ctype==BASS_CTYPE_STREAM_MP2) return "MPEG layer 2";
	if (ctype==BASS_CTYPE_STREAM_MP3) return "MPEG layer 3";
	if (ctype==BASS_CTYPE_STREAM_AIFF) return "Audio IFF";
	if (ctype==BASS_CTYPE_STREAM_WAV_PCM) return "PCM WAVE";
	if (ctype==BASS_CTYPE_STREAM_WAV_FLOAT) return "Floating-point WAVE";
	if (ctype==BASS_CTYPE_STREAM_MF) { // a Media Foundation codec, check the format...
		const WAVEFORMATEX *wf=(WAVEFORMATEX*)BASS_ChannelGetTags(chan,BASS_TAG_WAVEFORMAT);
		if (wf->wFormatTag==0x1610) return "Advanced Audio Coding";
		if (wf->wFormatTag>=0x0160 && wf->wFormatTag<=0x0163) return "Windows Media Audio";
	}
	if (ctype&BASS_CTYPE_STREAM_WAV) // other WAVE codec, could use acmFormatTagDetails to get its name, but for now...
		return "WAVE";
	return "?";
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

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
						ofn.lpstrFile=file;
						ofn.nMaxFile=MAX_PATH;
						if (GetOpenFileName(&ofn)) {
							BASS_StreamFree(chan); // free the old stream
							if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))) {
								// it ain't playable
								MESS(10,WM_SETTEXT,0,"click here to open a file...");
								MESS(11,WM_SETTEXT,0,"");
								Error("Can't play the file");
								break;
							}
							MESS(10,WM_SETTEXT,0,file);
							{ // display the file type and length
								QWORD bytes=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
								DWORD time=BASS_ChannelBytes2Seconds(chan,bytes);
								BASS_CHANNELINFO info;
								BASS_ChannelGetInfo(chan,&info);
								sprintf(file,"channel type = %x (%s)\nlength = %I64u (%u:%02u)",
									info.ctype,GetCTypeString(info.ctype,info.plugin),bytes,time/60,time%60);
								MESS(11,WM_SETTEXT,0,file);
								MESS(12,TBM_SETRANGEMAX,1,time); // update scroller range
							}
							BASS_ChannelPlay(chan,FALSE);
						}
					}
					break;
			}
			break;

		case WM_HSCROLL:
			if (l && LOWORD(w)!=SB_THUMBPOSITION && LOWORD(w)!=SB_ENDSCROLL) { // set the position
				int pos=SendMessage((HWND)l,TBM_GETPOS,0,0);
				BASS_ChannelSetPosition(chan,BASS_ChannelSeconds2Bytes(chan,pos),BASS_POS_BYTE);
			}
			break;

		case WM_TIMER:
			MESS(12,TBM_SETPOS,1,(DWORD)BASS_ChannelBytes2Seconds(chan,BASS_ChannelGetPosition(chan,BASS_POS_BYTE))); // update position
			break;

		case WM_INITDIALOG:
			win=h;
			// initialize default output device
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				DestroyWindow(win);
				break;
			}
			// initialize file selector
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			ofn.lpstrFilter=filter;
			memcpy(filter,"BASS built-in formats (*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif)\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0",96);
			{ // look for plugins (in the executable's directory)
				WIN32_FIND_DATA fd;
				HANDLE fh;
				char path[MAX_PATH],*fp=filter+96;
				GetModuleFileName(0,path,sizeof(path));
				strcpy(strrchr(path,'\\')+1,"bass*.dll");
				fh=FindFirstFile(path,&fd);
				if (fh!=INVALID_HANDLE_VALUE) {
					do {
						HPLUGIN plug;
						if (plug=BASS_PluginLoad(fd.cFileName,0)) { // plugin loaded...
							const BASS_PLUGININFO *pinfo=BASS_PluginGetInfo(plug); // get plugin info to add to the file selector filter...
							int a;
							for (a=0;a<pinfo->formatc;a++) {
								fp+=sprintf(fp,"%s (%s) - %s",pinfo->formats[a].name,pinfo->formats[a].exts,fd.cFileName)+1; // format description
								fp+=sprintf(fp,"%s",pinfo->formats[a].exts)+1; // extension filter
							}
							// add plugin to the list
							MESS(20,LB_ADDSTRING,0,fd.cFileName);
						}
					} while (FindNextFile(fh,&fd));
					FindClose(fh);
				}
				if (!MESS(20,LB_GETCOUNT,0,0)) // no plugins...
					MESS(20,LB_ADDSTRING,0,"no plugins - visit the BASS webpage to get some");
				// check if Media Foundation is available
				if (!BASS_GetConfig(BASS_CONFIG_MF_DISABLE)) {
					fp+=sprintf(fp,"Media Foundation formats (*.aac;*.m4a;*.mp4;*.wma)")+1;
					fp+=sprintf(fp,"*.aac;*.m4a;*.mp4;*.wma")+1;
				}
				memcpy(fp,"All files\0*.*\0\0",15);
			}
			SetTimer(h,0,500,0); // timer to update the position
			return 1;

		case WM_DESTROY:
			// "free" the output device and all plugins
			BASS_Free();
			BASS_PluginFree(0);
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
