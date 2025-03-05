#define MAX_ERRMSG 256

#include "SndTools.h"

#define WF_OFFSET_FORMATTAG			20
#define WF_OFFSET_CHANNELS			22
#define WF_OFFSET_SAMPLESPERSEC		24
#define WF_OFFSET_AVGBYTESPERSEC	28
#define WF_OFFSET_BLOCKALIGN		32
#define WF_OFFSET_BITSPERSAMPLE		34
#define WF_OFFSET_DATASIZE			40
#define WF_OFFSET_DATA				44
#define WF_HEADER_SIZE WF_OFFSET_DATA

long m_nLength;
WAVEFORMATEX m_Format;


//example code:
//
//{	//TCHAR szWavFile[MAX_PATH];
//	TCHAR szErrMsg[MAX_ERRMSG];
//	MMRESULT mmres;
//	
//	mmres = wave_play();
//	if (mmres != MMSYSERR_NOERROR) 
//	{
//		waveOutGetErrorText(mmres, (LPTSTR) szErrMsg, 256);
//		MessageBox(NULL, szErrMsg, TEXT("WaveOut Error"), MB_OK);
//	}


MMRESULT wave_play(char *sd, int len, int volume, int repeatCount)// play sound data in .wav format.
{	HWAVEOUT hwo;
	WAVEHDR whdr;
	MMRESULT mmres;
	HANDLE hDoneEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("DONE_EVENT"));
	UINT devId;
	DWORD dwOldVolume;

	wave_prepare(len);

	// Open audio device
	for (devId = 0; devId < waveOutGetNumDevs(); devId++) 
	{	mmres = waveOutOpen(&hwo, devId, getwaveformat(), 
							(DWORD) hDoneEvent, 0, CALLBACK_EVENT);
		if (mmres == MMSYSERR_NOERROR) 
			break;
	}
	if (mmres != MMSYSERR_NOERROR) 
	    return mmres;

	// Set volume
	mmres = waveOutGetVolume(hwo, &dwOldVolume);
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	if (volume == 0)
		waveOutSetVolume(hwo, 0x4444);
	else
	if (volume == 1)
		waveOutSetVolume(hwo, 0x8888);
	else
	if (volume == 2)
		waveOutSetVolume(hwo, 0xFFFFFFFF);	// set maximum volume.

	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	if (repeatCount > 100)
		repeatCount = 100;
	if (repeatCount < 0)
		repeatCount = 0;

	// Initialize wave header
	ZeroMemory(&whdr, sizeof(WAVEHDR));
	whdr.dwBufferLength = len;
	whdr.dwUser = 0;
	whdr.dwFlags = WHDR_BEGINLOOP;
	whdr.dwLoops = repeatCount;
	whdr.dwBytesRecorded = 0;
	whdr.lpNext = 0;
	whdr.reserved = 0;

	// Play buffer
	whdr.lpData = sd;

	mmres = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));	
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	mmres = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));	
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	// Wait for audio to finish playing
	while (!(whdr.dwFlags & WHDR_DONE)) 
		WaitForSingleObject(hDoneEvent, INFINITE);
		
	// Clean up
	mmres = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));	
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	waveOutSetVolume(hwo, dwOldVolume);
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	mmres = waveOutClose(hwo);
	if (mmres != MMSYSERR_NOERROR) 
		return mmres;

	return MMSYSERR_NOERROR;
}


void wave_prepare(int len)
{
	LockResource (0);
	m_nLength = len;
	m_Format.wFormatTag = 1;
	m_Format.nChannels = 2;
	m_Format.nSamplesPerSec = 8000;   //22050;
	m_Format.nAvgBytesPerSec = 16000; //44100;
	m_Format.nBlockAlign = 2;
	m_Format.wBitsPerSample = 8;
}



WAVEFORMATEX* getwaveformat()
{	return (&m_Format);
}

