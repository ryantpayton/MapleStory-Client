/*
	BASS custom looping example
	Copyright (c) 2004-2014 Un4seen Developments Ltd.
*/

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "bass.h"

#define WIDTH 600	// display width
#define HEIGHT 201	// height (odd number for centre line)

HWND win=NULL;
DWORD scanthread=0;
BOOL killscan=FALSE;

DWORD chan;
DWORD bpp;			// bytes per pixel
QWORD loop[2]={0};	// loop start & end
HSYNC lsync;		// looping sync

HDC wavedc=0;
HBITMAP wavebmp=0;
BYTE *wavebuf;

// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}

void CALLBACK LoopSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	if (!BASS_ChannelSetPosition(channel,loop[0],BASS_POS_BYTE)) // try seeking to loop start
		BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE); // failed, go to start of file instead
}

void SetLoopStart(QWORD pos)
{
	loop[0]=pos;
}

void SetLoopEnd(QWORD pos)
{
	loop[1]=pos;
	BASS_ChannelRemoveSync(chan,lsync); // remove old sync
	lsync=BASS_ChannelSetSync(chan,BASS_SYNC_POS|BASS_SYNC_MIXTIME,loop[1],LoopSyncProc,0); // set new sync
}

// scan the peaks
void __cdecl ScanPeaks(void *p)
{
	DWORD decoder=(DWORD)p;
	DWORD pos=0;
	float spp=BASS_ChannelBytes2Seconds(decoder,bpp); // seconds per pixel
	while (!killscan) {
		float peak[2];
		if (spp>1) { // more than 1 second per pixel, break it down...
			float todo=spp;
			peak[1]=peak[0]=0;
			do {
				float level[2],step=(todo<1?todo:1);
				BASS_ChannelGetLevelEx(decoder,level,step,BASS_LEVEL_STEREO); // scan peaks
				if (peak[0]<level[0]) peak[0]=level[0];
				if (peak[1]<level[1]) peak[1]=level[1];
				todo-=step;
			} while (todo>0);
		} else
			BASS_ChannelGetLevelEx(decoder,peak,spp,BASS_LEVEL_STEREO); // scan peaks
		{
			DWORD a;
			for (a=0;a<peak[0]*(HEIGHT/2);a++)
				wavebuf[(HEIGHT/2-1-a)*WIDTH+pos]=1+a; // draw left peak
			for (a=0;a<peak[1]*(HEIGHT/2);a++)
				wavebuf[(HEIGHT/2+1+a)*WIDTH+pos]=1+a; // draw right peak
		}
		pos++;
		if (pos>=WIDTH) break; // reached end of display
		if (!BASS_ChannelIsActive(decoder)) break; // reached end of channel
	}
	if (!killscan) {
		DWORD size;
		BASS_ChannelSetPosition(decoder,(QWORD)-1,BASS_POS_BYTE|BASS_POS_SCAN); // build seek table (scan to end)
		size=BASS_ChannelGetAttributeEx(decoder,BASS_ATTRIB_SCANINFO,0,0); // get seek table size
		if (size) { // got it
			void *info=malloc(size); // allocate a buffer
			BASS_ChannelGetAttributeEx(decoder,BASS_ATTRIB_SCANINFO,info,size); // get the seek table
			BASS_ChannelSetAttributeEx(chan,BASS_ATTRIB_SCANINFO,info,size); // apply it to the playback channel
			free(info);
		}
	}
	BASS_StreamFree(decoder); // free the decoder
	scanthread=0;
}

// select a file to play, and start scanning it
BOOL PlayFile()
{
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrTitle="Select a file to play";
	ofn.lpstrFilter="Playable files\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif;*.mo3;*.it;*.xm;*.s3m;*.mtm;*.mod;*.umx\0All files\0*.*\0\0";
	if (!GetOpenFileName(&ofn)) return FALSE;

	if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,0))
		&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMPS|BASS_MUSIC_POSRESET|BASS_MUSIC_PRESCAN,1))) {
		Error("Can't play file");
		return FALSE; // Can't load the file
	}
	{
		BYTE data[2000]={0};
		BITMAPINFOHEADER *bh=(BITMAPINFOHEADER*)data;
		RGBQUAD *pal=(RGBQUAD*)(data+sizeof(*bh));
		int a;
		bh->biSize=sizeof(*bh);
		bh->biWidth=WIDTH;
		bh->biHeight=-HEIGHT;
		bh->biPlanes=1;
		bh->biBitCount=8;
		bh->biClrUsed=bh->biClrImportant=HEIGHT/2+1;
		// setup palette
		for (a=1;a<=HEIGHT/2;a++) {
			pal[a].rgbRed=(255*a)/(HEIGHT/2);
			pal[a].rgbGreen=255-pal[a].rgbRed;
		}
		// create the bitmap
		wavebmp=CreateDIBSection(0,(BITMAPINFO*)bh,DIB_RGB_COLORS,(void**)&wavebuf,NULL,0);
		wavedc=CreateCompatibleDC(0);
		SelectObject(wavedc,wavebmp);
	}
	bpp=BASS_ChannelGetLength(chan,BASS_POS_BYTE)/WIDTH; // bytes per pixel
	{
		DWORD bpp1=BASS_ChannelSeconds2Bytes(chan,0.001); // minimum 1ms per pixel
		if (bpp<bpp1) bpp=bpp1;
	}
	BASS_ChannelSetSync(chan,BASS_SYNC_END|BASS_SYNC_MIXTIME,0,LoopSyncProc,0); // set sync to loop at end
	BASS_ChannelPlay(chan,FALSE); // start playing
	{ // create another channel to scan
		DWORD chan2=BASS_StreamCreateFile(FALSE,file,0,0,BASS_STREAM_DECODE);
		if (!chan2) chan2=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_DECODE,1);
		scanthread=_beginthread(ScanPeaks,0,(void*)chan2); // start scanning in a new thread
	}
	return TRUE;
}

void DrawTimeLine(HDC dc, QWORD pos, DWORD col, DWORD y)
{
	HPEN pen=CreatePen(PS_SOLID,0,col),oldpen;
	DWORD wpos=pos/bpp;
	DWORD time=BASS_ChannelBytes2Seconds(chan,pos)*1000; // position in milliseconds
	char text[16];
	sprintf(text,"%u:%02u.%03u",time/60000,(time/1000)%60,time%1000);
	oldpen=(HPEN)SelectObject(dc,pen);
	MoveToEx(dc,wpos,0,NULL);
	LineTo(dc,wpos,HEIGHT);
	SetTextColor(dc,col);
	SetBkMode(dc,TRANSPARENT);
	SetTextAlign(dc,wpos>=WIDTH/2?TA_RIGHT:TA_LEFT);
	TextOut(dc,wpos,y,text,strlen(text));
	SelectObject(dc,oldpen);
	DeleteObject(pen);
}

// window procedure
LRESULT CALLBACK SpectrumWindowProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
			if (w&MK_LBUTTON) SetLoopStart(LOWORD(l)*bpp); // set loop start
			if (w&MK_RBUTTON) SetLoopEnd(LOWORD(l)*bpp); // set loop end
			return 0;

		case WM_MBUTTONDOWN:
			BASS_ChannelSetPosition(chan,LOWORD(l)*bpp,BASS_POS_BYTE); // set current pos
			return 0;

		case WM_TIMER:
			InvalidateRect(h,0,0); // refresh window
			return 0;

		case WM_PAINT:
			if (GetUpdateRect(h,0,0)) {
				PAINTSTRUCT p;
				HDC dc;
				if (!(dc=BeginPaint(h,&p))) return 0;
				BitBlt(dc,0,0,WIDTH,HEIGHT,wavedc,0,0,SRCCOPY); // draw peak waveform
				DrawTimeLine(dc,loop[0],0xffff00,12); // loop start
				DrawTimeLine(dc,loop[1],0x00ffff,24); // loop end
				DrawTimeLine(dc,BASS_ChannelGetPosition(chan,BASS_POS_BYTE),0xffffff,0); // current pos
				EndPaint(h,&p);
			}
			return 0;

		case WM_CREATE:
			win=h;
			// initialize output
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				return -1;
			}
			if (!PlayFile()) { // start a file playing
				BASS_Free();
				return -1;
			}
			SetTimer(h,0,100,0); // set update timer (10hz)
			break;

		case WM_DESTROY:
			KillTimer(h,0);
			if (scanthread) { // still scanning
				killscan=TRUE;
				WaitForSingleObject((HANDLE)scanthread,1000); // wait for the thread
			}
			BASS_Free();
			if (wavedc) DeleteDC(wavedc);
			if (wavebmp) DeleteObject(wavebmp);
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}

	// register window class and create the window
	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc = SpectrumWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "BASS-CustLoop";
	if (!RegisterClass(&wc) || !CreateWindow("BASS-CustLoop",
			"BASS custom looping example (left-click to set loop start, right-click to set end)",
			WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE, 100, 100,
			WIDTH+2*GetSystemMetrics(SM_CXDLGFRAME),
			HEIGHT+GetSystemMetrics(SM_CYCAPTION)+2*GetSystemMetrics(SM_CYDLGFRAME),
			NULL, NULL, hInstance, NULL)) {
		Error("Can't create window");
		return 0;
	}
	ShowWindow(win, SW_SHOWNORMAL);

	while (GetMessage(&msg,NULL,0,0)>0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
