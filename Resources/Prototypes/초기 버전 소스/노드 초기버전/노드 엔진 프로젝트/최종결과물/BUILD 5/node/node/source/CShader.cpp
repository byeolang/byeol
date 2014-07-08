#include "CShader.hpp"
//	������
CShader::CShader() : _iniShaderOn(false), _rendertarget_width(0), _rendertarget_height(0), _recent_index(0),
_device(NULL), _mainframe_surface(NULL), 
_last_preserve_index(NULL)
{
	//	���� ��:
	for (int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
		_filter[n] = 0;	
	//	�ӽ� Ÿ�� ��:
	for (int n=0; n < 2 ;n++)
	{
		_temp_texture[n] = NULL;
		_temp_surface[n] = NULL;
		_nested_filter_texture[n] = NULL;
		_nested_filter_surface[n] = NULL;
	}
}
//	���̴��� ���õ� ��� ���� �ϰ������� �ʱ�ȭ�Ѵ�.
void	CShader::initialize(LPDIRECT3DDEVICE9 device, CINI* ini)//	��� �ʱ�ȭ�� ���⼭ �Ͼ
{
	_iniShaderOn = ini->_graphic_isShaderOn; // ini���� ���̴��� ����?
	//	���ܻ�Ȳó��: ini���� ���̴��� ���ٸ� �ʱ�ȭ�� �����Ѵ�.
	if (!_iniShaderOn)	
		return ;
	
	//	�����ڵ�:
	//		�ʱ�ȭ:
	initializeValue(ini, device); // ���̴��� Ų ���, ���� ���Ϳ��� ���̴��� �Ǵ��� üũ�Ѵ�. �׷��� �� �Ǵ� ���ʹ� enable = true�� �ǰ�, �ȵǴ� ���ʹ� false.
	initializeRenderTargetTexture();		
	initializeFilter();
}		
//	�Ҹ���
CShader::~CShader()
{
	release();
}
//	���̴��� ���õ� ������ �ϰ������� �����Ѵ�. ������ �ߺ� ����Ǿ ���۵����� �ʰԲ� �ؾ��Ѵ�.
void	CShader::release() 
{
	//	���ܻ�Ȳó��:		
	if (!_iniShaderOn) // ini�� ������ �ٷ� ������... ini�� ������, ���̴� �ʱ�ȭ�� ������ ����, ������ ���͸� ��ȸ�ϸ鼭 _enable�� true���� Ȯ���Ѵ�.
		return ;		
	
	//	�����ڵ�:
	//		����:
	releaseRenderTargetTexture();
	releaseFilter();
}
//	CScripter::searchLastPreserveCamera�� ã�Ƴ� ������ preserve�� �ּҸ� �����ͼ� �����Ѵ�. �ΰ����� ���ȴ�.
//	1. preserve�� �����ϴ°�.
//	2. �������Ϸ��� ī�޶� ������ preserve ī�޶��ΰ�
void	CShader::setLastPreserveCamera(CCamera* camera)
{
	_last_preserve_index = camera; // �ּҰ��� ��. preserve�� �����ϴ°�. Ȥ��, �� ī�޶� last�ΰ��� üũ��
}
//	������ Ÿ���� �ε����� �ֽŰ� ���ֽ��� �ٲ۴�.
void	CShader::swapOldToRecentIndex()
{
	_recent_index = 1 - _recent_index; // 0 �Ǵ� 1
}
//	�ֽ� ������ Ÿ���� �ε����� �ѱ��.
int		CShader::getRecentIndex() // �������� ����
{
	return _recent_index;
}
//	���ֽ� ������ Ÿ���� �ε����� �ѱ��.
int		CShader::getOldIndex()
{
	return (1 - _recent_index);
}
//	������ Ÿ���� �������������� �����Ѵ�.
//	#����# _mainframe_surface�� �ٸ� ������Ÿ���� �����ϰԵǸ�, d3d��ü ��ŷ�Ǿ, 
//			����̽��ս� ������, restore�� �Ұ����ϰ� �ȴ�.
void	CShader::swapToMainFrame()
{
	_device->SetRenderTarget(0, _mainframe_surface);
}
//	�ֽ� ������ Ÿ������ �����Ѵ�.
void	CShader::swapToRecentTarget()
{
	_device->SetRenderTarget(0, _temp_surface[ getRecentIndex() ]);
}
//	���ֽ� ������ Ÿ������ �����Ѵ�.
void	CShader::swapToOldTarget()
{
	_device->SetRenderTarget(0, _temp_surface[ getOldIndex() ]);
}	
//	ī�޶� 1���� ������ �ϱ����� �����ؾ��� ���͸��۾��� �����Ѵ�.
void	CShader::preFiltering(CCamera* camera)
{
	if ( _iniShaderOn)
	{
		if (_last_preserve_index) // last preserve�� �����ϴ°�?
		{
			if (	!camera->_preserve &&
					camera->isThereEnabledFilter()	) // ���Ͱ� �����ϴ°�
			{
				// preserve�� �ִ� ��Ȳ���� ���� ����
				//	swap ���ֽ�, ����Ŭ����
				swapToOldTarget();
				clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);
			}
		}
		else
		{
			if (camera->isThereEnabledFilter()) // ���Ͱ� �����ϴ°�
			{
				// preserve�� ����, ���� ����
				swapToOldTarget();
				clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);
			}
		}
	}
}
//	������ ��, ���͸��� �ϱ� ���� �������۾�
void	CShader::postFiltering(CCamera* camera, LPDIRECT3DVERTEXBUFFER9 vertex_buffer)
{
	if ( _iniShaderOn)
	{
		if (_last_preserve_index) // last preserve�� �����ϴ°�?
		{
			if (camera->isPreserved()) // ��ī�޶�� preserve�� ����ϴ°�
			{
				if (camera == _last_preserve_index) // ���� �� ī�޶� ������ preserve�ΰ�?
				{
					// preserve�� �ִ� ��Ȳ���� ������ preserve ����
					//	�������������� Ÿ�� ���� (ó�� ���������ӿ� �׸��� ����. �̹� ����Ŭ��� �Ǿ�����)
					//	�ؽ��Ĵ� �ֽ�
					swapToMainFrame();
					filtering(camera, _temp_texture[getRecentIndex()], vertex_buffer);
					initializeLastPreserveCamera();
				}
				else
				{
					// preserve�� �ִ� ��Ȳ���� �߰� preserve ����
					//	Ÿ��: ���ֽ� -> ����Ŭ����.
					//	�ؽ���: �ֽ�
					//	�׸���, �ֽ��� ������
					swapToOldTarget();
					clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);
					filtering(camera, _temp_texture[getRecentIndex()], vertex_buffer);
					swapOldToRecentIndex();
				}
			}
			else 
			{
				if (camera->isThereEnabledFilter()) // ���Ͱ� �����ϴ°�
				{
					// preserve�� �ִ� ��Ȳ���� ���� ����
					//	Ÿ��: �ֽ�
					//	�ؽ���: ���ֽ�
					swapToRecentTarget();
					filtering(camera, _temp_texture[getOldIndex()], vertex_buffer);
				}
			}
		}
		else
		{
			if (camera->isThereEnabledFilter()) // ���Ͱ� �����ϴ°�
			{
				// preserve�� ����, ���� ����
				//	����:
				swapToMainFrame();
				filtering(camera, _temp_texture[getOldIndex()], vertex_buffer);					
			}
		}
	}
	else // ���̴��� ��������
	{
		// ���� �׳� ������			
		//	X (����������)
	}
}	
//	CScripter::searchLastPreserveCamera�κ��� ���� �ּҰ��� �η� �ʱ�ȭ�Ѵ�.
void	CShader::initializeLastPreserveCamera()
{
	_last_preserve_index = NULL; 
}
//	���� ������ Ÿ���� ���۸� ����.
void	CShader::clearBuffer(D3DCOLOR color)
{
	_device->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0 );
}
//	������ ���͸��� �Ͼ��.
//	��� ���Ͱ� enable������ �����ϰ�, ������ ������ �ε����� �����Ѵ�.
//	���� ���Ͱ� 1���� ���, singleFiltering�� ȣ���ϰ�, 2�� �̻��̸�, nestedFiltering�� ȣ���Ѵ�.
void	CShader::filtering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer)
{
	//	�ʱ�ȭ:
	char	last_filter_index	= 0; 
	char	filter_count		= 0;

	//	Ž�� ����:
	for(int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
	{
		if (camera->_filter_data[n]->_enable)
		{
			filter_count++;
			last_filter_index = n;
		}
	}

	//	��� �м�:
	if (filter_count > 1)
		nestedFiltering(camera, texture, vertex_buffer, last_filter_index);
	else
		singleFiltering(camera, texture, vertex_buffer);
}
//	���͸� �ʱ�ȭ�Ѵ�.
void	CShader::initializeFilter() // #����#�߰�#
{
	_filter[DEFAULT_CSHADER_BLURFILTER] = new CBlurFilter(_device, _rendertarget_width, _rendertarget_height);
	_filter[DEFAULT_CSHADER_NOISEFILTER] = new CNoiseFilter(_device, _rendertarget_width, _rendertarget_height);
	_filter[DEFAULT_CSHADER_BLOOMFILTER] = new CBloomFilter(_device, _rendertarget_width, _rendertarget_height);
}
//	������Ÿ�ٰ� ���õ� ���Ҹ� ��� �ʱ�ȭ�Ѵ�.
void	CShader::initializeRenderTargetTexture()
{
	//	temp Ÿ�� �ʱ�ȭ:
	_device->CreateTexture( _rendertarget_width, _rendertarget_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_temp_texture[0], NULL);
	_temp_texture[0]->GetSurfaceLevel(0, &_temp_surface[0]);
	_device->CreateTexture( _rendertarget_width, _rendertarget_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_temp_texture[1], NULL);
	_temp_texture[1]->GetSurfaceLevel(0, &_temp_surface[1]);
	//	mainFrame Ÿ�� �ʱ�ȭ:
	_device->GetRenderTarget(0, &_mainframe_surface);
	//	��ø���͸��� ���� �ӽ� �ؽ��� �ʱ�ȭ:
	_device->CreateTexture( _rendertarget_width, _rendertarget_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_nested_filter_texture[0], NULL);
	_nested_filter_texture[0]->GetSurfaceLevel(0, &_nested_filter_surface[0]);
	_device->CreateTexture( _rendertarget_width, _rendertarget_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_nested_filter_texture[1], NULL);
	_nested_filter_texture[1]->GetSurfaceLevel(0, &_nested_filter_surface[1]);
}
//	�Ϲ� �������� �ʱ�ȭ�Ѵ�.
void	CShader::initializeValue(CINI* ini, LPDIRECT3DDEVICE9 device)
{
	//	����̽�:
	_device = device;
	//	���̴� ũ��(=����Ƽ)
	_rendertarget_width = static_cast<UINT> (ini->getShaderWidth());
	_rendertarget_height = static_cast<UINT> (ini->getShaderHeight());
}
//	���͸� �������Ѵ�.
void	CShader::releaseFilter()
{
	for (int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
	{
		if (_filter[n])
		{
			delete _filter[n];
			_filter[n] = NULL;
		}			
	}
}
//	������Ÿ�ٰ� ���õ� ���ҽ��� ���� �������Ѵ�.
void	CShader::releaseRenderTargetTexture()
{
	//	���������� Ÿ��:
	if (_mainframe_surface)
	{
		_mainframe_surface->Release();
		_mainframe_surface = NULL;
	}
	//	�ӽ� Ÿ��:
	if (_temp_texture[0])
	{
		_temp_texture[0]->Release();
		_temp_texture[0] = NULL;
	}
	if (_temp_surface[0])
	{
		_temp_surface[0]->Release();
		_temp_surface[0] = NULL;
	}
	if (_temp_texture[1])
	{
		_temp_texture[1]->Release();
		_temp_texture[1] = NULL;
	}
	if (_temp_surface[1])
	{
		_temp_surface[1]->Release();
		_temp_surface[1] = NULL;
	}
	if (_nested_filter_texture[0])
	{
		_nested_filter_texture[0]->Release();
		_nested_filter_texture[0] = NULL;
	}
	if (_nested_filter_surface[0])
	{
		_nested_filter_surface[0]->Release();
		_nested_filter_surface[0] = NULL;
	}
	if (_nested_filter_texture[1])
	{
		_nested_filter_texture[1]->Release();
		_nested_filter_texture[1] = NULL;
	}
	if (_nested_filter_surface[1])
	{
		_nested_filter_surface[1]->Release();
		_nested_filter_surface[1] = NULL;
	}
}
//	�� ī�޶��� ���Ͱ� �Ѱ��� ���� ���
void	CShader::singleFiltering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer)
{
	for (int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
	{
		if (camera->_filter_data[n]->_enable)
		{
			_filter[n]->excute(_device, texture, vertex_buffer, camera);
			return;
		}				
	}
}
//		��ø���͸�: �ϳ��� ī�޶��� �������� ���Ͱ� ���ÿ� �������
void	CShader::nestedFiltering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, char last_filter_index)
{
	//	������. ��ø ī�޶� preserve�� ������ �����. �������.
	//	�˰���.
	//		ó���϶�, oldtarget ���� -> target = recent_temp -> ����Ŭ���� texture = ����
	//		�߰��ܰ�, target = old_temp -> ����Ŭ���� -> tex = recent_temp
	//		������,	  target = oldtarget -> tex = recent

	//	��ø���͸� �غ�:
	//		���� ����Ÿ�� ����:
	LPDIRECT3DSURFACE9	old_buffer = NULL;		
	int					nested_filter_recent_index = 0;
	_device->GetRenderTarget(0, &old_buffer);

	//	���� ����:		
	bool	first_filtering = true;
	for (int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
	{
		if (camera->_filter_data[n]->_enable)
		{
			if (first_filtering)
			{
				//	ó���ܰ�:
				//	target = �ֱ� Ÿ��
				//	texture = ���� �ؽ���
				_device->SetRenderTarget(0, _nested_filter_surface[nested_filter_recent_index]);
				clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);				
				first_filtering = false;
				_filter[n]->excute(_device, texture, vertex_buffer, camera);
			}				
			else if (n == last_filter_index) // �̰� ������ ���Ͷ��
			{
				//	������:
				//	target = ���� �������� Ÿ��(old_buffer)
				//	texture = �ֱ� �ؽ���
				_device->SetRenderTarget(0, old_buffer);						
				_filter[n]->excute(_device, _nested_filter_texture[nested_filter_recent_index], vertex_buffer, camera);
				break;
			}
			else 
			{
				//	�߰�:
				//	target = ���ֱ� Ÿ��
				//	texture = �ֱ� �ؽ���
				_device->SetRenderTarget(0, _nested_filter_surface[1-nested_filter_recent_index]);						
				clearBuffer(DEFAULT_CENGINE_RENDERTARGET_BACKGROUND);
				_filter[n]->excute(_device, _nested_filter_texture[nested_filter_recent_index], vertex_buffer, camera);
				nested_filter_recent_index = 1 - nested_filter_recent_index; // �ֱ� �ε��� ����
			}				
		}				
	}

	//	����:
	//		���� ������:
	old_buffer->Release();
	old_buffer = NULL;
}