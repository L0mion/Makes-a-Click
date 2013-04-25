#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <Windows.h>
#include <WindowsX.h>

#include <XAudio2.h>
#include <XAudio2fx.h>

#include <vector>

using namespace std;

#include "Utility.h"


#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


static const int SOUND_BACKGROUND_MUSIC			= 0;
static const int SOUND_LASER_SWORD_HUM			= 1;
static const int SOUND_TENSION_PIANO_DOOM_LOW	= 2;
static const int SOUND_ZOMBIE_EATING			= 3;

class SoundManager
{
private:
	IXAudio2 *xAudio2;
	IXAudio2MasteringVoice *masterVoice;

	vector<IXAudio2SourceVoice*> sourceVoices;
	vector<XAUDIO2_BUFFER> buffers;

	WAVEFORMATEXTENSIBLE wfx;

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	BYTE* pDataBuffer;

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
	HRESULT loadFile(TCHAR* strFileName);

	void createSound(TCHAR* strFileName, UINT LoopCount, UINT LoopBegin);

	void createXAudio2();
	void createMasterVoice();

public:
	SoundManager();
	~SoundManager();

	void initialize();

	void startVoice(int voiceIndex);
	void stopVoice(int voiceIndex);
	void setMasterVolume(float volume);
	void setSourceVoiceVolume(int voiceIndex, float volume);
	void setSourceVoiceVolumeDistance(int voiceIndex, float distance);
	void setSourceVoicePanning(int voiceIndex, float angle, bool fromRight);
};

#endif