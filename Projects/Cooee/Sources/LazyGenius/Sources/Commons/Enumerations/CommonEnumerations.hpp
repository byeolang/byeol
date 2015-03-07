#pragma once

namespace LG
{
	enum COLOR
	{		
		BLACK=0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE, UNKNOWN
	};
	enum SPECIAL_KEY
	{
		//	�Է¿� Ű:
		ENTER		= 13,
		SPACE		= 32,
		BACKSPACE	= 8,
		TAB			= 9,
		ESCAPE		= 27,
		UP			= 72,
		DOWN		= 80,
		LEFT		= 75,
		RIGHT		= 77,
		INSERT		= 82,
		DEL			= 83,
		CTRL_D		= 4,
		CTRL_V		= 22,
		CTRL_X		= 24,
		HOME		= 71,		
		//	�ʼ� Ű:		
		CONFIRM		= ENTER,	//	Ȯ��
		CANCEL		= BACKSPACE,	//	��� / �޴�Ű
		//	���� Ű:
		//		����Ű:
		CLOSE		= ESCAPE,
		MAP			= HOME,	
		ADD			= INSERT,
		REMOVE		= DEL,
		COPY		= CTRL_D,
		PASTE		= CTRL_V,
		CUT			= CTRL_X,
		HELP		= '?',
		//		�߰�Ű:
		COMMAND		= '~'
	};
}