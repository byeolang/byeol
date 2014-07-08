/*
	���ϸ�:	CBlurFilterData.hpp
	�̸�:	�����͵����� Ŭ����
	���:	������Ŭ�������� �����͵����� Ŭ������ �ҷ��鿩�� �۵��� ��.
			ī�޶󺰷� ���͸� ���ϼ� �ְԲ� ��.
*/
#pragma once
#include "CFilterData.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CBlurFilterData : public CFilterData
{
//	����Լ�:
public:
	//	������:
	CBlurFilterData();
	//	�Ҹ���:
	~CBlurFilterData();
	//	������:
	//		�帲����:
	float	getBlurLevelHeight();
	void	setBlurLevelHeight(float blur_level_height);



//	���κ���:
private:
	//	�Ϲݺ���:
	float	_blur_level_height;
};