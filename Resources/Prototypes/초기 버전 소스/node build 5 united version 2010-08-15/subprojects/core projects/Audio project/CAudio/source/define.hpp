/*
	���ϸ�:	define.hpp
	�̸�:	NODE�� ����Ʈ
	���:	���� NODE���� ���Ǵ� ��� ����Ʈ���� ����ִ�. ��� �κп����� �����Ӱ� include�ؼ� ����ϸ� �ȴ�.
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//			CHUB:
#define DEFAULT_CHUB_TIMER_PERIOD							1
//			CWindow:
#define DEFAULT_CWINDOW_WINDOW_X							100
#define DEFAULT_CWINDOW_WINDOW_Y							100
#define DEFAULT_CWINDOW_WINDOW_NAME							_T("DirectX 9 Tutorial 2 - Triangle")
#define DEFAULT_CWINDOW_WINDOW_CLASSNAME					_T("DX9_TUTORIAL2_CLASS")
//			CINI:
//#define DEFAULT_CINI_AUDIO_WIDTH							640
//			CError:
#define	DEFAULT_CERROR_MESSAGEBOX_COMMENT_TOP				_T("������ ���� ������ �߻���, error.log�� ��� �߽��ϴ�.\n\n")
#define DEFAULT_CERROR_MESSAGEBOX_COMMENT_BOTTOM			_T("\n\n�ذ����� �𸦰�쿣, Ȩ�������� ����ʽÿ�.")
//			ȭ�����:
typedef enum enum_window_ratio
{
	RATIO_4_3	= 0, // 1200x900
	RATIO_16_9	= 1, // 1600x900
	RATIO_16_10	= 2 // 1440x900
} WINDOW_RATIO;
typedef	enum enum_file_extention // #��������#�߰�#
{
	WAV,
	OGG,
	NOT_SUPPORT_EXTENTION
} FILE_EXTENTION;
