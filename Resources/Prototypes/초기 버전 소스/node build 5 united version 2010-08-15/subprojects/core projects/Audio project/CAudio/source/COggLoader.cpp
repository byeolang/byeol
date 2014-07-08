#include "COggLoader.hpp"
#include "CAudioBuffer.hpp"
//-----------------------------------------------------------------------------
// Name: COggLoader::COggLoader()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.
//       Then call Read() as needed.  Calling the destructor or Close()
//       will close the file.
//-----------------------------------------------------------------------------
COggLoader::COggLoader()
{

}


//-----------------------------------------------------------------------------
// Name: COggLoader::~COggLoader()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
COggLoader::~COggLoader()
{

}

//-----------------------------------------------------------------------------
// Name: COggLoader::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT COggLoader::open(CString strFileName, CAudioBuffer* audio_buffer)
{
	FILE*	f;
	errno_t err;

#ifdef UNICODE
	err  = _wfopen_s( &f, strFileName, _T("rb") );
#else
	err  = fopen_s( &f, strFileName, _T("rb") );
#endif
	
	if ( err != 0)
	{
		CError	e
		(
			_T("���XX:\t Ogg, fopen ����. ���ϸ�:" + strFileName), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
		e.log();
		return E_FAIL;
	}

	//ov_open(f, &vf, NULL, 0);	//Windows does not like this function so we use ov_open_callbacks() instead
	if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_DEFAULT) < 0)
	{
		fclose(f);
		CError	e
		(
			_T("���XX:\t Ogg, ov_open ����. ���ϸ�:" + strFileName), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
		e.log();
		return E_FAIL;
	}

	//The vorbis_info struct keeps the most of the interesting format info
	vorbis_info *vi = ov_info(&vf, -1);

	//Set the wave format
	
	//memset(audio_buffer->_wave_format, 0, sizeof(WAVEFORMATEX));
	if (audio_buffer->_wave_format)
	{
		delete audio_buffer->_wave_format;
		audio_buffer->_wave_format = NULL;
	}
	audio_buffer->_wave_format = ( WAVEFORMATEX* )new CHAR[sizeof(WAVEFORMATEX)];
	memset(audio_buffer->_wave_format, 0, sizeof(WAVEFORMATEX));
	audio_buffer->_wave_format->cbSize	= sizeof(WAVEFORMATEX);
	audio_buffer->_wave_format->nChannels       = vi->channels;
	audio_buffer->_wave_format->wBitsPerSample  = 16;                    //Ogg vorbis is always 16 bit
	audio_buffer->_wave_format->nSamplesPerSec  = vi->rate;
	audio_buffer->_wave_format->nAvgBytesPerSec = audio_buffer->_wave_format->nSamplesPerSec * audio_buffer->_wave_format->nChannels * 2;
	audio_buffer->_wave_format->nBlockAlign     = 2 * audio_buffer->_wave_format->nChannels;
	audio_buffer->_wave_format->wFormatTag      = 1;


	//	ä��*���ú�Ʈ*PCM/8bits , �Ǵ� ä��*���ú�Ʈ*���÷���Ʈ*�ð�/8bits �� ����
	//	�̿��Ͽ� �� ����Ʈ���� ����ϸ� �ð��� ��� ov_time_total()�� �̿��Ͽ� ���Ѵ�.
	audio_buffer->_wave_size =  vi->channels * 16 * (DWORD)(ov_pcm_total(&vf, -1) / 8);
	CHAR*	data = new CHAR[audio_buffer->_wave_size];

	//	read:	
	DWORD	pos			= 0;
	int		ret			= 1;
	int		bitstream	= 0;
	while(ret && pos <= audio_buffer->_wave_size)
	{
		ret = ov_read(&vf, data + pos, audio_buffer->_wave_size - pos, 0, 2, 1, &bitstream);
		pos += ret;		
	}

	audio_buffer->_wave_data = (BYTE*) data;
	//	close:
	ov_clear(&vf);
	fclose(f);


	return S_OK;
}

HRESULT	COggLoader::load(CString filename, CAudioBuffer* audio_buffer)
{
	audio_buffer->releaseWaveFormat();
	audio_buffer->_load_success = true;

	if ( FAILED( open(filename, audio_buffer)) )
		return E_FAIL;

	return S_OK;
}