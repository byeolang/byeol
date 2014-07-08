#include "CEngine.hpp"

//	������
CEngine::CEngine(CINI* ini) : _ini(ini), _direct(NULL), _device(NULL),
_texture(NULL), _texture_capacity(0), _texture_size(0),
_playable_display_mode_number(0), _playable_display_modes(NULL), _selected_display_mode(),
_isHardwareVertexProcessingEnable(D3DCREATE_SOFTWARE_VERTEXPROCESSING), _caps()
{	
	initializeDirectx();
	initializeDisplayMode();	
	initializeDevice();	
	initializeCapabilities();
	initializeVertexBuffer();
	initializeShader();
	initializeMatrix();
	initializeRenderState();	
	//	#�۾�#	���߿� CTexture�� CScripter���� �ʱ�ȭ��Ű��	
	initializeTextureCapacity(5);
	loadTexture(_T("..\\resource\\graphic\\background2.jpg"));
	loadTexture(_T("..\\resource\\graphic\\background.png"));	
	loadTexture(_T("..\\resource\\graphic\\car.bmp"));
	loadTexture(_T("..\\resource\\graphic\\logo.bmp"));
	loadTexture(_T("..\\resource\\graphic\\game.bmp"));
}
//	�Ҹ���
CEngine::~CEngine()
{
	releaseShader();
	//	���ؽ�����:
	if (_vertex_buffer)
		_vertex_buffer->Release();
	//	����̽�:
	if (_device)
		_device->Release();		
	//	���̷�ƮX:
	if (_direct)
		_direct->Release();	
}
//	�Ѱܹ��� ī�޶� ���� �ð����� �������Ѵ�
void	CEngine::render(CCamera* camera, int index)
{	
	DWORD	color = 0;
	//	ī�޶� ����:
	//		����������Ʈ:
	if (camera->_render_state_setted)
		setRenderState(camera->getRenderState());
	else
		setRenderState(DEFAULT_CENGINE_RENDERSTATE_DEFAULT);
	//		����:
	if ( camera->_argb.isSetted() )
		color = camera->_argb.toD3DCOLOR();
	else	
		color = D3DCOLOR_ARGB(255,255,255,255);
	//		ī�޶� ��ġ:
	getWorldMatrix(camera);
	

	_device->BeginScene();

	if (index == 0)
	{	
		// #�۾�# �߰��� ������ �̰���	

		setVertex(	CArea(	-_texture[1]._halfwidth,
			-_texture[1]._halfheight,
			_texture[1]._halfwidth,
			_texture[1]._halfheight),
			color );	
		_texture[1].setTexture(_device); // ���
		drawVertex();
	}
	else if (index == 1)
	{
		setVertex(	CArea(	-_texture[2]._halfwidth,
			-_texture[2]._halfheight,
			_texture[2]._halfwidth,
			_texture[2]._halfheight),
			color );
		_texture[2].setTexture(_device); // ĳ����
		drawVertex();

		setVertex(	CArea(100-_texture[2]._halfwidth,
			100-_texture[2]._halfheight,
			100+_texture[2]._halfwidth,
			100+_texture[2]._halfheight),
			color );
		_texture[2].setTexture(_device); // ĳ����
		drawVertex();
	}
	else if (index == 2)
	{		
		setVertex(	CArea(	-_texture[0]._halfwidth,
			-_texture[0]._halfheight,
			_texture[0]._halfwidth,
			_texture[0]._halfheight),
			color );
		_texture[0].setTexture(_device); // ���2
		drawVertex();
	}
	else if (index == 3)
	{
		
		setVertex(	CArea(	-_texture[3]._halfwidth,
			-_texture[3]._halfheight,
			_texture[3]._halfwidth,
			_texture[3]._halfheight),
			color );
		_texture[3].setTexture(_device); // ���2
		drawVertex();
	}
	else if (index == 4)
	{
		setVertex(	CArea(	-_texture[4]._halfwidth,
			-_texture[4]._halfheight,
			_texture[4]._halfwidth,
			_texture[4]._halfheight),
			color );
		_texture[4].setTexture(_device); // ���2
		drawVertex();
	}

	_device->EndScene(); 
}
//	���̷�Ʈ9�� �����Ѵ�.
void	CEngine::initializeDirectx()
{
	_direct = Direct3DCreate9(D3D_SDK_VERSION);
	if ( !_direct)
	{
		// �ɰ��� ����. �޼����ڽ��� ��½�Ű�� ���ؼ� throw��
		throw CError 
		(	
			_T("����01:\t���̷�ƮX9 ���� ����"), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
	}
}
//	���� ���÷��� ��带 �����Ѵ�
void CEngine::getCurrentDisplayMode(D3DDISPLAYMODE* display)
{	
	if( FAILED( _direct->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, display)))
	{
		// �ɰ��� ����. �޼����ڽ��� ��½�Ű�� ���ؼ� throw��
		CError e
		(	
			_T("����02:\t���� ����ϰ� �ִ� ���÷��̸�带 Ȯ������ ���߽��ϴ�."), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
		e.log();

		throw e;
	}
}
//	�ؽ��ĸ� ���� �迭�� �뷮�� ���Ѵ� (�ʱ�ȭ�� ����)
void	CEngine::initializeTextureCapacity(int texture_capacity)
{	
	//	�ʱ�ȭ:
	if ( _texture )
	{
		delete [] _texture;
		_texture = NULL;
	}
	_texture_capacity = texture_capacity;
	_texture_size = 0;
	//	�Ҵ�:
	_texture = new CTexture[_texture_capacity];
}
//	����̽� �ʱ�ȭ
void	CEngine::initializeDevice()
{
	//	Direct 3D ����̽� ����:
	//		���� ä���:
	//			��üȭ�� && ������â ����:
	ZeroMemory( &_present_parameter, sizeof(_present_parameter) );		
	_present_parameter.SwapEffect = DEFAULT_CENGINE_CREATEDEVICE_SWAPEFFECT; // ȭ����ȯ ���	
	_present_parameter.Windowed = _ini->_graphic_window_mode; // ��üȭ�� or ������	
	_present_parameter.BackBufferFormat = _selected_display_mode.Format; // ã�Ƴ� ���� ���
	if ( _ini->_graphic_vertical_synchronize == true ) // ��������ȭ�� ��üȭ�鿡���� ����ڿ� ���ؼ� on�� �ȴ�
		_present_parameter.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	else
		_present_parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//			��üȭ�� ���:	
	if (_present_parameter.Windowed == FALSE)
	{	
		_present_parameter.BackBufferCount = DEFAULT_CENGINE_CREATEDEVICE_BACKBUFFERCOUNT;
		_present_parameter.BackBufferHeight = _selected_display_mode.Height;
		_present_parameter.BackBufferWidth = _selected_display_mode.Width;		
		_present_parameter.EnableAutoDepthStencil = false;
		_present_parameter.FullScreen_RefreshRateInHz = _selected_display_mode.RefreshRate;
		if ( _ini->_graphic_vertical_synchronize == true ) // ��������ȭ�� ��üȭ�鿡���� ����ڿ� ���ؼ� on�� �ȴ�
			_present_parameter.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		else
			_present_parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
		_present_parameter.FullScreen_RefreshRateInHz = 0; // ������ ��忡���� 0�̾�� �Ѵ�		
	

	//		����̽� ����:
	if( FAILED(_direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		CWindow::_hWnd, _isHardwareVertexProcessingEnable, &_present_parameter, &_device)) ) // ����ƽ����(HWND)�� ����ϰ��ִ�
	{
		// �ɰ��� ����. �޼����ڽ��� ��½�Ű�� ���ؼ� throw��
		throw CError
		(	
			_T("����03:\t����̽� ���� ����"), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
	}
}
//	���ؽ����� �ʱ�ȭ
void	CEngine::initializeVertexBuffer()
{
	//	���ؽ� ����:
	if ( FAILED(_device->CreateVertexBuffer(	
		4 * sizeof(MYVERTEX), 0, DEFAULT_CENGINE_D3DFVF_MYVERTEX, D3DPOOL_DEFAULT, &_vertex_buffer,	NULL)))
	{
		// �ɰ��� ����.
		throw CError
		(	
			_T("����04:\t���ؽ����� ���� ����"), 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
	}				
	//	���ؽ� �ʱ�ȭ:		
	_device->SetFVF(DEFAULT_CENGINE_D3DFVF_MYVERTEX);
	_device->SetStreamSource(0, _vertex_buffer, 0, sizeof(MYVERTEX));
}
//	������� �ʴ�, ������ �ռ� ����� �����Ѵ�. (���ӳ�����, �ι��ٽ� �ǵ帱 �ʿ����)
void	CEngine::initializeRenderState()
{
	//	����������Ʈ:
	//		3D�� ����� �׸�: OFF�� ��
	_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	_device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	_device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);	
	_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);	
	//	�ؽ��Ľ�������������Ʈ:
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // COLOR�μ�(ARG)1�� 2�� ���� ���Ѱ��� ���������� ��������
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // �μ�(ARG)1�� �ؽ����� ��
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); // �μ�2�� ���ؽ��� ��		
	//	Į�� ��������:
	_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}
//	��Ʈ������ �ʱ�ȭ�Ѵ�
void	CEngine::initializeMatrix()
{
	//	�����:
	D3DXMATRIXA16	viewer_matrix,
					projection_matrix;
	D3DXMatrixIdentity(&viewer_matrix);		
	_device->SetTransform(D3DTS_VIEW, &viewer_matrix);		
	//	�������� ���:	������ ũ�⿡ �°� ���� (����ũ��) 2���� �ؽ��ĸ� ��������, �ؼ��� -0.5f��ŭ �̵��ؾ� ����� ������.
	//					�װ��� �����ϱ� ���ؼ� ����� �����, ������Ŀ� ������
	//	�ؽ��� ����:
	D3DXMATRIXA16	texture_correction_matrix;
	D3DXMatrixTranslation(&texture_correction_matrix,-0.5f,-0.5f,0);
	//	���� ���:
	D3DXMatrixOrthoLH(	&projection_matrix, 
						(float) _ini->getScreenWidth(),
						(float) _ini->getScreenHeight(),
						DEFAULT_CENGINE_MATRIXTRANSFORM_MAX_Z, DEFAULT_CENGINE_MATRIXTRANSFORM_MIN_Z );		
	_device->SetTransform(D3DTS_PROJECTION, &(texture_correction_matrix * projection_matrix));		
}
//	���ؽ��� �����Ѵ�.
void	CEngine::setVertex(CArea vertex_area, DWORD color, CArea texture_area)
{	
	//	���� �ᱸ��:
	MYVERTEX*	vertex;
	_vertex_buffer->Lock(	0, 4 * sizeof(MYVERTEX), (void**) &vertex, NULL);	
	//	���ؽ� ����:
	//		����:
	vertex[0].x = vertex_area._x1;		vertex[0].y = vertex_area._y2;
	vertex[0].tu = texture_area._x1;	vertex[0].tv = texture_area._y1;
	vertex[0].z = 1.0f;					vertex[0].color = color;
	//		�»�:
	vertex[1].x = vertex_area._x1;		vertex[1].y = vertex_area._y1;
	vertex[1].tu = texture_area._x1;	vertex[1].tv = texture_area._y2;
	vertex[1].z = 1.0f;					vertex[1].color = color;
	//		����:
	vertex[2].x =vertex_area._x2;		vertex[2].y = vertex_area._y2;
	vertex[2].tu = texture_area._x2;	vertex[2].tv = texture_area._y1;
	vertex[2].z = 1.0f;					vertex[2].color = color;
	//		���:
	vertex[3].x = vertex_area._x2;		vertex[3].y = vertex_area._y1;
	vertex[3].tu = texture_area._x2;	vertex[3].tv = texture_area._y2;
	vertex[3].z = 1.0f;					vertex[3].color = color;
	//	���� Ǯ��:
	_vertex_buffer->Unlock();
	
}
//	���ؽ����۸� �׸���
void	CEngine::drawVertex()
{		
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // D3DPT_TRIANGLESTRIPǥ�ù��, ������ �����ε���, ǥ���� �ﰢ���� ����) 			
}
//	ī�޶� ���߰��ִ� ������ �����Ѵ�
void	CEngine::getWorldMatrix(CCamera* camera)
{
	//	�ʱ�ȭ:
	D3DXMATRIXA16	translation_matrix,
					rotation_matrix,
					scaling_matrix;
	//	��� ���ϱ�:
	D3DXMatrixTranslation(&translation_matrix, camera->_x, camera->_y, camera->_z ); // �����̵�	
	D3DXMatrixRotationZ(&rotation_matrix, camera->_angle );// ȸ��	
	D3DXMatrixScaling(&scaling_matrix, camera->_scale_x, camera->_scale_y, 1.0f);// ũ������
	//	���� ����� �����Ͽ� ��ȯ:
	_device->SetTransform(D3DTS_WORLD, &(translation_matrix * rotation_matrix * scaling_matrix));
}
//	���ϰ�ο� �����ϴ� �׸� ������ �о ���� ����� ��ġ�� �ؽ��ĸ� ��´�
void	CEngine::loadTexture(CString filename)
{
	if (_texture_size < _texture_capacity ) // _texture_capacity-1 : �ִ� ��밡���� �ε���
	{
		_texture[_texture_size].setFileName(filename);
		_texture[_texture_size].loadTexture(_device, _ini->_grpahic_32bit_texture);
		_texture_size++;		
	}
	else
	{
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���02:\t�ؽ����� ��뷮�� �Ѿ����ϴ�. ��ũ��Ʈ������ �߸����� �ʾҳ� �ǽɵ˴ϴ�."), 
			_T("�ذ���:\t"),
			_T(__FUNCTION__)
		);
		e.log();	
	}	
}
//	����̽��� ������
void	CEngine::restore()
{
	HRESULT	results = _device->TestCooperativeLevel();
	while(results == D3DERR_DEVICELOST)
	{
		while(results != D3DERR_DEVICENOTRESET)
		{
			Sleep(1000);
			MSG	message;
			PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&message);
			DispatchMessage(&message);

			results = _device->TestCooperativeLevel();
		}		
		releaseResource(); // ������ �ǽ���
		if ( FAILED(releaseDevice()) )
		{
			results = D3DERR_DEVICELOST;
		}
		else
		{			
			restoreDevice(); // ��ġ�� ���� �����Ѵ�
			restoreResource();
			results = _device->TestCooperativeLevel();
		}		
	}
}
//	���ҽ��� Ǯ���ش�
void	CEngine::releaseResource()
{
	//	�ؽ���:
	for (int n = 0; n < _texture_capacity ;n++)
		_texture[n].UnloadTexture();
	//	���ؽ�:
	if (_vertex_buffer)
	{
		_vertex_buffer->Release();
		_vertex_buffer = NULL;
	}
	//	���̴������� ���� ����: ���Ͱ��� �״�� �����ȴ�.
	releaseShader();
}	
//	����̽��� �ٽ� �����Ѵ�
HRESULT	CEngine::releaseDevice()
{		
	return _device->Reset(&_present_parameter);
}
//	����̽��� �����Ѵ�
void	CEngine::restoreDevice()
{
	initializeVertexBuffer();
	initializeShader();
	initializeMatrix();
	initializeRenderState();
}
//	���ҽ��� �ٽ� �÷����´�
void	CEngine::restoreResource()
{		
	for (int n = 0; n < _texture_capacity ;n++)
		_texture[n].loadTexture(_device, _ini->_grpahic_32bit_texture);
}
// _select_display_mode�� ����� ����
void	CEngine::initializeDisplayMode() 
{
	//	���ܻ�Ȳ üũ:
	//		��������: ��������� 16/32��Ʈ, �츣��, ������ �ʿ���⶧���� �ٷ� ���� �� �ִ�.
	if (_ini->_graphic_window_mode)
	{
		getCurrentDisplayMode(&_selected_display_mode); // ���������, ���� ����ȭ��FORMAT���� �ؾ��Ѵ�.
		return ;
	}


	//	���� �ڵ�:
	//		�ʱ�ȭ:
	_playable_display_mode_number = 0;
	if (_playable_display_modes)
		delete [] _playable_display_modes;
	//		��밡���� ����� ������ ����:
	_playable_display_mode_number = getEnumerateDisplayModeCount(_ini->_graphic_32colorbit);
	if ( !_playable_display_mode_number) // ��� �����ߴµ� �����ٸ�
	{
		getCurrentDisplayMode(&_selected_display_mode); // ���������, ���� ����ȭ��FORMAT���� �ؾ��Ѵ�.
		return ;
	}

	//		�迭����:			
	_playable_display_modes = new D3DDISPLAYMODE[_playable_display_mode_number];
	//		�迭�� �� ����:
	getEnumerateDisplayMode(_ini->_graphic_32colorbit);	
	selectModeInEnumeratedMode(); // //	���� �迭�� �߿��� �ػ󵵸� ã�µ� �����Ұ��, ���ο��� �������(������ ���)�� ���õȴ�.		
}
//	�׷��� �ϵ������ ��� �� ������ �����Ѵ�
void	CEngine::initializeCapabilities()
{
	_direct->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &_caps);	

	//	�ϵ���� ���ؽ��� �����ϴ°�?
	if (_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		_isHardwareVertexProcessingEnable = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else	
		_isHardwareVertexProcessingEnable = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	

	//	���̴��� �����ϴ� ��
	//		SEPERATE_ALPHA�� �����Ѱ�: �Ұ��ɽ�, ����
	if (_ini->_graphic_isShaderOn)
	{
		if (_caps.PrimitiveMiscCaps & D3DPMISCCAPS_SEPARATEALPHABLEND)
		{
			if (	_caps.VertexShaderVersion < DEFAULT_CENGINE_VERTEXSHADER_VERSION ||
					_caps.PixelShaderVersion < DEFAULT_CENGINE_PIXELSHADER_VERSION )
			{
				_ini->_info_hardware_shader_support = false;
				_ini->_graphic_isShaderOn = false;
				CError	e
				(
					_T("���07:\t�׷��� ī�尡 ����Ϸ��� ���̴� ������ �������� ���մϴ�."), 
					_T("�ذ���"),
					_T(__FUNCTION__)
				);
				e.log();
			}
			else
				_ini->_info_hardware_shader_support = true; // ����
		}	
		else
		{
			_ini->_info_hardware_shader_support = false;
			_ini->_graphic_isShaderOn = false;
			CError	e
			(
				_T("���19:\t�׷��� ī�尡 SEPARATEALPHABLEND �� �������� �ʽ��ϴ�. ���̴� ����� ������ ���ϴ�."), 
				_T("�ذ���"),
				_T(__FUNCTION__)
			);
			e.log();
		}
	}
}
//	������ ���÷��̸���߿���, ������ ���ϴ� ��带 ã�´�
void	CEngine::selectModeInEnumeratedMode()
{
	for (int n = 0; n < _playable_display_mode_number ;n++)
	{
		if (	_playable_display_modes[n].Width == _ini->_graphic_width &&
				_playable_display_modes[n].Height == _ini->_graphic_height ) // �ϴ� �ػ�, ��常 ������, refresh�� �׳� ����
		{
			_selected_display_mode = _playable_display_modes[n];
			return ;
		}
	}

	//	�Ѱ��� ���ٸ�, �������� ����.
	//	��������, �������忡�� �۵��ϹǷ�, ���� ����ȭ���� ��带 �����´�.
	//	���� ����.
	CError e
	(	
		_T("���05:\t����, ������ ����Ǿ��� �׷��ȿɼ����� ���࿡ �����߽��ϴ�. ù �����̰ų�, �ٸ� ��ǻ���� ���� �ɼ����� ������ ��(ini����) �״�� ���������� �����̽��ϱ�?"),
		_T("���� ���� �׷��� �ɼ�(�������)���� �����մϴ�."),
		_T(__FUNCTION__)
	);
	e.log();
	_ini->setDefaultVeryLow();
	getCurrentDisplayMode(&_selected_display_mode);
}
//	�׷����ϵ��� �����ϴ� ��� ��带 �����Ѵ�
void	CEngine::getEnumerateDisplayMode(bool is32bitcolor)
{	
	int	count = 0,
		iter = 0;

	if (is32bitcolor)
	{
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_X8R8G8B8,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8B8G8R8); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_X8B8G8R8,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A8R8G8B8); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_A8R8G8B8,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A8B8G8R8); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_A8B8G8R8,iter, count); // iter���� count��ŭ.			
	}
	else
	{
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_R5G6B5); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_R5G6B5,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A1R5G5B5); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_A1R5G5B5,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A4R4G4B4); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_A4R4G4B4,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X1R5G5B5); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_X1R5G5B5,iter, count); // iter���� count��ŭ.
		iter += count;
		count = _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X4R4G4B4); // D3DFMT_R5G6B5�� ����
		enumerateOneDisplayMode(D3DFMT_X4R4G4B4,iter, count); // iter���� count��ŭ.			
	}

}
//	�ϳ��� ��带 �����Ѵ�
void	CEngine::enumerateOneDisplayMode(D3DFORMAT format, int start_index, int count)
{
	for (int n = 0; n < count ;n++)
		_direct->EnumAdapterModes(D3DADAPTER_DEFAULT, format, n, &(_playable_display_modes[start_index + n]) );		
}
//	�׷��� �ϵ��� �����ϴ� ����� ������ ���Ѵ�
int		CEngine::getEnumerateDisplayModeCount(bool is32bitcolor)
{
	int	count = 0;
	
	if ( is32bitcolor )
	{
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8B8G8R8);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A8R8G8B8);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A8B8G8R8);
	}
	else
	{
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_R5G6B5);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A1R5G5B5);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_A4R4G4B4);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X1R5G5B5);
		count += _direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X4R4G4B4);
	}

	return count;	
}
//	����Ʈ ����Ʈ�� �ʱ�ȭ�� �ʿ��� �������� ����ȴ�.
void	CEngine::initializeShader()
{		
	_shader.initialize(_device, _ini);
}

//	����Ʈ ����Ʈ ������ �ʿ��� ��� ������ ����ȴ�.
void	CEngine::releaseShader()
{
	_shader.release();
}


//	����������Ʈ�� ������
void	CEngine::setRenderState(char mode)
{
	if (mode == DEFAULT_CENGINE_RENDERSTATE_LINEAR)
	{
		_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_ALPHABLENDING)
	{
		_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_DARKEN)
	{
		_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	}
	else if (mode == DEFAULT_CENGINE_RENDERSTATE_NEGATIVE)
	{
		_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	}			 
}
//	�ϳ��� ī�޶� �������Ѵ�.
void	CEngine::rendering(CCamera* camera, int index)
{
	_shader.preFiltering(camera);	
	render(camera, index);	
	_shader.postFiltering(camera, _vertex_buffer);	
}
//	��� ������ �۾��� ������ �� �������� ��´�.
HRESULT	CEngine::present()
{
	return _device->Present(NULL, NULL, NULL, NULL);
}
//	ȭ���� ����.
void	CEngine::clearBuffer(D3DCOLOR color)
{
	_device->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0 );
}
