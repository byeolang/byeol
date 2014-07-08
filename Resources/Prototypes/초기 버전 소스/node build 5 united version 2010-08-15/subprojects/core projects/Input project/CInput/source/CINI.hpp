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
//		���漱��:
 class CInputNodeSample; // #��ǲ������#�߰�#

//	����:
//		Ŭ����:
class CINI
{
//	��� �Լ�:
public:
	//	������:
	CINI();
	//	�Ҹ���:
	~CINI();

	SHORT	getAnalogstickDeadzone()
	{
		return _analog_stick_deadzone;
	}
	void	setAnalogstickDeadzone(float deadzone_percentage)
	{
		_analog_stick_deadzone = (SHORT) (deadzone_percentage * float(0x7FFF));
	}
	SHORT	getAnalogstickMaxDeadzone()
	{
		return _analog_stick_max_deadzone;
	}
	void	setAnalogstickMaxDeadzone(float deadzone_percentage)
	{
		_analog_stick_max_deadzone = (SHORT) (deadzone_percentage * float(0x7FFF));
	}
	SHORT	getAnalogstickToButtonDeadzone()
	{
		return _analog_stick_to_button_deadzone;
	}
	void	setAnalogstickToButtonDeadzone(float deadzone_percentage)
	{
		_analog_stick_to_button_deadzone = (SHORT) (deadzone_percentage * float(0x7FFF));
	}
	BYTE	getTriggerDeadzone()
	{
		return _trigger_deadzone;
	}
	void	setTriggerDeadzone(BYTE deadzone) // max = 255
	{
		_trigger_deadzone = deadzone;
	}
	BYTE	getTriggerToButtonDeadzone()
	{
		return _trigger_to_button_deadzone;
	}
	void	setTriggerToButtonDeadzone(BYTE deadzone)
	{
		_trigger_to_button_deadzone = deadzone;
	}	

//	CInput:
public:
	//	NodeSamplePreset: // #��ǲ������#�߰�#
	CInputNodeSample*	_sample;
	HWND				_hWnd;
	bool				_window_activate;
	RECT				_client_rect;

//	���κ���:
private:
	//		ȭ�� ����:
	WINDOW_RATIO	_graphic_ratio;
	SHORT			_analog_stick_deadzone;
	SHORT			_analog_stick_max_deadzone; // �Ƴ��α� ��ƽ�� ������, deadzone ~ max deadzone ������ �ȴ�.
	SHORT			_analog_stick_to_button_deadzone;	
	BYTE			_trigger_deadzone;
	BYTE			_trigger_to_button_deadzone;
};

extern CINI g_ini;