/*
	���ϸ�:	CHUB.hpp
	�̸�:	HUB; ���
	���:	��� �� Ŭ������ �����ϴ� ����	
*/
#pragma once
//	����:
//		���̺귯��:
#pragma comment (lib, "winmm.lib")
//		�⺻ ��Ŭ���:
#include <Windows.h>
#include <MMSystem.h>
//		Ŀ���� ��Ŭ���:
#include "CWindow.hpp"
#include "CINI.hpp"
#include "define.hpp"
#include "CInput.hpp"
//	����:
//		Ŭ����:
class CHUB
{
//	��� �Լ�:
public:
	//	������:
	CHUB(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow);	
	//	�Ҹ���:
	~CHUB();	
	//	�޼���:
	void	handleMessage();



//	�������:
private:
	//	Ŭ����������:
	CWindow		_window; // �����츦 �����Ѵ�.
	CCartridge	_cartridge;
};