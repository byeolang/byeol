#include "CCamera.hpp"

CCamera::CCamera() 
: _x(0.0f), _y(0.0f), _z(0.0f), _angle(0.0f), _scale_x(1.0f), _scale_y(1.0f), 
_argb(), _preserve(false), _enable(true), _render_state(DEFAULT_CENGINE_RENDERSTATE_DEFAULT), _render_state_setted(false)
{
	initializeFilterData();
}
CCamera::CCamera(float x, float y, float z, float angle, float scale_x, float scale_y, CArgb argb, bool preserve, char render_state) 
: _x(x), _y(y), _z(z), _angle(angle), _scale_x(scale_x), _scale_y(scale_y), 
_render_state(render_state), _argb(argb), _render_state_setted(true),
_preserve(preserve), _enable(true)
{}
//	�Ҹ���:
CCamera::~CCamera()
{}
//	Ȱ��ȭ�� ���Ͱ� �ϳ��� �ִ��� �˻��Ѵ�
bool	CCamera::isThereEnabledFilter()
{
	for (int n=0; n < DEFAULT_CSHADER_FILTER_NUMBER ;n++)
	{
		if (_filter_data[n]->_enable)
			return true;
	}
	
	return false;
}
//	���͸� �ʱ�ȭ�Ѵ�.
void	CCamera::initializeFilterData() // #����#�߰�#
{
	_filter_data[DEFAULT_CSHADER_BLURFILTER] = new CBlurFilterData();
	_filter_data[DEFAULT_CSHADER_NOISEFILTER] = new CNoiseFilterData();
	_filter_data[DEFAULT_CSHADER_BLOOMFILTER] = new CBloomFilterData();
}
//	ī�޶��� ����������Ʈ�� �����Ѵ�.
//	_render_state_setted�� true�� �Ǹ�, �� ī�޶�� �������ϴ� ��� ��尳ü��, 
//	ī�޶��� ����������Ʈ�� �����Ѵ�.
void	CCamera::setRenderState(char render_state)
{
	if (render_state > DEFAULT_CENGINE_RENDERSTATE_NUMBER)
		render_state = DEFAULT_CENGINE_RENDERSTATE_NUMBER;

	_render_state = render_state;
	_render_state_setted = true;
}
//	���� ����������Ʈ�� �����´�.
char	CCamera::getRenderState()
{
	return _render_state;
}
//	����������Ʈ�� �ʱ�ȭ�Ѵ�. (������, ��尳ü�� ���������̸� �������� �ʴ´�)
void	CCamera::initializeRenderState()
{
	_render_state = DEFAULT_CENGINE_RENDERSTATE_DEFAULT;
	_render_state_setted = false;
}
//	�� ī�޶� preserve ���������� ��ȯ�Ѵ�.
//	�ܼ���, _preserve �� true�̶�� �ص�, ���͸� �ѳ��� ������ �ǹ̰� ����.
//	�׷���, ���Ͱ� ���������� false�� ��ȯ�Ѵ�.
bool	CCamera::isPreserved()
{
	if (_preserve && isThereEnabledFilter())
		return true;
	else
		return false;
}