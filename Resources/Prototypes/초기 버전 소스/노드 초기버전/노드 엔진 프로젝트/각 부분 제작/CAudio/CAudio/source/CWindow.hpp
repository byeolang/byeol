/*
	���ϸ�:	CWindow.hpp
	�̸�:	������ Ŭ����
	���:	�������� ���¸� �����Ѵ�	
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <windows.h>
#include <tchar.h> // �����ڵ� ��ũ�� ����
//		Ŀ���� ��Ŭ���:
#include "CINI.hpp"
#include "define.hpp"
#include "CAudio.hpp"
//		���漱��:
class CScripter;
//		�Լ�:
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//	����:
//		Ŭ����:
class CWindow
{
//	����Լ�:
public:	
	//	������:
	CWindow(HINSTANCE hInstance, int nCmdShow, CINI* ini, CAudio* audio);
	//	�Ҹ���:
	~CWindow();	
	//	�Ϲ��Լ�:
	void	HandleMessage();

//	�����Լ�:	
private:
	//	�ʱ�ȭ:
	void	initializeWindowClass();	
	void	initializeWindow();		
	//	�Ϲ��Լ�:
	void	setClientRect();
	void	isAlreadyExcuted();



//	�������:
public:	
	//		��������:
	static HWND	_hWnd;

//	���κ���:
private:	
	//		�Ϲ�:	
	MSG			_msg;	
	HINSTANCE	_hInstance;
	int			_nCmdShow;	
	//		�����ں���:
	CINI*		_ini;
	CAudio*		_audio;
};
