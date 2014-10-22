#pragma once

namespace LG
{
	enum COLOR
	{		
		BLACK=0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE, UNKNOWN
	};
	enum SPECIAL_KEY
	{
		//	�ʼ� Ű:
		UP			= 72,
		DOWN		= 80,
		LEFT		= 75,
		RIGHT		= 77,
		CONFIRM		= 'z',
		CANCEL		= 'x',	//	��� / �޴�Ű
		//	�Է¿� Ű:
		ENTER		= 13,
		SPACE		= 32,	
		BACKSPACE	= 8,	//	Backspace
		//	���� Ű:
		//		����Ű:
		CLOSE		= 27,	//	esc
		MAP			= 71,	//	home
		ADD			= 82,	//	ins
		REMOVE		= 83,	//	del			
		COPY		= 'c',
		PASTE		= 'v',
		CUT			= 'u',
		HELP		= '?',
		//		�߰�Ű:
		COMMAND		= '~'
	};
}