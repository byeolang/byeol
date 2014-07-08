#include "CWindow.hpp"
#include "CInput.hpp"
#include "CError.hpp"

#include <iostream>
using namespace std;

//	CHUB�� �������߿� CError �޼����� ĳġ�ؾ��ϴµ�... CError�� ������ �ڵ��� CHUB�κ��� �������,
//	���� ������������ CHUB�κ��� �������ڵ��� ���� �� ���� (������ �ڵ��� �޸𸮿� ����������..)
//	�׷��� ��¿������ ������ �ڵ��� ��������(����ƽ)�� �Ѵ�.

CWindow::CWindow(HINSTANCE hInstance, int nCmdShow, CInput* input) 
: _hInstance(hInstance), _nCmdShow(nCmdShow), _input(input)
{	
	//	�ʱ�ȭ:
	isAlreadyExcuted();
	initializeWindowClass();
	initializeWindow();
	setClientRect();
}
//		�Ҹ���:
CWindow::~CWindow()
{
	
}
//	������ Ŭ������ �����ϰ� ����Ѵ�.
void	CWindow::initializeWindowClass()
{
	WNDCLASS wc = 
	{
		CS_VREDRAW|CS_HREDRAW|CS_OWNDC/*��Ÿ��*/, WndProc,
		0, 0, _hInstance, NULL/*������*/, NULL/*Ŀ��*/, 
		(HBRUSH) 2/*����*/, NULL, DEFAULT_CWINDOW_WINDOW_CLASSNAME
	};
	RegisterClass(&wc);
	
}
//	�����찴ü�� �����Ѵ�
void	CWindow::initializeWindow()
{
	//	������ ����:
	_hWnd = CreateWindow( DEFAULT_CWINDOW_WINDOW_CLASSNAME, DEFAULT_CWINDOW_WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,  DEFAULT_CWINDOW_WINDOW_X,  DEFAULT_CWINDOW_WINDOW_Y, 
		100, 100, NULL,NULL,
		_hInstance, NULL);	

	g_ini._hWnd = _hWnd;

	//	�����츦 show:
	ShowWindow(_hWnd, _nCmdShow);
	UpdateWindow(_hWnd);
}
//	��������ü�� �ƴ�, �׸��� ��µǴ� Ŭ���̾�Ʈ������, ini ������ �ʺ� �ǰ��Ѵ�
void	CWindow::setClientRect()
{
	RECT crt;
	DWORD style, ex_style;
	SetRect(&crt, 0, 0, 800, 600); // RECT�� ����ü�̱⿡ ���Լ��� �ʱ�ȭ��Ų��

	style = GetWindowLong(_hWnd,GWL_STYLE);
	ex_style = GetWindowLong(_hWnd, GWL_EXSTYLE);
	AdjustWindowRectEx(&crt, style, GetMenu(_hWnd) != NULL, ex_style);
	if (style & WS_VSCROLL)
		crt.right += GetSystemMetrics(SM_CXVSCROLL);
	if (style & WS_HSCROLL)
		crt.bottom += GetSystemMetrics(SM_CYVSCROLL);

	SetWindowPos(_hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);		
}
//	�޼����� �ٷ��. ���ѷ���.
void	CWindow::HandleMessage()
{
	//	�޼����� �����ߴ��� Ȯ��(peek = ������)
	//	getMessage�� ����ϸ�, �޼����� �����Ҷ��� �������� ��
	while ( _msg.message != WM_QUIT )
	{
		if ( PeekMessage(&_msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			//	�����ϸ� �޼���ó��
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		else
		{
			_input->excute();
			//	�� �������� ��� �� ����Ѵ�
			//if (GetAsyncKeyState(VK_F11) & 0x0001)
			//{
			//	_audio->unpauseSound(); // musicmono
			//}			
	
			static bool	press_once = false;

			if (_input->_sample._ok.getState() == DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN)
			{
				//#pragma message("\nok:down")
				//OutputDebugString(_T("\nok:down"));
				press_once = false;
			}
			if (_input->_sample._ok.getState() == DEFAULT_CINPUTBUTTON_STATE_BUTTON_UP)
			{
				//OutputDebugString(_T("\nok:up"));
				press_once = false;
			}
			static USHORT level = 10000;
			if (_input->_sample._ok.getState() == DEFAULT_CINPUTBUTTON_STATE_BUTTON_PRESS_ON)
			{
				if (!press_once)
				{
					//OutputDebugString(_T("\nok:pressing"));
					press_once = true;
				}				

				//level += 300;
				//if (level > 65000)
					level = 65000;
				_input->_console_xbox360pad.setBigMotor(0, level, 500);
			}
			if (GetAsyncKeyState(VK_L))
			{
				_input->_console_xbox360pad.setBigMotor(0, level, 500);
			}
			if (_input->_sample._cancle.getState() == DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN)
			{
				OutputDebugString(_T("\n\tsetting_mode . . ."));
				_input->inputLabel(&_input->_sample._ok, 0);				
				level = 10000;
			}
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				SendMessage(_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL);
			}
			
			//	���콺��ǥ
			static	int x;
			static	int y;
			
			if (	x != _input->_sample._cursor.getRealCursorX() ||
					y != _input->_sample._cursor.getRealCursorY()  )
			{
				x = _input->_sample._cursor.getRealCursorX();
				y = _input->_sample._cursor.getRealCursorY();		

				TCHAR buffer[256];
				_stprintf( buffer, _T("\n%d, %d"), x, y  );			
				OutputDebugString(buffer);
			}
			if ( (_input->_sample._power.getPressLevel()) != 0 )
			{
				TCHAR buffer[256];
				_stprintf( buffer, _T("\npower %d"), _input->_sample._power.getPressLevel());			
				OutputDebugString(buffer);
			}
		}
	}
}
//	�ߺ����� ����
void	CWindow::isAlreadyExcuted()
{
	//	���α׷� �̸��� ������ �����찡 �̹� �ִ��� �˻�:
	HWND hWnd = FindWindow(NULL,  DEFAULT_CWINDOW_WINDOW_NAME);

	if (hWnd != NULL)
	{
		SetForegroundWindow(hWnd);
		ShowWindow(hWnd, SW_RESTORE);
		//	�� APP�� ����Ǿ��ϱ� ������ ������ throw�Ѵ�
		throw CError 
		(	
			_T("����00:\t�ߺ���������, ���α׷��� �����մϴ�."), 
			_T("�� ������ ��ϵ����ʽ��ϴ�."),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
		);
	}
}