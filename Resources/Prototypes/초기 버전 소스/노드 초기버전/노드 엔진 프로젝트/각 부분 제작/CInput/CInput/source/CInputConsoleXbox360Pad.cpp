#include "CInputConsoleXbox360Pad.hpp"

CInputConsoleXbox360Pad::CInputConsoleXbox360Pad() : _enable(false)
{
	for(int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
	{
		_pad[n]._bConnected = false;
		_pad[n]._state.dwPacketNumber = NULL;
		_pad[n]._state.Gamepad.bLeftTrigger = NULL;
		_pad[n]._state.Gamepad.bRightTrigger = NULL;
		_pad[n]._state.Gamepad.sThumbLX = NULL;
		_pad[n]._state.Gamepad.sThumbLY = NULL;
		_pad[n]._state.Gamepad.sThumbRX = NULL;
		_pad[n]._state.Gamepad.sThumbRY = NULL;
		_pad[n]._state.Gamepad.wButtons = NULL;

		_pad[n]._vibration.wLeftMotorSpeed = 0;
		_pad[n]._vibration.wRightMotorSpeed = 0;

		_pad[n]._big_vibration_end_time = 0;
		_pad[n]._small_vibration_end_time = 0;
	}
}
CInputConsoleXbox360Pad::~CInputConsoleXbox360Pad()
{}
void	CInputConsoleXbox360Pad::disable()
{
	XInputEnable(false);
}
void	CInputConsoleXbox360Pad::enable()
{
	XInputEnable(true);
}
void	CInputConsoleXbox360Pad::update()
{
	DWORD dwResult;
	for(DWORD n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS; n++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(n, &_pad[n]._state);

		if(dwResult == ERROR_SUCCESS)
			_pad[n]._bConnected = true;
		else
			_pad[n]._bConnected = false;
	}

	setDeadzone();

	return ;
} 
XINPUT_GAMEPAD*	CInputConsoleXbox360Pad::getStateGamepad(int index) // ���� �߻���, ���� ��ȯ��
{
	//	���ܻ�Ȳó��:
	if (	index < 0												||
			index >= DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS)
	{
		CError e
		(	
			_T("���XX:\t�߸��� �����е��ȣ�� �����Ϸ� �߽��ϴ�."), 
			_T(""),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
		);
		e.log();
		return NULL;
	}
	if (!isJoypadPlayerConnected(index)) // log�� ����ϸ�, �α׸޼����� �ʹ� ���� ����Ƿ� (�����е尡 ������, �� �����Ӹ��� �����) �����Ѵ�.
		return NULL;

	//	�����ڵ�:
	return &(_pad[index]._state.Gamepad);
}
bool	CInputConsoleXbox360Pad::isJoypadPlayerConnected(int index)
{
	//	���ܻ���ó��:
	if (	index < 0													||
			index >= DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS	)
	{
		CError e
		(	
			_T("���XX:\t�߸��� �����е��ȣ�� �����Ϸ� �߽��ϴ�."), 
			_T(""),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
		);
		e.log();
		return false;
	}

	//	�����ڵ�:
	return _pad[index]._bConnected;
}
void	CInputConsoleXbox360Pad::setDeadzone()
{


	//	�����ڵ�:
	for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
		setDeadzoneOnePad(n);
}
void	CInputConsoleXbox360Pad::setDeadzoneOnePad(int index)
{
	//	���ܻ�Ȳó��:
	if (	index < 0													||
			index >= DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS	)
	{
		CError e
			(	
			_T("���XX:\t�߸��� �����е��ȣ�� �����Ϸ� �߽��ϴ�."), 
			_T(""),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
			);
		e.log();
		return;
	}

	//	�����ڵ�:
	setAnalogstickDeadzone(index);
	setTriggerDeadzone(index);
}