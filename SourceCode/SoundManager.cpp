#include "SoundManager.h"

SoundManager::SoundManager()
{
	this->xAudio2		= NULL;
	this->masterVoice	= NULL;
}

SoundManager::~SoundManager()
{
	xAudio2->Release();

	delete pDataBuffer;
}

HRESULT SoundManager::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	 HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
                hr = HRESULT_FROM_WIN32( GetLastError() );
            break;

        default:
            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
				return HRESULT_FROM_WIN32( GetLastError() );            
        }

        dwOffset += sizeof(DWORD) * 2;
        
        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    
    return S_OK;

}

HRESULT SoundManager::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
        return HRESULT_FROM_WIN32( GetLastError() );
    DWORD dwRead;
    if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );
    return hr;
}

HRESULT SoundManager::loadFile(TCHAR* strFileName)
{
	dwChunkSize = 0;
	dwChunkPosition = 0;
	pDataBuffer = NULL;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEXTENSIBLE));

	HANDLE hFile = CreateFile(strFileName,
							  GENERIC_READ,
							  FILE_SHARE_READ,
							  NULL,
							  OPEN_EXISTING,
							  0,
							  NULL);

	if( INVALID_HANDLE_VALUE == hFile )
		return HRESULT_FROM_WIN32( GetLastError() );

	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );

	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile,fourccRIFF,dwChunkSize, dwChunkPosition );
	DWORD filetype;
	ReadChunkData(hFile,&filetype,sizeof(DWORD),dwChunkPosition);
	if(filetype != fourccWAVE)
		return S_FALSE;


	//Find fmt chunk
	FindChunk( hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData( hFile, &wfx, dwChunkSize, dwChunkPosition);

	//Find data chunk
	FindChunk( hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData( hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	pDataBuffer;
	return S_OK;
}

void SoundManager::createSound(TCHAR* strFileName, UINT LoopCount, UINT LoopBegin)
{
	if(S_OK == loadFile(strFileName))
	{
		//Fill XAUDIO2_BUFFER
		XAUDIO2_BUFFER _buffer;
		ZeroMemory(&_buffer, sizeof(XAUDIO2_BUFFER));

		_buffer.LoopBegin = LoopBegin;
		_buffer.LoopCount = LoopCount;
		_buffer.AudioBytes = dwChunkSize; // Buffer containing audio data
		_buffer.pAudioData = pDataBuffer; //size of the audio buffer in bytes
		_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

		buffers.push_back(_buffer);

		IXAudio2SourceVoice* _sourceVoice;
		xAudio2->CreateSourceVoice(&_sourceVoice,
									(WAVEFORMATEX*)&wfx,
									0,
									XAUDIO2_DEFAULT_FREQ_RATIO,
									NULL,
									NULL,
									NULL);
		
		sourceVoices.push_back(_sourceVoice);
	}
}

void SoundManager::createXAudio2()
{
	HRESULT hr;
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"Could not create XAudio2 instance!", L"SoundManager error!", MB_OK | MB_ICONEXCLAMATION);
	}
}

void SoundManager::createMasterVoice()
{
	UINT32 deviceCount;
	xAudio2->GetDeviceCount(&deviceCount);

	XAUDIO2_DEVICE_DETAILS deviceDetails;
	int preferredDevice = 0;

	int index = 0;
	bool finnished = false;

	while(index < deviceCount && !finnished) 
	{
		xAudio2->GetDeviceDetails(index, &deviceDetails);
		if(deviceDetails.OutputFormat.Format.nChannels > 2)
		{
			preferredDevice = index;
			finnished = true;
		}

		index++;
	}

	HRESULT hr;
	hr = xAudio2->CreateMasteringVoice(	&masterVoice,
										XAUDIO2_DEFAULT_CHANNELS,
										XAUDIO2_DEFAULT_SAMPLERATE,
										0,
										preferredDevice,
										NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"Could not create Mastering Voice!", L"SoundManager error!", MB_OK | MB_ICONEXCLAMATION);
	}
}

void SoundManager::initialize()
{
	#ifndef _XBOX
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#endif

	createXAudio2();
	createMasterVoice();

	createSound(__TEXT("Files/Music/NightCrickets.wav"), XAUDIO2_LOOP_INFINITE, 0);
	createSound(__TEXT("Files/Music/BadReligion.wav"), XAUDIO2_LOOP_INFINITE/*XAUDIO2_NO_LOOP_REGION*/, 0);
	createSound(__TEXT("Files/Music/TensionPianoDoomLow.wav"), XAUDIO2_LOOP_INFINITE, 0);
	createSound(__TEXT("Files/Music/ZombieEating.wav"), XAUDIO2_LOOP_INFINITE, 0);
}

void SoundManager::startVoice(int voiceIndex)
{
	sourceVoices[voiceIndex]->SubmitSourceBuffer(&buffers[voiceIndex]);
	sourceVoices[voiceIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}

void SoundManager::stopVoice(int voiceIndex)
{
	sourceVoices[voiceIndex]->Stop();
	sourceVoices[voiceIndex]->FlushSourceBuffers();
}

void SoundManager::setMasterVolume(float volume)
{
	if(volume > -XAUDIO2_MAX_VOLUME_LEVEL && volume < XAUDIO2_MAX_VOLUME_LEVEL)
		masterVoice->SetVolume(volume);
}

void SoundManager::setSourceVoiceVolume(int voiceIndex, float volume)
{
	if(volume > -XAUDIO2_MAX_VOLUME_LEVEL && volume < XAUDIO2_MAX_VOLUME_LEVEL)
		sourceVoices[voiceIndex]->SetVolume(volume);
}

void SoundManager::setSourceVoiceVolumeDistance(int voiceIndex, float distance)
{
	float radius = 490.0f;
	float volume = ((radius - distance)/(radius));

	if(volume < 0)
		volume = 0;

	sourceVoices[voiceIndex]->SetVolume(volume);
}

void SoundManager::setSourceVoicePanning(int voiceIndex, float angle, bool fromRight)
{
	float position = sin(angle);

	float volumeLeft = 0;
	float volumeRight = 0;

	if(!fromRight)
		position *= -1;

	volumeLeft	= sqrt(2.0f) / 2 * (cos(position)+sin(position));
	volumeRight = sqrt(2.0f) / 2 * (cos(position)-sin(position));

	float volumes[] = {volumeLeft, volumeRight};
	sourceVoices[voiceIndex]->SetChannelVolumes(2, volumes);
}