#include "CScripter.hpp"

CScripter::CScripter(CEngine* engine)
: _engine(engine)
{
	/*	
		#�۾�# ���߿� ����� �ʱ�ȭ ����..
		// �ӽ÷� ī�޶� �ʱ�ȭ
		//_camera[0]._x = 100; _camera[0]._y = 100;
		_camera[1]._angle = 0;	
	*/
	_camera[1]._argb.setAlpha(100);
	_camera[1]._argb.setRed(0);
	_camera[1]._argb.setBlue(0);
	//_camera[3]._angle = timeGetTime() / 2000.0f;
	_camera[4]._argb.setAlpha(20);
	_camera[3].setRenderState(DEFAULT_CENGINE_RENDERSTATE_ALPHABLENDING);
}
CScripter::~CScripter() 
{}
//	�ڽ��� ����ϰ� �ִ� ����� ����: �� ������ ��� �� ����ϱ�
void	CScripter::excute()
{
	// vectoring();
	rendering();
}

void	CScripter::rendering()
{
	//	#�۾�# �ϴ� �׽�Ʈ������ �̷��� �����..
	static float	angle1;
	static float	zoom = 1.0;
	static int		sign = 1;
	

	zoom += 1.0f * sign;	
	if(zoom > 600)
		sign = -1;
	else if (zoom < -600)
		sign = 1;
	_camera[0]._x = zoom;
	_camera[0]._y = zoom;

	_camera[1]._angle = timeGetTime() / 2000.0f;
	//	Űüũ
	//	��: F1~F3
	//	F1 : �� Ű�� / ����
	//	F2 : �� �帮��
	//	F3 : �� �帮��
	static	int	camera_index = 0;
	if ( GetAsyncKeyState(VK_ESCAPE) & 0x0001 ) // Ű�� ��������
	{
		if (_camera[camera_index]._enable)
			_camera[camera_index]._enable = false;
		else
			_camera[camera_index]._enable = true;
	}
	if ( GetAsyncKeyState(VK_F1) & 0x0001 ) // Ű�� ��������
	{
		if (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLURFILTER]->_enable)
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLURFILTER]->_enable = false;
		else
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLURFILTER]->_enable = true;

	}
	if ( GetAsyncKeyState(VK_F2) & 0x0001 ) // Ű�� ��������
	{				
		CBlurFilterData* blur_data = static_cast<CBlurFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLURFILTER]);
		float blur_level = blur_data->getBlurLevelHeight();

		blur_level -= 0.5f;
		blur_data->setBlurLevelHeight(blur_level);
	}
	if ( GetAsyncKeyState(VK_F3) & 0x0001 ) // Ű�� ��������
	{				
		CBlurFilterData* blur_data = static_cast<CBlurFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLURFILTER]);
		float blur_level = blur_data->getBlurLevelHeight();

		blur_level += 0.5f;
		blur_data->setBlurLevelHeight(blur_level);
	}

	//	������:	F4~F8
	//	F5:������ Ű�� / ����
	//	F6:MAX�� ���̱�
	//	F7:MAX�� ���߱�
	//	F8:�����̸� ������ �ϱ�
	if ( GetAsyncKeyState(VK_F5) & 0x0001 ) // Ű�� ��������
	{
		if (_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]->_enable)
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]->_enable = false;
		else
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]->_enable = true;

	}
	if ( GetAsyncKeyState(VK_F6) & 0x0001 ) // Ű�� ��������
	{				
		CNoiseFilterData* noise_data = static_cast<CNoiseFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]);
		int noise_max = noise_data->getNoiseMaxLevel();
		int noise_min = noise_data->getNoiseMinLevel();
		
		noise_max = noise_max + 15;
		noise_min = noise_min + 15;
		noise_data->setNoiseMaxLevel(noise_max);
		noise_data->setNoiseMinLevel(noise_min);
	}
	if ( GetAsyncKeyState(VK_F7) & 0x0001 ) // Ű�� ��������
	{				
		CNoiseFilterData* noise_data = static_cast<CNoiseFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]);
		int noise_max = noise_data->getNoiseMaxLevel();
		int noise_min = noise_data->getNoiseMinLevel();


		noise_min = noise_min - 15;		
		noise_max = noise_max - 15;
		noise_data->setNoiseMaxLevel(noise_max);
		noise_data->setNoiseMaxLevel(noise_min);
	}
	if ( GetAsyncKeyState(VK_F8) & 0x0001 ) // Ű�� ��������
	{				
		CNoiseFilterData* noise_data = static_cast<CNoiseFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_NOISEFILTER]);
		int noise_delay = noise_data->getDelay();

		noise_delay += 1;
		noise_data->setDelay(noise_delay);
	}


	//	���: F9, F11~F12
	//	F9:��� Ű�� / ����
	//	F11:��� �� ���ϰ�
	//	F12:��� �� ���ϰ�
	
	if ( GetAsyncKeyState(VK_F9) & 0x0001 ) // Ű�� ��������
	{
		if (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLOOMFILTER]->_enable)
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLOOMFILTER]->_enable = false;
		else
			_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLOOMFILTER]->_enable = true;

	}	
	if ( GetAsyncKeyState(VK_F11) & 0x0001 ) // Ű�� ��������
	{				
		CBloomFilterData* bloom_data = static_cast<CBloomFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLOOMFILTER]);
		float bloom_level = bloom_data->getBloomLevel();

		bloom_data->setBloomLevel(bloom_level + 0.005f);						
	}
	if ( GetAsyncKeyState(VK_F12) & 0x0001 ) // Ű�� ��������
	{				
		CBloomFilterData* bloom_data = static_cast<CBloomFilterData*> (_camera[camera_index]._filter_data[DEFAULT_CSHADER_BLOOMFILTER]);
		float bloom_level = bloom_data->getBloomLevel();

		bloom_data->setBloomLevel(bloom_level - 0.005f);						
	}

	if ( GetAsyncKeyState('1') &0x0001)
	{
		camera_index = 1;
	}
	if ( GetAsyncKeyState('2') &0x0001)
	{
		camera_index = 2;
	}
	if ( GetAsyncKeyState('3') &0x0001)
	{
		camera_index = 3;
	}
	if ( GetAsyncKeyState('4') &0x0001)
	{
		camera_index = 4;
	}
	if ( GetAsyncKeyState('5') &0x0001)
	{
		camera_index = 5;
	}
	if ( GetAsyncKeyState('0') &0x0001)
	{
		camera_index = 0;
	}

	//	ī�޶� ����������Ʈ:
	//	insert: linear
	//	home:	alphablending
	//	delete:	negative
	//	end:	darken
	if ( GetAsyncKeyState(VK_INSERT) & 0x0001 ) // Ű�� ��������
	{
		_camera[camera_index].setRenderState(DEFAULT_CENGINE_RENDERSTATE_LINEAR);
	}	
	if ( GetAsyncKeyState(VK_HOME) & 0x0001 ) // Ű�� ��������
	{
		_camera[camera_index].setRenderState(DEFAULT_CENGINE_RENDERSTATE_ALPHABLENDING);
	}	
	if ( GetAsyncKeyState(VK_DELETE) & 0x0001 ) // Ű�� ��������
	{
		_camera[camera_index].setRenderState(DEFAULT_CENGINE_RENDERSTATE_NEGATIVE);
	}	
	if ( GetAsyncKeyState(VK_END) & 0x0001 ) // Ű�� ��������
	{
		_camera[camera_index].setRenderState(DEFAULT_CENGINE_RENDERSTATE_DARKEN);
	}	
	//	ī�޶� ���󺯰�:
	//	numpad 7, 4: ALPHA +, -
	//	numpad 8, 5: RED +, -
	//	numpad 9, 6: GREEN +, -
	//	numpad -, +: BLUE +, -
	//	numpad 0: �ʱ�ȭ
	if ( GetAsyncKeyState(VK_NUMPAD7) & 0x0001 ) // Ű�� ��������
	{
		int alpha = _camera[camera_index]._argb.getAlpha();
		alpha++;	
		_camera[camera_index]._argb.setAlpha(alpha);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD4) & 0x0001 ) // Ű�� ��������
	{
		int alpha = _camera[camera_index]._argb.getAlpha();
		alpha--;
		_camera[camera_index]._argb.setAlpha(alpha);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD8) & 0x0001 ) // Ű�� ��������
	{
		int red = _camera[camera_index]._argb.getRed();
		if (red < 255) 
			red++;	
		_camera[camera_index]._argb.setRed(red);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD5) & 0x0001 ) // Ű�� ��������
	{
		int red = _camera[camera_index]._argb.getRed();
		red--;	
		_camera[camera_index]._argb.setRed(red);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD9) & 0x0001 ) // Ű�� ��������
	{
		int blue = _camera[camera_index]._argb.getBlue();
		blue++;	
		_camera[camera_index]._argb.setBlue(blue);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD6) & 0x0001 ) // Ű�� ��������
	{
		int blue = _camera[camera_index]._argb.getBlue();
		blue--;	
		_camera[camera_index]._argb.setBlue(blue);
	}	
	if ( GetAsyncKeyState(VK_ADD) & 0x0001 ) // Ű�� ��������
	{
		int green = _camera[camera_index]._argb.getGreen();
		green++;	
		_camera[camera_index]._argb.setGreen(green);
	}	
	if ( GetAsyncKeyState(VK_SUBTRACT) & 0x0001 ) // Ű�� ��������
	{
		int green = _camera[camera_index]._argb.getGreen();
		green--;
		_camera[camera_index]._argb.setGreen(green);
	}	
	if ( GetAsyncKeyState(VK_NUMPAD0) & 0x0001 ) // Ű�� ��������
	{
		_camera[camera_index]._argb.initialize();
	}	

	//	ī�޶� preserve
	//	backspace:
	if ( GetAsyncKeyState(VK_BACK) & 0x0001 ) // Ű�� ��������
	{
		if (_camera[camera_index]._preserve)
			_camera[camera_index]._preserve = false;
		else
			_camera[camera_index]._preserve = true;
	}	


	_camera[1]._scale_x = 0.8f;
	_camera[1]._scale_y = 0.8f;
	_camera[2]._x = -400.0f;
	_camera[2]._y = -400.0f;

	_camera[3]._x = 100.0f;
	_camera[3]._y = -150.0f;

	_camera[4]._x = +350.0f;
	_camera[4]._y = +350.0f;



	// ����� �˰�ĥ�Ѵ�
	preRender();

	for( int n=0; n < TEMP_CAMERA_NUMBER ;n++)
	{
		if (_camera[n]._enable)
			_engine->rendering(&_camera[n], n);
	}

	if ( postRender() == D3DERR_DEVICELOST ) // ����̽� ����
		_engine->restore();				
}
//	�������� �������� �ϱ����� �����ϴ� �۾���. �������ӿ� �ѹ��� ����ȴ�.
void	CScripter::preRender()
{	
	_engine->clearBuffer(DEFAULT_CENGINE_MAINFRAME_BACKGROUND); // ���� ����: �̹�, �������������� Ÿ���� �Ǿ� ����		
	searchLastPreserveCamera(); // ī�޶� Ž��
}
//	ī�޶�� �߿���, ���� �������� ��� PRESERVE�� Ž���Ѵ�. (����Ž��)
void	CScripter::searchLastPreserveCamera() // ī�޶�迭�� �� Ž����
{
	// �����δ�, �迭�� �ƴ϶� C����ڸ� �����
	for (int n=TEMP_CAMERA_NUMBER-1; n >= 0 ;n--) // !�Ųٷ� Ž��!
	{
		if (_camera[n].isPreserved()) // preserved ���͸� ������ΰ�
		{
			_engine->_shader.setLastPreserveCamera(&_camera[n]);
			_engine->_shader.swapToRecentTarget();
			_engine->clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);
			return ;
		}
	}	
}
//	�������ӿ� �ʿ��� ��� ������ �۾��� ������ ����Ǵ� �۾�.
HRESULT	CScripter::postRender()
{
	return _engine->present();
}