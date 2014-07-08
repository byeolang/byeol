#include "CNoiseFilter.hpp"
//	������
CNoiseFilter::CNoiseFilter(LPDIRECT3DDEVICE9 device, int render_target_width, int render_target_height)
: CFilter(device, render_target_width, render_target_height),
_noisemap_texture(), _xrepeat_handle(NULL), _yrepeat_handle(NULL), _source_texture_handle(NULL), _noise_texture_handle(NULL)
{
	initializeShader(device);
	initializeHLSLValue(device);
	initializeResource(device);
}
//	���̴� �ʱ�ȭ
void	CNoiseFilter::initializeShader(LPDIRECT3DDEVICE9 device)
{
	loadShaderFile(DEFAULT_CNOISEFILTER_HLSL_PATH, device);
}	
//	HLSL�� ���� ���ε�
void	CNoiseFilter::initializeHLSLValue(LPDIRECT3DDEVICE9 device)
{
	//	���ܻ�Ȳ ó��:
	if (!_enable)
		return ;

	//	���� �ڵ�:
	//		���ε�:
	_technique = _effect->GetTechniqueByName("Tech");
	_xrepeat_handle = _effect->GetParameterByName(NULL, "m_fRepeatX");
	_yrepeat_handle = _effect->GetParameterByName(NULL, "m_fRepeatY");
	_source_texture_handle = _effect->GetParameterByName(NULL, "source_texture");
	_noise_texture_handle = _effect->GetParameterByName(NULL, "noise_texture");
}
void	CNoiseFilter::initializeResource(LPDIRECT3DDEVICE9 device)
{
	_noisemap_texture.setFileName(DEFAULT_CNOISEFILTER_NOISEMAP_PATH);
	_noisemap_texture.loadTexture(device, true);
}
//	�Ҹ���
CNoiseFilter::~CNoiseFilter()
{ 
	releaseResource();

	CFilter::release(); // ���̽�Ŭ���� ȣ��
}
//	���ҽ� ����
void	CNoiseFilter::releaseResource()
{
	_noisemap_texture.UnloadTexture();
}
//	����: ���� �Լ�
void	CNoiseFilter::excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
{
	CNoiseFilterData* noise_filter = casting(camera->_filter_data[DEFAULT_CSHADER_NOISEFILTER]);
	if (!noise_filter)
		return ;

	update(noise_filter);
	filtering(device, texture, vertex_buffer, camera);
}
//	ĳ���ú�ȯ - ���͵����͸� ���������͵����ͷ�
CNoiseFilterData*	CNoiseFilter::casting(CFilterData* filter_data)
{
	return ( static_cast<CNoiseFilterData*>(filter_data) );
}
//	���� ������Ʈ
void	CNoiseFilter::update(CNoiseFilterData* noise_filter_data)
{
	updateNoiseLevel(noise_filter_data);
}
//	�������� �������� ������Ʈ �Ѵ�
void	CNoiseFilter::updateNoiseLevel(CNoiseFilterData* noise_filter_data)
{
	//	���ܻ�Ȳó��: �����̰� ���� �ȵȰ��.
	if (noise_filter_data->isTimeElapsed())
	{
		//	�����ڵ�:
		//		����Ÿ ���: ���̴��� ���ε��Ҷ�, �������� �� ��ȭ�� ũ�Ƿ�, max�� min�� �ٲ��� �Ѵ�.		
		int		max_level = DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL - noise_filter_data->getNoiseMinLevel(); 
		int		min_level = DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL - noise_filter_data->getNoiseMaxLevel();
		noise_filter_data->_before_xrepeat = (min_level + rand()%(max_level-min_level)) / 20.0f;
		noise_filter_data->_before_yrepeat = (min_level + rand()%(max_level-min_level)) / 20.0f;
	}
	//		���ε�:
	_effect->SetFloat(_xrepeat_handle, noise_filter_data->_before_xrepeat);
	_effect->SetFloat(_yrepeat_handle, noise_filter_data->_before_yrepeat);
}
//	���͸�
void	CNoiseFilter::filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera)
{		
	//	���ܻ�Ȳ ó��:
	if (!_enable)
		return ;

	//	���� �ڵ�:
	//		���̴� �غ�:
	_effect->SetTechnique(_technique);
	_effect->Begin(NULL, 0);
	//			���÷�������Ʈ: �� ���Ϳ����� ����ϴ� ��
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//		2�н�: ������
	_effect->BeginPass(0);		
	device->SetTexture(0, texture);
	device->SetTexture(1, _noisemap_texture._texture);
			
	//			ī�޶���: ����������� ��������, ī�޶��� ����������Ʈ�� argb�� �����Ѵ�.		
	setVertex(vertex_buffer, CArea(-1, -1, 1, 1)); // ī�޶��� rgba�� ����
	setRenderState(device, camera->getRenderState()); // ī�޶��� ����������Ʈ�� ����
	//			���:
	drawVertex(device); // vertex = -1, -1, 1, 1 
	_effect->EndPass();	
	//		����:		
	_effect->End();		
	device->SetTexture(1, NULL);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);	
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	device->SetTexture(1, NULL);
}

