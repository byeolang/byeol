#include "CBloomFilterData.hpp"
//	������
CBloomFilterData::CBloomFilterData() : _bloom_level(DEFAULT_CBLOOMFILTER_BLOOM_LEVEL)
{}
//	�Ҹ���
CBloomFilterData::~CBloomFilterData()
{}
//	����� ������ �����´�
float	CBloomFilterData::getBloomLevel()
{
	return _bloom_level;
}
//	�󸶳� ����� �Ұ����� ���� �ִ´�.
void	CBloomFilterData::setBloomLevel(float bloom_level)
{
	//	���ܻ�Ȳó��:
	if (bloom_level < DEFAULT_CBLOOMFILTER_BLOOM_MIN_LEVEL)
		bloom_level = DEFAULT_CBLOOMFILTER_BLOOM_MIN_LEVEL;

	_bloom_level = bloom_level;
}
	

