/*
	���ϸ�:	CRgb.hpp
	�̸�:	RGB Ŭ����
	���:	RGB���� int�� ���� ����
	�޸�:	�׽�ƮX
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//	����:
//		Ŭ����:
class CRgb
{
//	��� �Լ�:
public:
	//	������:
	//		�⺻������:
	CRgb();
	//		float 4��:
	CRgb(char red, char green, char blue);
	//		���������:
	CRgb(CRgb& rgb);
	//	�Ҹ���:
	~CRgb();
	//	�ʱ�ȭ:
	void	initialize();
	//	������ �����ε�:
	DWORD	toD3DCOLOR();
	CRgb&	operator = (CRgb& rgb);
	//	������:
	//		����:
	void	setRed(int red);
	int		getRed();
	//		�ʷ�:
	void	setGreen(int green);
	int		getGreen();
	//		�Ķ�:
	void	setBlue(int blue);
	int		getBlue();
	//		RGB:
	void	setRGB(int red, int green, int blue);
	//		������ �� �� �ִ°�: RGB���� ��ȿ�Ѱ�
	bool	isSetted();
	


//	�������:
protected:
	//	RGB:
	int		_red, _green, _blue;
	bool	_setted; // �ܺο��� ���� ������ �߳�
};
