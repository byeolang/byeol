/*
	���ϸ�:	CINI.hpp
	�̸�:	INI �δ�
	���:	�ý��� ����(�׷���, ����, ����) && ���ӿɼ� �� ����
*/

#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h> // CString�� API���� ����ϱ�����
//		Ŀ���� ��Ŭ���:
#include "CError.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CINI
{
//	��� �Լ�:
public:
	//	������:
	CINI() 
	{}
	//	�Ҹ���:
	~CINI()
	{

	}



//	���κ���:
private:
	//		ȭ�� ����:
	WINDOW_RATIO	_graphic_ratio;
};