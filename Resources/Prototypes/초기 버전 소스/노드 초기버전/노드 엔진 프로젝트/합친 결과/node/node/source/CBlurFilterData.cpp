#include "CBlurFilterData.hpp"
//	������
CBlurFilterData::CBlurFilterData() : CFilterData(), _blur_level_height(30.0f)
{
}
//	�Ҹ���
CBlurFilterData::~CBlurFilterData()
{

}
//	�󸶳� �帮�� �ϴ��� ���� �Ѱ��ش�.
float	CBlurFilterData::getBlurLevelHeight()
{
	return _blur_level_height;
}
//	�帲������ �Ѱܿ� �����Ѵ�.
void	CBlurFilterData::setBlurLevelHeight(float blur_level_height)
{
	//	���ܻ�Ȳó��: ��輱(min, max)�� ������ ���� ����
	if (blur_level_height < DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MIN)
		blur_level_height = DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MIN;
	if (blur_level_height > DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MAX)
		blur_level_height = DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MAX;
	
	//	�����ڵ�:
	_blur_level_height = blur_level_height;
}
