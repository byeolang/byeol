#include "CBlurFilter.hpp"
//	������
CBlurFilter::CBlurFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height)
: CFilter(device, render_target_width, render_target_height),
_xblured_texture(NULL), _xblured_surface(NULL)
{
	initializeShader(device);
	initializeHLSLValue(device);
	initializeResource(device);
}
//	���̴��� �ʱ�ȭ�Ѵ�.
void	CBlurFilter::initializeShader(LPDIRECT3DDEVICE9 device)
{
	loadShaderFile(DEFAULT_CBLURFILTER_HLSL_PATH, device);
}	
//	���̴��� ���õ� ������ ���ε��Ѵ�.
void	CBlurFilter::initializeHLSLValue(LPDIRECT3DDEVICE9 device)
{
	//	���ܻ�Ȳ ó��:
	if (!_enable)
		return ;

	//	���� �ڵ�:
	//		���ε�:
	_technique = _effect->GetTechniqueByName("Tech");
	_blur_level_width_handle = _effect->GetParameterByName(NULL, "m_TexW");
	_blur_level_height_handle = _effect->GetParameterByName(NULL, "m_TexH");
}
//	���ҽ��� �ʱ�ȭ�Ѵ�.
void	CBlurFilter::initializeResource(LPDIRECT3DDEVICE9 device)
{
	device->CreateTexture( _render_target_width, _render_target_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_xblured_texture, NULL);
	_xblured_texture->GetSurfaceLevel(0, &_xblured_surface);
}
//	�Ҹ���
CBlurFilter::~CBlurFilter()
{ 
	releaseResource();

	CFilter::release(); // ���̽�Ŭ���� ȣ��
}
//	���ҽ��� �����Ѵ�.
void	CBlurFilter::releaseResource()
{
	if (_xblured_texture)
	{
		_xblured_texture->Release();
		_xblured_texture = NULL;
	}
	if (_xblured_surface)
	{
		_xblured_surface->Release();
		_xblured_surface = NULL;
	}

}
//	����: ���� �Լ�
void	CBlurFilter::excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
{
	CBlurFilterData* blur_filter = casting(camera->_filter_data[DEFAULT_CSHADER_BLURFILTER]);
	if (!blur_filter)
		return ;

	update(blur_filter);
	filtering(device, texture, vertex_buffer, camera);
}
//	ĳ���� ��ȯ
CBlurFilterData*	CBlurFilter::casting(CFilterData* filter_data)
{
	return ( static_cast<CBlurFilterData*>(filter_data) );
}
//	���Ϳ� ���õ� �������� ������Ʈ �Ѵ�.
void	CBlurFilter::update(CBlurFilterData* blur_filter_data)
{
	updateBlurLevel(blur_filter_data);
}
//	�帲 ������ �����Ϳ��� ����, ���̴��� ������Ʈ�Ѵ�.
void	CBlurFilter::updateBlurLevel(CBlurFilterData* blur_filter_data)
{
	float height_squared = blur_filter_data->getBlurLevelHeight() * blur_filter_data->getBlurLevelHeight();
	
	_effect->SetFloat(_blur_level_height_handle, height_squared);
	_effect->SetFloat(_blur_level_width_handle, height_squared * _ratio);
}
//	������ ���͸��� �ǽ��Ѵ�.
//	texture�� Xblur_texture�� ���  ->  oldBUffer�� ����
void	CBlurFilter::filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
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
	device->SetRenderTarget(0, _xblured_surface);
	device->Clear(0, NULL, D3DCLEAR_TARGET, DEFAULT_CENGINE_RENDERTARGET_BACKGROUND, 1.0f, 0 );
	setVertex(vertex_buffer, CArea(-1,-1,1,1) );
	//			���̴� �غ�:
	_effect->SetTechnique(_technique);
	_effect->Begin(NULL, 0);
	//		2�н�: X���� ��		
	_effect->BeginPass(0);		
	device->SetTexture(0, texture);	
	drawVertex(device); // vertex = -1, -1, 1, 1
	_effect->EndPass();		
	//		3�н�: Y���� ��
	device->SetRenderTarget(0, old_surface); // ����Ÿ���� ����Ŭ���� �ʿ����
	_effect->BeginPass(1);		
	device->SetTexture(0, _xblured_texture);	
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

}

