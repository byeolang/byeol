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
#include <time.h>
//		Ŀ���� ��Ŭ���:
#include "CEngine.hpp"
#include "CWindow.hpp"
#include "CScripter.hpp"
#include "CINI.hpp"
#include "define.hpp"

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
	CINI		_ini; // INI�� ������� �ʱ�ȭ �Ǿ� �Ѵ�.
	CWindow		_window; // �����츦 �����Ѵ�.
	CEngine		_engine; // CWindow ������ CEngine�� �ν��Ͻ� �Ǿ��Ѵ�.	
	CScripter	_scripter;
};