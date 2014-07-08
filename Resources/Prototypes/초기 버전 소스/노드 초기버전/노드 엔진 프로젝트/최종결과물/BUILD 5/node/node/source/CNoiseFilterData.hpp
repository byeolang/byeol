/*
	���ϸ�:	CNoiseFilterData.hpp
	�̸�:	���������͵����� Ŭ����
	���:	����������Ŭ�������� ���������͵����� Ŭ������ �ҷ��鿩�� �۵��� ��.
			ī�޶󺰷� ���͸� ���ϼ� �ְԲ� ��.
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <Windows.h>
//		Ŀ���� ��Ŭ���:
#include "CFilterData.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CNoiseFilterData : public CFilterData
{
//	����Լ�:
public:
	//	������:
	CNoiseFilterData();
	//	�Ҹ���:
	~CNoiseFilterData();
	//	������:
	//		������ ����:
	int		getNoiseMaxLevel();
	void	setNoiseMaxLevel(int noise_max);
	int		getNoiseMinLevel();
	void	setNoiseMinLevel(int noise_min);
	//		������:
	void	setDelay(int delay);
	int		getDelay();
	bool	isTimeElapsed();


//	�������:
public:
	float	_before_yrepeat;
	float	_before_xrepeat;

//	���κ���:
private:
	//	�Ϲݺ���:
	int		_min;
	int		_max;
	int		_delay; // milisecond
	DWORD	_before_worked_milisecond; // �������� �۵��ߴ� Ÿ��.

};