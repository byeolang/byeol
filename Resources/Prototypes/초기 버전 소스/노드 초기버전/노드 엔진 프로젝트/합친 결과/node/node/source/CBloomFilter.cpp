#include "CBloomFilter.hpp"
//	������:
CBloomFilter::CBloomFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height)
: CFilter(device, render_target_width, render_target_height),
_dif_texture_handle(NULL), _dib_texture_handle(NULL)
{
	initializeShader(device);
	initializeHLSLValue(device);
	initializeResource(device);
}
//	���̴��� �ʱ�ȭ�Ѵ�
void	CBloomFilter::initializeShader(LPDIRECT3DDEVICE9 device)
{
	loadShaderFile(DEFAULT_CBLOOMFILTER_HLSL_PATH, device);
}	
//	HLSL�� ������ ���ε�
void	CBloomFilter::initializeHLSLValue(LPDIRECT3DDEVICE9 device)
{
	//	���ܻ�Ȳ ó��:
	if (!_enable)
		return ;

	//	���� �ڵ�:
	//		���ε�:
	_technique = _effect->GetTechniqueByName("Tech");
	_bloom_level_handle = _effect->GetParameterByName(NULL, "_bloom");
	_dif_texture_handle = _effect->GetParameterByName(NULL, "m_TxDif");
	_dib_texture_handle = _effect->GetParameterByName(NULL, "m_TxDiB");
}
void	CBloomFilter::initializeResource(LPDIRECT3DDEVICE9 device)
{		
	device->CreateTexture( _render_target_width, _render_target_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_temp_texture[0], NULL);
	_temp_texture[0]->GetSurfaceLevel(0, &_temp_surface[0]);		
	device->CreateTexture( _render_target_width, _render_target_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_temp_texture[1], NULL);
	_temp_texture[1]->GetSurfaceLevel(0, &_temp_surface[1]);
}
//	�Ҹ���
CBloomFilter::~CBloomFilter()
{ 
	releaseResource();

	CFilter::release(); // ���̽�Ŭ���� ȣ��
}
//	���ҽ��� �����Ѵ�
void	CBloomFilter::releaseResource()
{
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
}
//	����: ���� �Լ�
void	CBloomFilter::excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
{
	CBloomFilterData* bloom_filter = casting(camera->_filter_data[DEFAULT_CSHADER_BLOOMFILTER]);
	if (!bloom_filter)
		return ;

	update(bloom_filter);
	filtering(device, texture, vertex_buffer, camera);
}
//	ĳ���ú�ȯ:
CBloomFilterData*	CBloomFilter::casting(CFilterData* filter_data)
{
	return ( static_cast<CBloomFilterData*>(filter_data) );
}
//	���� ������Ʈ:
void	CBloomFilter::update(CBloomFilterData* bloom_filter_data)
{
	updateBloomLevel(bloom_filter_data);
}
//	��� ������ ������Ʈ�Ѵ�
void	CBloomFilter::updateBloomLevel(CBloomFilterData* bloom_filter_data)
{
	//	�����ڵ�:
	//		���ε�:
	_effect->SetFloat(_bloom_level_handle, bloom_filter_data->getBloomLevel());		
}
//	���͸�:	
void	CBloomFilter::filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
{
	//	���ܻ�Ȳ ó��:
	if (!_enable)
		return ;

	//	���� �ڵ�:
	//		�غ�:
	//			oldBuffer: ���� Ÿ���� ������
	LPDIRECT3DSURFACE9	old_surface;
	device->GetRenderTarget(0, &old_surface); // main������ �Ǵ� �ֽ��ؽ��İ� ������
	//			���ο� Ÿ��:
	device->SetRenderTarget(0, _temp_surface[0]);
	device->Clear(0, NULL, D3DCLEAR_TARGET, DEFAULT_CENGINE_RENDERTARGET_BACKGROUND, 1.0f, 0 );
	setVertex(vertex_buffer, CArea(-1,-1,1,1) );
	//			���̴� �غ�:
	_effect->SetTechnique(_technique);
	_effect->Begin(NULL, 0);
	//		2�н�: ���� ���̱�
	_effect->BeginPass(0);		
	device->SetTexture(0, texture); // ����
	drawVertex(device); // vertex = -1, -1, 1, 1
	_effect->EndPass();	
	//		3�н�: X���� ��
	device->SetRenderTarget(0, _temp_surface[1]);
	device->Clear(0, NULL, D3DCLEAR_TARGET, DEFAULT_CENGINE_RENDERTARGET_BACKGROUND, 1.0f, 0 );
	_effect->BeginPass(1);		
	device->SetTexture(0, _temp_texture[0]);
	drawVertex(device);
	_effect->EndPass();
	//		4�н�: Y���� ��
	device->SetRenderTarget(0, _temp_surface[0]);		
	device->Clear(0, NULL, D3DCLEAR_TARGET, DEFAULT_CENGINE_RENDERTARGET_BACKGROUND, 1.0f, 0 );
	_effect->BeginPass(2);		
	device->SetTexture(0, _temp_texture[1]);
	drawVertex(device);
	_effect->EndPass();		
	//		5�н�: ���ļ� ���� ���
	device->SetRenderTarget(0, old_surface);
	_effect->BeginPass(3);
	device->SetTexture(0, texture); // ����
	device->SetTexture(1, _temp_texture[0]); // ����
	//			ī�޶���: ����������� ��������, ī�޶��� ����������Ʈ�� argb�� �����Ѵ�.		
	//setVertex(vertex_buffer, CArea(-1, -1, 1, 1), camera->_argb.toD3DCOLOR()); // ī�޶��� rgba�� ����
	setRenderState(device, camera->getRenderState()); // ī�޶��� ����������Ʈ�� ����
	//			���:
	drawVertex(device); // vertex = -1, -1, 1, 1
	_effect->EndPass();		
	//		����:		
	_effect->End();		
	old_surface->Release();
	old_surface = NULL;
	device->SetTexture(1, NULL);
}
