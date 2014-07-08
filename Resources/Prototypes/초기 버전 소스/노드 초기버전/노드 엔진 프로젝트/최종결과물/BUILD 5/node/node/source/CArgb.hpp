/*
	���ϸ�:	CARgb.hpp
	�̸�:	ARGB Ŭ���� (CRgb�� �Ļ�Ŭ����)
	���:	ARGB���� ������. 
			���� CArgb���� ������ ����.
			DWORD�� CArgb�� ������ �ڵ����� D3DCOLOR_ARGB�� ������ ó���Ѵ�
	�޸�:	�׽�ƮX
			
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//		Ŀ���� ��Ŭ���:
#include "CRgb.hpp"
//	����:
//		Ŭ����:
class CArgb : public CRgb
{
//	����Լ�:
public:
	//	������:
	CArgb();
	CArgb(char alpha, char red, char green, char blue);
	CArgb(CArgb& argb);
	//	�Ҹ���:
	~CArgb();
	//	�ʱ�ȭ:
	void	initialize();
	//	������:
	//		����:
	void	setAlpha(int alpha);
	int		getAlpha();
	//		ARGB:	
	void	setARGB(int alpha, int red, int green, int blue);
	//		������ ������ �ִ°�: ARGB���� ��ȿ�Ѱ�
	bool	isSetted();
	//	������ �����ε�:
	DWORD	toD3DCOLOR();
	CArgb&	operator = (CArgb& argb);



//	�������:
private:
	//	�Ϲݺ���:
	int		_alpha;
	bool	_alpha_setted;
};