#include "CInputButton.hpp"

CInputButton::CInputButton() : CInputInterface(), _before_state(NULL), _state(DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED)
{

}
CInputButton::~CInputButton()
{

}

CInputButton&	CInputButton::operator = (CInputButton& rhs)
{
	_before_state = NULL;
	_state = DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED;

	CInputInterface::operator = (rhs);
	return *this;
}
BYTE	CInputButton::getState()
{
	return _state;
}

void	CInputButton::updateKeyboardMouse(CInputConsoleKeyboardAndMouse* keyboard_mouse, int index)
{
	WORD	now_state = keyboard_mouse->getState(_interface_set[index].getLabel());


	if (	(!_before_state)	&& // up event: bef=0, now=0x8000
			(now_state)			)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_UP;		
	else if (	(_before_state)	&& // press: bef = 0x8000, now 0x8000
		(now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_PRESS_ON;
	else if (	(_before_state)	&& // down: bef = 0x8000, now = 0x0000
		(!now_state)	)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN;
	else if (	(!_before_state)	&& // not_pressed
		(!now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED;

	_before_state = now_state;
}
void	CInputButton::updateXbox360Pad(CInputConsoleXbox360Pad* xbox360pad, int index) // �Ѱܹ��� �ϳ��� Ű�� ������.
{
	//	���ܻ�Ȳó��:
	XINPUT_GAMEPAD*	gamepad_state = xbox360pad->getStateGamepad(_interface_set[index].getJoypadPlayer());	
	if (!gamepad_state)
		return; // �����е��� ���¸� ��µ� �����ߴ�.

	//	�����ڵ�:
	if ( isStick(_interface_set[index].getLabel()) ) // �����е��÷��̾� index�� �Ƴ��α׽�ƽ�ΰ�?
		getStickState(gamepad_state, index);
	else if ( isTrigger(_interface_set[index].getLabel()) ) 
		getTriggerState(gamepad_state, index);
	else 
		getButtonState(gamepad_state->wButtons, index);	
}

void	CInputButton::getButtonState(WORD button_state, int index)
{
	WORD	now_state = getButtonNowState(button_state, _interface_set[index].getLabel());

	if (	(!_before_state)	&& // up event: bef=0, now=0x8000
			(now_state)			)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_UP;		
	else if (	(_before_state)	&& // press: bef = 0x8000, now 0x8000
		(now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_PRESS_ON;
	else if (	(_before_state)	&& // down: bef = 0x8000, now = 0x0000
		(!now_state)	)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN;
	else if (	(!_before_state)	&& // not_pressed
		(!now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED;

	_before_state = now_state;
}
WORD	CInputButton::getButtonNowState(WORD button_state, WORD lable)
{
	WORD target = NULL;		

	if (lable == VK_PAD_A)
		target = XINPUT_GAMEPAD_A;

	else if (lable == VK_PAD_B)
		target = XINPUT_GAMEPAD_B;

	else if (lable == VK_PAD_X)
		target = XINPUT_GAMEPAD_X;

	else if (lable == VK_PAD_Y)
		target = XINPUT_GAMEPAD_Y;

	else if (lable == VK_PAD_LSHOULDER)
		target = XINPUT_GAMEPAD_LEFT_SHOULDER;

	else if (lable == VK_PAD_RSHOULDER)
		target = XINPUT_GAMEPAD_RIGHT_SHOULDER;

	else if (lable == VK_PAD_DPAD_UP)
		target = XINPUT_GAMEPAD_DPAD_UP;

	else if (lable == VK_PAD_DPAD_LEFT)
		target = XINPUT_GAMEPAD_DPAD_LEFT;

	else if (lable == VK_PAD_DPAD_RIGHT)
		target = XINPUT_GAMEPAD_DPAD_RIGHT;

	else if (lable == VK_PAD_DPAD_DOWN)
		target = XINPUT_GAMEPAD_DPAD_DOWN;

	else if (lable == VK_PAD_START)
		target = XINPUT_GAMEPAD_START;

	else if (lable == VK_PAD_BACK)
		target = XINPUT_GAMEPAD_BACK;

	else if (lable == VK_PAD_LTHUMB_PRESS)
		target = XINPUT_GAMEPAD_LEFT_THUMB;

	else if (lable == VK_PAD_RTHUMB_PRESS)
		target = XINPUT_GAMEPAD_RIGHT_THUMB;

	if (button_state & target)
		return 1;
	else
		return 0;
}
void	CInputButton::getStickState(XINPUT_GAMEPAD* stick, int index) // �Ƴ��α� ������ ��ư�� ���������� ġȯ�Ѵ�.
{
	WORD now_state = getStickNowState(stick, _interface_set[index].getLabel());

	if (	(!_before_state)	&& // up event: bef=0, now=0x8000
			(now_state)			)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_UP;		
	else if (	(_before_state)	&& // press: bef = 0x8000, now 0x8000
				(now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_PRESS_ON;
	else if (	(_before_state)	&& // down: bef = 0x8000, now = 0x0000
				(!now_state)	)
		_state = DEFAULT_CINPUTBUTTON_STATE_BUTTON_DOWN;
	else if (	(!_before_state)	&& // not_pressed
				(!now_state)		)
		_state = DEFAULT_CINPUTBUTTON_STATE_NOT_PRESSED;

	_before_state = now_state;
}
WORD	CInputButton::getStickNowState(XINPUT_GAMEPAD* stick, WORD lable)
{
	if (lable == VK_PAD_LTHUMB_DOWN)
	{
		if (stick->sThumbLY < -(g_ini.getAnalogstickToButtonDeadzone()))
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_LTHUMB_UP)
	{
		if (stick->sThumbLY > g_ini.getAnalogstickToButtonDeadzone())
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_LTHUMB_LEFT)
	{
		if (stick->sThumbLX < -(g_ini.getAnalogstickToButtonDeadzone()))
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_LTHUMB_RIGHT)
	{
		if (stick->sThumbLX > g_ini.getAnalogstickToButtonDeadzone())
			return 1; // press
		else	
			return 0;
	}
	if (lable == VK_PAD_RTHUMB_DOWN)
	{
		if (stick->sThumbRY < -(g_ini.getAnalogstickToButtonDeadzone()))
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_RTHUMB_UP)
	{
		if (stick->sThumbRY > g_ini.getAnalogstickToButtonDeadzone())
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_RTHUMB_LEFT)
	{
		if (stick->sThumbRX < -(g_ini.getAnalogstickToButtonDeadzone()))
			return 1; // press
		else	
			return 0;
	}
	else if (lable == VK_PAD_RTHUMB_RIGHT)
	{
		if (stick->sThumbRX > g_ini.getAnalogstickToButtonDeadzone())
			return 1; // press
		else	
			return 0;
	}
	else
		return 0;
}
