/*
	���ϸ�:	define.hpp
	�̸�:	NODE�� ����Ʈ
	���:	���� NODE���� ���Ǵ� ��� ����Ʈ���� ����ִ�. ��� �κп����� �����Ӱ� include�ؼ� ����ϸ� �ȴ�.
*/
#pragma once
//	����Ʈ���̺귯��:
#pragma comment(lib, "xinput.lib")
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
#include <XInput.h>
//			CHUB:
#define DEFAULT_CHUB_TIMER_PERIOD							1
//			CWindow:
#define DEFAULT_CWINDOW_WINDOW_X							100
#define DEFAULT_CWINDOW_WINDOW_Y							100
#define DEFAULT_CWINDOW_WINDOW_NAME							_T("DirectX 9 Tutorial 2 - Triangle")
#define DEFAULT_CWINDOW_WINDOW_CLASSNAME					_T("DX9_TUTORIAL2_CLASS")
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


#define DEFAULT_CINPUTINTERFACESET_MAX_KEYSET	2 // �ϴ� �׽�Ʈ�� ���ؼ� �� Ű���� 2������ (����Ű + ����Ű = fps ��Ÿ��)

#define DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED		0
#define DEFAULT_CINPUTBUTTON_STATE_BUTTON_UP		1
#define DEFAULT_CINPUTBUTTON_STATE_BUTTON_PRESS_ON	2
#define DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN		3

#define VK_0	0x30
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39

#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4A
#define VK_K	0x4B
#define VK_L	0x4C
#define VK_M	0x4D
#define VK_N	0x4E
#define VK_O	0x4F
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5A

#define VK_PAD_LTHUMB					0x5828 // CInputStick�� ����Ѵ�. �̰�, ���� �Ƴ��α����� �˸��� ǥ�ÿ� �Ұ��ϴ�.
#define VK_PAD_RTHUMB					0x5829

#define DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS					1
#define	DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_DEADZONE			0.2f // 0.0f�����ϸ�, �⺻������ �������� ������� �ʴ´�.
#define DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_MAX_DEADZONE		0.75f // CInputStick�� ���, �밢���� 20000/20000�����ۿ� �ȵǹǷ�, Ŀ���� �׻� ���� �׸��� �ȴ�. �׷��� �ƽ��������� ����
#define	DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_TO_BUTTON_DEADZONE	0.5f // 0.0f�����ϸ�, �⺻������ �������� ������� �ʴ´�. #����# �밢���� ���, 32500/32500 �� �ƴ϶� 20000/20000������ �����⶧���� �������� �ʹ� �ø��� �밢�� �Է��� �� ������

#define DEFAULT_CINPUTCONSOLEXBOX360PAD_TRIGGER_DEADZONE				0	// max = 255
#define	DEFAULT_CINPUTCONSOLEXBOX360PAD_TRIGGER_TO_BUTTON_DEADZONE		125
#define DEFAULT_CINPUTCONSOLEXBOX360PAD_LEFT_THUMB_DEADZONE				7849
#define DEFAULT_CINPUTCONSOLEXBOX360PAD_RIGHT_THUMB_DEADZONE			8689
#define DEFAULT_CINPUTCONSOLEXBOX360PAD_TRIGGER_THRESHOLD				30

struct CONTROLLER_STATE
{
	XINPUT_STATE		_state;
	bool				_bConnected;
	XINPUT_VIBRATION	_vibration;
	DWORD				_big_vibration_end_time; // ū ���� ������ ������ �ð�. 0�� �ƴ϶�� ������ �Ȱ��̴�.
	DWORD				_small_vibration_end_time; // ���� ����
};
