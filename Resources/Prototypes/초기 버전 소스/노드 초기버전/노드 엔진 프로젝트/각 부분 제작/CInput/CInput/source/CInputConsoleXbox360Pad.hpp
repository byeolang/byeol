#pragma once

#include <Windows.h>
#include <XInput.h>
#include <MMSystem.h>
#include "define.hpp"
#include "CINI.hpp"

class CInputConsoleXbox360Pad
{
public:
	CInputConsoleXbox360Pad();
	~CInputConsoleXbox360Pad();
	void	disable();
	void	enable();
	void	update();
	XINPUT_GAMEPAD*	getStateGamepad(int index);
	bool	isJoypadPlayerConnected(int index);
	void	setBigMotor(int joypad_number, USHORT level, DWORD duration_miliseconds)
	{
		//	���ܻ�Ȳó��:
		if (!isJoypadPlayerConnected(joypad_number))
			return;

		//	�����ڵ�:
		_pad[joypad_number]._vibration.wLeftMotorSpeed = level;
		_pad[joypad_number]._big_vibration_end_time = timeGetTime() + duration_miliseconds;
	}
	void	setSmallMotor(int joypad_number, USHORT level, DWORD duration_miliseconds)
	{
		//	���ܻ�Ȳó��:
		if (!isJoypadPlayerConnected(joypad_number))
			return;

		//	�����ڵ�:
		_pad[joypad_number]._vibration.wRightMotorSpeed = level;
		_pad[joypad_number]._small_vibration_end_time = timeGetTime() + duration_miliseconds;
	}
	void	setWholeSmallMotor(USHORT level, DWORD duration_miliseconds)
	{
		for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
			setSmallMotor(n, level, duration_miliseconds);
	}
	void	setWholeBigMotor(USHORT level, DWORD duration_miliseconds)
	{
		for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
			setBigMotor(n, level, duration_miliseconds);
	}
	void	vibrate()
	{
		for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
			if (isJoypadPlayerConnected(n))
				vibrateOneJoypad(n);
	}	

private:
	void	vibrateOneJoypad(int joypad_number)
	{
		applyVibration(joypad_number);		
		XInputSetState(joypad_number, &_pad[joypad_number]._vibration);
	}
	void	applyVibration(int joypad_number)
	{	
		DWORD end_time = timeGetTime();

		checkBigMotor(joypad_number, end_time);
		checkSmallMotor(joypad_number, end_time);
	}
	void	checkBigMotor(int joypad_number, DWORD end_time)
	{	
		//	���ܻ�Ȳó��:
		if (_pad[joypad_number]._vibration.wLeftMotorSpeed <= 0)
			return;

		//	�����ڵ�:
		if (_pad[joypad_number]._big_vibration_end_time <= end_time)
		{
			_pad[joypad_number]._big_vibration_end_time = 0;
			_pad[joypad_number]._vibration.wLeftMotorSpeed = 0;
		}
	}
	void	checkSmallMotor(int joypad_number, DWORD end_time)	
	{	
		//	���ܻ�Ȳó��:
		if (_pad[joypad_number]._vibration.wRightMotorSpeed <= 0)
			return;

		//	�����ڵ�:
		if (_pad[joypad_number]._small_vibration_end_time <= end_time)
		{
			_pad[joypad_number]._small_vibration_end_time = 0;
			_pad[joypad_number]._vibration.wRightMotorSpeed = 0;
		}
	}
	void	setDeadzone();
	void	setDeadzoneOnePad(int index);
	void	setAnalogstickDeadzone(int index)
	{
		//	���ܻ�Ȳó��:
		if (g_ini.getAnalogstickDeadzone() <= 0.0f) 
			return; // ����Ʈ�� "������"���� �Ǿ�������. ������ �ƴϴ�.

		//	�����ڵ�:
		//		min:
		//	���� �Ƴ��α� ��ƽ
		if (	( _pad[index]._state.Gamepad.sThumbLX < g_ini.getAnalogstickDeadzone() && _pad[index]._state.Gamepad.sThumbLX > -(g_ini.getAnalogstickDeadzone()) ) &&
				( _pad[index]._state.Gamepad.sThumbLY < g_ini.getAnalogstickDeadzone() && _pad[index]._state.Gamepad.sThumbLY > -(g_ini.getAnalogstickDeadzone()) ) )
		{
			_pad[index]._state.Gamepad.sThumbLX = 0;
			_pad[index]._state.Gamepad.sThumbLY = 0;
		}
		//	������ �Ƴ��α� ��ƽ
		if (	( _pad[index]._state.Gamepad.sThumbRX < g_ini.getAnalogstickDeadzone() && _pad[index]._state.Gamepad.sThumbRX > -(g_ini.getAnalogstickDeadzone()) ) &&
				( _pad[index]._state.Gamepad.sThumbRY < g_ini.getAnalogstickDeadzone() && _pad[index]._state.Gamepad.sThumbRY > -(g_ini.getAnalogstickDeadzone()) ) )
		{
			_pad[index]._state.Gamepad.sThumbRX = 0;
			_pad[index]._state.Gamepad.sThumbRY = 0;
		}
		//		max:
		//	���� �Ƴ��α� ��ƽ
		//		X��
		if (_pad[index]._state.Gamepad.sThumbLX > g_ini.getAnalogstickMaxDeadzone())
			_pad[index]._state.Gamepad.sThumbLX = g_ini.getAnalogstickMaxDeadzone();
		else if (_pad[index]._state.Gamepad.sThumbLX < -(g_ini.getAnalogstickMaxDeadzone()))
			_pad[index]._state.Gamepad.sThumbLX = -(g_ini.getAnalogstickMaxDeadzone());
		//		Y��
		if (_pad[index]._state.Gamepad.sThumbLY > g_ini.getAnalogstickMaxDeadzone())
			_pad[index]._state.Gamepad.sThumbLY = g_ini.getAnalogstickMaxDeadzone();
		else if (_pad[index]._state.Gamepad.sThumbLY < -(g_ini.getAnalogstickMaxDeadzone()))
			_pad[index]._state.Gamepad.sThumbLY = -(g_ini.getAnalogstickMaxDeadzone());			
		//	������ �Ƴ��α� ��ƽ
		//		X��
		if (_pad[index]._state.Gamepad.sThumbRX > g_ini.getAnalogstickMaxDeadzone())
			_pad[index]._state.Gamepad.sThumbRX = g_ini.getAnalogstickMaxDeadzone();
		else if (_pad[index]._state.Gamepad.sThumbRX < -(g_ini.getAnalogstickMaxDeadzone()))
			_pad[index]._state.Gamepad.sThumbRX = -(g_ini.getAnalogstickMaxDeadzone());
		//		Y��
		if (_pad[index]._state.Gamepad.sThumbRY > g_ini.getAnalogstickMaxDeadzone())
			_pad[index]._state.Gamepad.sThumbRY = g_ini.getAnalogstickMaxDeadzone();
		else if (_pad[index]._state.Gamepad.sThumbRY < -(g_ini.getAnalogstickMaxDeadzone()))
			_pad[index]._state.Gamepad.sThumbRY = -(g_ini.getAnalogstickMaxDeadzone());			

	}
	void	setTriggerDeadzone(int index)
	{
		//	���ܻ�Ȳó��:
		if (g_ini.getTriggerDeadzone() <= 0) 
			return; // ����Ʈ�� "������"���� �Ǿ�������. ������ �ƴϴ�.

		//	�����ڵ�:
		//	���� Ʈ����
		if (_pad[index]._state.Gamepad.bLeftTrigger < g_ini.getTriggerDeadzone())
			_pad[index]._state.Gamepad.bLeftTrigger = 0;			
		//	������ Ʈ����
		if (_pad[index]._state.Gamepad.bRightTrigger < g_ini.getTriggerDeadzone())
			_pad[index]._state.Gamepad.bRightTrigger = 0;
	}


public:
	bool	_enable;
	bool	_deadzone_enable;

private:
	CONTROLLER_STATE	_pad[DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS];
};