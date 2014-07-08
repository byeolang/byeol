#pragma once
#pragma comment(lib, "winmm.lib")
#include <MMSystem.h>

#include "CError.hpp"
#include <xaudio2.h>
#include <strsafe.h>
#include <conio.h>
#include "CAudioBuffer.hpp"

#ifdef	_DEBUG
#define DEFAULT_CAUDIO_INTIALIZE_XAUDIO_FLAG	XAUDIO2_DEBUG_ENGINE
#else
#define DEFAULT_CAUDIO_INTIALIZE_XAUDIO_FLAG	0
#endif
class CAudio
{
//����Լ�:
public:
	//	������:
	CAudio(int fx_max_channel): _xaudio2(NULL), _mastering_voice(NULL), _fx_submix(NULL), _music_submix(NULL),
	_fx_file(NULL), _fx_bank_capacity(0), _fx_bank_size(0), _music_file(NULL), _music_bank_capacity(0), _music_bank_size(0),
	_fx_source(NULL), _music_source(NULL), _enable(false), _fx_max_channel(fx_max_channel)

	{		
		_enable = true; // �߰��� ������ ����, �� �Լ� ���ο��� release�� ȣ��Ǹ鼭 �ڵ����� enable�� false�� ��
		initializeXAudio2();
		initializeMasteringVoice();
		initializeSubmixVoice();
		initializeSoundSource();
		initializeMusicBuffer();
	}	
	//	�Ҹ���:
	~CAudio()
	{
		release();
	}
	void	pauseAudio()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;

		_xaudio2->StopEngine();
	}
	void	unpauseAudio()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;

		_xaudio2->StartEngine();
	}
	void	initializeSoundCapacity(int fx_bank_capacity)
	{
		//	���ܻ�Ȳó��:
		if (fx_bank_capacity < 1)
		{
			CError	e
			(
				_T("���XX:\tȿ������ũ�� ��뷮(capacity)�� ������ 0���� �۰ų� �����ϴ�. ���� ������ �߰����� ���ϰԵ˴ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			return ;
		}

		//	�����ڵ�:
		_fx_bank_capacity = fx_bank_capacity;
		_fx_file = new CAudioBuffer[_fx_bank_capacity];
		_fx_bank_size = 0;
	}
	void	loadSound(CString filename)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		//	���ܻ�Ȳó��:
		if (_fx_bank_capacity <= _fx_bank_size)
		{
			CError	e
			(
				_T("���XX:\t������ũ�� �� á���ϴ�. ������ �߰����� ���߽��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			return ;
		}

		//	�����ڵ�:
		_fx_file[_fx_bank_size].load(filename);
		_fx_bank_size++;	
	}
	void	playSound(int index)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		//	���ܻ�Ȳó��:
		if (index >= _fx_bank_size)
			return ;
		if (!_fx_file)
			return ;
		if (!_fx_file[index]._load_success)
			return ;

		XAUDIO2_SEND_DESCRIPTOR SFXSend = {0, _fx_submix};	
		XAUDIO2_VOICE_SENDS SFXSendList = {1, &SFXSend};
		HRESULT hr = NULL;
		int		target_index = -1;
		//	� fx�ε����� ���� ������ ã��.
		for (int n=0; n < _fx_max_channel ;n++)
		{
			if ( !isPlaying(&_fx_source[n]) )
			{
				target_index = n;
				break;
			}
		}
		if (target_index == -1) // ä���� ��ȭ���¶�, ���̻� ȿ������ ���� �� ����
			return ; 
		
		if (_fx_source[target_index])
		{
			_fx_source[target_index]->Stop();
			_fx_source[target_index]->DestroyVoice();
		}

		hr = _xaudio2->CreateSourceVoice( &_fx_source[target_index], _fx_file[index]._wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL,	&SFXSendList, NULL);
		if( FAILED( hr ) )
		{
			CError	e
			(
				_T("���XX:\tȿ�����ҽ����̽��� �������� ���߽��ϴ�. ������ ������� �ʽ��ϴ�. ���ϸ�:" + _fx_file[index]._filename ), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			if (_fx_source[target_index])
			{
				_fx_source[target_index]->DestroyVoice();
				_fx_source[target_index] = NULL; 
			}
		}

		XAUDIO2_BUFFER	temp_buffer = {0};
		temp_buffer.pAudioData = _fx_file[index]._wave_data;
		temp_buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		temp_buffer.AudioBytes = _fx_file[index]._wave_size;
		
		if( FAILED( _fx_source[target_index]->SubmitSourceBuffer( &temp_buffer ) ) )
		{
			CError	e
			(
				_T("���XX:\tȿ�����ҽ����۸� SUBMIT �������߽��ϴ�. ������ ������� �ʽ��ϴ�. ���ϸ�:" + _fx_file[index]._filename), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			_fx_source[target_index]->DestroyVoice();
			_fx_file[index].releaseWaveFormat();
			_fx_file[index].releaseWaveData();
			_fx_file[index]._load_success = false; // �Ź� �̺κб��� �ͼ� �����ϰ� �����, ���ϰ� ����ġ �����Ƿ�, ���ġ ���۸� �����Ѵ�
		}
		_fx_source[target_index]->Start();
	}
	void	pauseSound()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;

		for (int n=0; n < _fx_max_channel ;n++)
		{
			if (_fx_source[n])
			{
				_fx_source[n]->Stop();
			}
		}
	}
	void	unpauseSound()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		
		for (int n=0; n < _fx_max_channel ;n++)
		{
			if (_fx_source[n])
			{
				_fx_source[n]->Start();
			}
		}
	}
	float	getSoundVolume()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return 0.0f;
		float	volume = 0.0f;
		_fx_submix->GetVolume(&volume);
		
		return volume;
	}
	void	setSoundVolume(float volume)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;

		_fx_submix->SetVolume(volume);
	}
	void	initializeMusicCapacity(int music_bank_capacity)
	{
		//	���ܻ�Ȳó��:
		if (music_bank_capacity < 1)
		{
			CError	e
			(
				_T("���XX:\t������ũ�� ��뷮(capacity)�� ������ 0���� �۰ų� �����ϴ�. ���� ������ �߰����� ���ϰԵ˴ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			return ;
		}

		//	�����ڵ�:
		_music_bank_capacity = music_bank_capacity;
		_music_file = new CAudioBuffer[_music_bank_capacity];
		_music_bank_size = 0;
	}
	void	loadMusic(CString filename)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		//	���ܻ�Ȳó��:
		if (_music_bank_capacity <= _music_bank_size)
		{
			CError	e
			(
				_T("���XX:\t������ũ�� �� á���ϴ�. ������ �߰����� ���߽��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			return ;
		}

		//	�����ڵ�:
		_music_file[_music_bank_size].load(filename);
		_music_bank_size++;		
	}
	void	playMusic(int index, DWORD loop_count = 0) // XAUDIO2_LOOP_INFINITE: infinity, 0: no looping
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		//	���ܻ�Ȳó��:
		if (index > _music_bank_size)
			return ;
		if (!_music_file)
			return ;
		if (!_music_file[index]._load_success)
			return ;

		
		XAUDIO2_SEND_DESCRIPTOR SFXSend = {0, _music_submix};	
		XAUDIO2_VOICE_SENDS SFXSendList = {1, &SFXSend};
		HRESULT hr = NULL;

		if (_music_source)
		{
			_music_source->Stop();
			_music_source->DestroyVoice();
		}

		hr = _xaudio2->CreateSourceVoice( &_music_source, _music_file[index]._wave_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL,	&SFXSendList, NULL);
		if( FAILED( hr ) )
		{
			CError	e
			(
				_T("���XX:\t�����ҽ����̽��� �������� ���߽��ϴ�. ������ ������� �ʽ��ϴ�. ���ϸ�:" + _music_file[index]._filename ), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			if (_music_source) 
			{
				_music_source->DestroyVoice();
				_music_source = NULL;
			}

		}

		initializeMusicBuffer();
		_music_buffer.pAudioData = _music_file[index]._wave_data;
		_music_buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		_music_buffer.AudioBytes = _music_file[index]._wave_size;
		_music_buffer.LoopCount = loop_count;
		
		//buffer.PlayLength = pwfx->nSamplesPerSec * 2;
		if( FAILED( _music_source->SubmitSourceBuffer( &_music_buffer ) ) )
		{
			CError	e
			(
				_T("���XX:\t�����ҽ����۸� SUBMIT �������߽��ϴ�. ������ ������� �ʽ��ϴ�. ���ϸ�:" + _music_file[index]._filename), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			_music_source->DestroyVoice();
			_music_file[index].releaseWaveFormat();
			_music_file[index].releaseWaveData();
			_music_file[index]._load_success = false; // �Ź� �̺κб��� �ͼ� �����ϰ� �����, ���ϰ� ����ġ �����Ƿ�, ���ġ ���۸� �����Ѵ�
		}
		_music_source->Start();
	}
	void	pauseMusic()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		if (!_music_source)
			return ;

		_music_source->Stop();
	}
	void	unpauseMusic()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		if (!_music_source)
			return ;

		_music_source->Start();
	}
	void	replayMusic()
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;
		if (!_music_source)
			return ;

		if ( isPlaying(&_music_source) )
			_music_source->Stop();
		_music_source->FlushSourceBuffers();
		_music_buffer.PlayBegin = 0;
		_music_source->SubmitSourceBuffer(&_music_buffer);
		_music_source->Start();
	}
	float	getMusicVolume()
	{	//	���ܻ�Ȳó��:
		if (!_enable)
			return 0.0f;

		float	volume;
		_music_submix->GetVolume(&volume);
		
		return volume;
	}
	void	setMusicVolume(float volume)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return ;

		_music_submix->SetVolume(volume);
	}
	bool	isPlaying(IXAudio2SourceVoice** source_voice)
	{
		//	���ܻ�Ȳó��:
		if (!_enable)
			return false;
		if (!*source_voice)
			return false;

		XAUDIO2_VOICE_STATE state;
		(*source_voice)->GetState(&state);
	
		if (state.BuffersQueued > 0)
			return true;
		else
			return false;
	}


//	�����Լ�:
private:
	void	initializeXAudio2()
	{
		CoInitializeEx( NULL, COINIT_MULTITHREADED );

		if( FAILED( XAudio2Create(&_xaudio2, DEFAULT_CAUDIO_INTIALIZE_XAUDIO_FLAG)) )
		{
			CError	e
			(
				_T("���XX:\tXAudio2 �ʱ�ȭ ����. �Ҹ��� ������ �ʽ��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
			release();
		}		
	}
	void	release()
	{
		releaseResource();
		//releaseDevice();	//	�Ϻ� ��ǻ�Ϳ��� ������ �߻��Ѵ�. �׸��� ������ �߻��ϸ� �� ������ ��� �۾��� ĵ���ǹǷ� (������ ���� �𸣰���) �ϴ� ������, �׳� ���д�
		//CoUninitialize();	
		_enable = false; // CAudio��ü�� ��������. ������ ��ġ�� unload���� �˸��� ���� enable�� false�� ��
	}
	void	releaseDevice()
	{
		
		if (_fx_submix)
		{
			_fx_submix->DestroyVoice();
			_fx_submix = NULL;
		}
		if (_music_submix)
		{
			_music_submix->DestroyVoice();
			_music_submix = NULL;
		}		
		if (_mastering_voice)
		{
			_mastering_voice->DestroyVoice();			
			_mastering_voice = NULL;
		}		
		if (_xaudio2)
		{
			_xaudio2->Release();			
			_xaudio2 = NULL;
		}
	}
	void	releaseResource()
	{
		releaseMusicResource();
		releaseSoundResource();
		releaseMusicSource();
		releaseSoundSource();
	}
	void	releaseMusicSource()
	{		
		if (_music_source)
		{
			_music_source->DestroyVoice();
			_music_source = NULL;
		}
	}
	void	releaseSoundSource()
	{
		if (_fx_source)
		{
			for (int n=0; n < _fx_max_channel ;n++)
			{
				if ( _fx_source[n] )
				{
					_fx_source[n]->DestroyVoice();
					_fx_source[n] = NULL;
				}			
			}

			delete [] _fx_source;
			_fx_source = NULL;
		}
	}
	void	releaseMusicResource()
	{
		if (_music_file)
		{
			for (int n=0; n < _music_bank_size ;n++) // size�� index.
			{
				_music_file[n].release();
			}

			delete [] _music_file;
			_music_file = NULL;
		}
	}
	void	releaseSoundResource()
	{
		if (_fx_file)
		{
			for (int n=0; n < _fx_bank_size ;n++) // size�� index.
			{	
				_fx_file[n].release();
			}

			delete [] _fx_file;
			_fx_file = NULL;
		}
	}
	void	initializeMasteringVoice()
	{
		if( FAILED( _xaudio2->CreateMasteringVoice(&_mastering_voice)) )
		{
			CError	e
			(
				_T("���XX:\t�����͸� ���̽� �ʱ�ȭ ����. �Ҹ��� ������ �ʽ��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);			
			e.log();

			release();
		}
	}
	void	initializeSubmixVoice()
	{
		if( FAILED(_xaudio2->CreateSubmixVoice(&_fx_submix, 2, 44100 ) ) )
		{
			CError	e
			(
				_T("���XX:\tȿ���� ����ͽ� ���̽� �ʱ�ȭ ����. �Ҹ��� ������ �ʽ��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);	
			e.log();

			release();
		}
		if( FAILED( _xaudio2->CreateSubmixVoice(&_music_submix, 2, 44100 ) ) )
		{
			CError	e
			(
				_T("���XX:\t���� ����ͽ� ���̽� �ʱ�ȭ ����. �Ҹ��� ������ �ʽ��ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);	
			e.log();

			release();
		}
	}
	void	initializeMusicBuffer()
	{
		_music_buffer.AudioBytes = 0;
		_music_buffer.Flags = 0;
		_music_buffer.LoopBegin = 0;
		_music_buffer.LoopCount = 0;
		_music_buffer.LoopLength = 0;
		_music_buffer.pAudioData = 0;
		_music_buffer.pContext = 0;
		_music_buffer.PlayBegin = 0;
		_music_buffer.PlayLength = 0;
	}


	void	initializeSoundSource()
	{
		if (_fx_source)
			releaseSoundSource();

		_fx_source = new IXAudio2SourceVoice*[_fx_max_channel];
		for (int n=0; n < _fx_max_channel ;n++)
		{
			_fx_source[n] = NULL;
		}
	}
//	�������:
public:
	IXAudio2SubmixVoice*	_fx_submix;
	IXAudio2SubmixVoice*	_music_submix;

//	���κ���:
private:
	//	xaudio:
	IXAudio2*				_xaudio2;
	IXAudio2MasteringVoice* _mastering_voice;	
	//	BANK:
	//		ȿ����:
	CAudioBuffer*	_fx_file; //_fx_file[_fx_bank_capacity]
	int				_fx_bank_size;
	int				_fx_bank_capacity;
	//		����:
	CAudioBuffer*	_music_file;
	int				_music_bank_size;
	int				_music_bank_capacity;
	//	SOURCEVOICE:
	//		ȿ����:
	IXAudio2SourceVoice*	*_fx_source;
	int						_fx_max_channel; // ini���� ������
	//		����:
	IXAudio2SourceVoice*	_music_source; // �ϳ��� ����
	XAUDIO2_BUFFER			_music_buffer;

	//	���º���
	bool	_enable;
};