/*
	���ϸ�:	CBloomFilterData.hpp
	�̸�:	������͵����� Ŭ����
	���:	�������Ŭ�������� ������͵����� Ŭ������ �ҷ��鿩�� �۵��� ��.
			ī�޶󺰷� ���͸� ���ϼ� �ְԲ� ��.
*/
#pragma once
//	����:
//		Ŀ���� ��Ŭ���:
#include "CFilterData.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CBloomFilterData : public CFilterData
{
//	����Լ�:
public:
	//	������:
	CBloomFilterData();
	//	�Ҹ���:
	~CBloomFilterData();
	//	������:
	//		������ ����:
	float	getBloomLevel();
	void	setBloomLevel(float bloom_level);
	

	
//	���κ���:
private:
	//	�Ϲݺ���:
	float	_bloom_level;
};