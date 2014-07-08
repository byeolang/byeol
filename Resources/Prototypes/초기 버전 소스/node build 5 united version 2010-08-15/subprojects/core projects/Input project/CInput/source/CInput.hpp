#pragma once

#include <Windows.h>
#include "CINI.hpp"
#include "CInputInterface.hpp"
//	#��ǲ������#�߰�#
#include "CInputNodeSample.hpp"
//	#�ܼ��Է�#�߰�#
#include "CInputConsoleKeyboardAndMouse.hpp"
#include "CInputConsoleXbox360Pad.hpp"


class CInput
{
public:
	CInput() : _activated(true), _setting_interface(NULL), _setting_interface_index(0)
	{
		_sample = *(g_ini._sample);	
	}
	~CInput()
	{}
	
	void	excute()
	{
		activate();
		if (_setting_interface)
			setting_mode();
		else
			update();
	}
	void	inputLabel(CInputInterface* input_interface, int input_interface_index=0) // input_interface_index = ����Ű? ����Ű?
	{
		//	���ܻ�Ȳó��:
		if (!input_interface)
			return ;

		//	�����ڵ�:
		initializeState();
		_setting_interface = input_interface;
		_setting_interface_index = input_interface_index;
	}
	void	initializeState() // ��� ��ư, ��ƽ��.. �������̽��� ���¸� �ʱ�ȭ �Ѵ�.
	{
		_sample.initializeState();
	}

private:
	void	activate()
	{
		if (g_ini._window_activate)
		{
			if (!_activated)
			{
				_console_xbox360pad.enable();
				_activated = true;
			}
		}
		else
		{	
			if (_activated)
			{
				_console_xbox360pad.disable();
				_activated = false;
			}
		}
	}
	void	setting_mode()
	{
		updateConsole();
		CInputInterfaceSet new_binding_key = _setting_interface->inputLabel(&_console_keyboard_mouse, &_console_xbox360pad);

		if (new_binding_key.getLabel()) // 0���� ũ��, Ű �νĿ� �����ߴٴ� ����
		{
			OutputDebugString(_T("\n\tsetting_mode . . . quit"));
			*(_setting_interface->getInterfaceSet(_setting_interface_index)) = new_binding_key;
			_setting_interface = NULL;
			_setting_interface_index = 0;
		}
	}
	void	update()
	{
		updateConsole();		
		updateInterfacePreset();
		_console_xbox360pad.vibrate();
	}
	void	updateInterfacePreset()
	{
		//	#��ǲ������#�߰�#
		_sample.update(&_console_keyboard_mouse, &_console_xbox360pad); // #�ܼ��Է�#�߰�#
	}
	void	updateConsole()
	{
		//	#�ܼ��Է�#�߰�#
		_console_xbox360pad.update();
	}

public:
	//	#��ǲ������#�߰�#
	CInputNodeSample	_sample;
	//	#�ܼ��Է�#�߰�#
	CInputConsoleKeyboardAndMouse	_console_keyboard_mouse;	
	CInputConsoleXbox360Pad			_console_xbox360pad;

private:
	bool							_activated;
	CInputInterface*				_setting_interface; // ���� �¸�� ���� �������̽�. ��(0)�� �ƴҰ���, �ٸ� �Է��� ��� �����ϰ� ���ÿ��� �����ϴ� �¸�尡 �ȴ�.
	int								_setting_interface_index;
};