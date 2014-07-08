#include "CWindow.hpp"
#include "CError.hpp"

//	CHUB�� �������߿� CError �޼����� ĳġ�ؾ��ϴµ�... CError�� ������ �ڵ��� CHUB�κ��� �������,
//	���� ������������ CHUB�κ��� �������ڵ��� ���� �� ���� (������ �ڵ��� �޸𸮿� ����������..)
//	�׷��� ��¿������ ������ �ڵ��� ��������(����ƽ)�� �Ѵ�.
HWND	CWindow::_hWnd = 0;

CWindow::CWindow(HINSTANCE hInstance, int nCmdShow, CINI* ini, CAudio* audio) 
: _hInstance(hInstance), _nCmdShow(nCmdShow), _ini(ini), _audio(audio)
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
			//	�� �������� ��� �� ����Ѵ�

			if (GetAsyncKeyState(VK_F1) & 0x0001)
			{
				_audio->playMusic(0); // musicmono
			}
			if (GetAsyncKeyState(VK_F2) & 0x0001)
			{
				_audio->pauseMusic(); // musicmono
			}
			if (GetAsyncKeyState(VK_F3) & 0x0001)
			{
				_audio->unpauseMusic(); // musicmono
			}
			if (GetAsyncKeyState(VK_F4) & 0x0001)
			{
				_audio->setMusicVolume(0.6f); // musicmono
			}
			if (GetAsyncKeyState(VK_F5) & 0x0001)
			{
				_audio->replayMusic(); // musicmono
			}
			if (GetAsyncKeyState(VK_F7) & 0x0001)
			{
				_audio->playSound(0); // musicmono
			}
			if (GetAsyncKeyState(VK_F8) & 0x0001)
			{
				_audio->playSound(1); // musicmono
			}
			if (GetAsyncKeyState(VK_F9) & 0x0001)
			{
				_audio->pauseSound(); // musicmono
			}
			if (GetAsyncKeyState(VK_F11) & 0x0001)
			{
				_audio->unpauseSound(); // musicmono
			}
			if (GetAsyncKeyState(VK_F12) & 0x0001)
			{
				_audio->setSoundVolume(0.5); // musicmono
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