/*	
	��ƽ�̶�?
	0,0�� ��������, ��, ��, ��, ��, �밢���� 8���� ������ ��Ÿ���� �� �����ϴ�.
	�׸���, �� ������ ���������ؾ��ϸ�,
	�� ������ ������ ����, ������ǥ�� Ŀ���� ��ġ�� ��ȯ�ϰ� �ȴ�.
	��ǲ��: �ܼ� �Է� ��ġ (Ű����&���콺 || �����е�)
			����
	�ƿ�ǲ��: Ŀ���� ��ġ
*/
#pragma once

#include "CInputInterface.hpp"
#include "CINI.hpp"
#include "define.hpp"
#include <XInput.h>

class CInputStick : public CInputInterface
{
public:
	CInputStick()
	{
		_cursor.x = 0;
		_cursor.y = 0;
	}
	~CInputStick()
	{}

	CInputStick&	operator = (CInputStick& rhs)
	{
		_cursor.x = 0;
		_cursor.y = 0;

		CInputInterface::operator =(rhs);
		return *this;
	}
	void	setCursor(int x, int y)
	{
		_cursor.x = x;
		_cursor.y = y;
	}
	int		getRealCursorX() // ������ǥ���� Ŀ��
	{
		return _cursor.x;
	}
	int		getRealCursorY()
	{
		return _cursor.y;
	}
	// ���ӿ��� ���Ǵ� Ŀ����ǥ. ī�޶��� �ܰ� �����̵���ǥ��, ���ӻ󿡼� Ŀ���� ��ǥ�� ����Ѵ�.
	int		getCursorX(float camera_x = 0.0f, float camera_y = 0.0f, float camera_zoom = 1.0f)
	{
		return 0;
	}
	int		getCursorY(float camera_x = 0.0f, float camera_y = 0.0f, float camera_zoom = 1.0f)
	{
		return 0;
	}
	void	initializeState()
	{
		// #�۾�# y = g_ini.halfHeight;
	}
protected:
	virtual void	updateKeyboardMouse(CInputConsoleKeyboardAndMouse* keyboard_mouse, int index)
	{
		// ���⼭ ��üȭ������ üũ�Ѵ�. if (g_ini)
		// ��üȭ��� GetCursorPos(&_cursor); �� ����
		GetCursorPos(&_cursor);
		ScreenToClient(g_ini._hWnd, &_cursor); // �����츦 �Űܵ� �ڵ����� ���� ����
		// if (ini�κ��� Ŭ���̾�Ʈ �ʺ�� ���̸� ���ؼ� �װͺ��� ũ��, �ʺ�� ���̷� �����Ѵ�.)
		// WM_SIZE�� ���ϰ� �ϴ���... �ƴϸ�, WM_SIZE��, Ŭ���̾�Ʈ������ ini�� �����ϴ���.
		setMouseCursorDeadzone();
	}
	virtual	void	updateXbox360Pad(CInputConsoleXbox360Pad* xbox360pad, int index)
	{
		//	���ܻ�Ȳó��:
		XINPUT_GAMEPAD*	gamepad_state = xbox360pad->getStateGamepad(_interface_set[index].getJoypadPlayer());	
		if (!gamepad_state)
			return; // �����е��� ���¸� ��µ� �����ߴ�.
		if ( !isStick(_interface_set[index].getLabel()) ) // ��ƽ�� �ƴϴ�
			return; 

		POINT	tilt;
		tilt = getStickTilt(xbox360pad->getStateGamepad(_interface_set[index].getJoypadPlayer()), _interface_set[index].getLabel()); // ����� ��ƽ���� �Ǵ��ѵ�, �� ���� �����´�
		//	��ǥ���
		setStickCursorPos(tilt);
	}
	virtual	CInputInterfaceSet	checkKeyboardMouse(CInputConsoleKeyboardAndMouse* keyboard_mouse) // 0�� �ν� ����. WORD�� ������, _label�� WORD�̹Ƿ�
	{
		if (	GetAsyncKeyState(VK_LBUTTON)	|| // ���콺�� ���������� �ϸ�, ���콺�� �޴�����ٰ� �����е带 ���ø��ϴ� ��찡 ����� �ֱ⿡ ��/�� Ŭ���� �����. �׷��� ������, �̷��� ���� �ʰ� �޴��󿡼� ������ �����ϰ� �ǰ���.
				GetAsyncKeyState(VK_RBUTTON)	)
			return	CInputInterfaceSet(VK_LBUTTON, 0); // Ű���忡 �ش��ϴ� ���̺��̸� ���� ����.

		return CInputInterfaceSet();
	}
	virtual	CInputInterfaceSet	checkXbox360Pad(CInputConsoleXbox360Pad* xbox360pad) // 0�� �ν� ����. WORD�� ������, _label�� WORD�̹Ƿ�
	{
		for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
		{
			if (xbox360pad->isJoypadPlayerConnected(n))
			{
				if (	xbox360pad->getStateGamepad(n)->sThumbLX	||
						xbox360pad->getStateGamepad(n)->sThumbLY	||
						xbox360pad->getStateGamepad(n)->sThumbRX	||
						xbox360pad->getStateGamepad(n)->sThumbRY	)
					return	CInputInterfaceSet(VK_PAD_A, n); // �����е忡 �ش��ϴ� ���̺��̸� ���� ����.
			}
		}

		return CInputInterfaceSet();
	}
	POINT	getStickTilt(XINPUT_GAMEPAD* gamepad, WORD label)
	{
		POINT	container;
		container.x = 0;
		container.y = 0;

		if (isLeftStick(label))
		{
			container.x = (LONG) gamepad->sThumbLX;
			container.y = (LONG) -(gamepad->sThumbLY); // Y�� ��ȣ�� �ݴ�� �Ǿ��ִ�. �»��� ++ �ΰ�.
			return container;
		}
		else if (isRightStick(label))
		{
			container.x = (LONG) gamepad->sThumbRX;
			container.y = (LONG) -(gamepad->sThumbRY); 
			return container;
		}
		else
		{
			container.x = 0;
			container.y = 0;
			CError e
			(	
				_T("���XX:\tXBOX360�е�� ����, ������ 2���� �Ƴ��α� ��ƽ�ۿ� ���µ�, ����ü ���� ��ƽ�Դϱ�?"), 
				_T(""),
				_T(__FUNCTION__)				
			);
			e.log();

			return container;
		}
	}
	bool	isLeftStick(WORD label)
	{
		if (	label >= VK_PAD_LTHUMB_UP	&&
				label <= VK_PAD_LTHUMB		)
			return true;
		else
			return false;
	}
	bool	isRightStick(WORD label)
	{
		if (	label >= VK_PAD_RTHUMB			&&
				label <= VK_PAD_RTHUMB_DOWNLEFT	)
			return true;
		else
			return false;
	}
	//	�賭��-_-; ���͸��� ���ļ� ������ ����� ������ �̿��ؼ� ������ ���콺 Ŀ���� ���Ѵ�.
	void	setStickCursorPos(POINT tilt_level) 
	{
		float	x_percentage = 0.0f,
				y_percentage = 0.0f;
		
		x_percentage = getStickTiltPercentage(tilt_level.x);
		y_percentage = getStickTiltPercentage(tilt_level.y);		
		//	����Ŀ����ǥ ���ϱ� #�۾�#
		//_cursor.x = (g_ini.getHalfHeight * x_percentage) + g_ini.getHalfHeight;
		_cursor.x = (LONG) (400 * x_percentage) + 400;
		_cursor.y = (LONG) (300 * y_percentage) + 300;
		
		POINT	filtered_pos;	
		filtered_pos.x = 0;
		filtered_pos.y = 0;
		//ScreenToClient(g_ini._hWnd, &_cursor); // #�׽�Ʈ# ������ �ʿ����
		//SetCursorPos(_cursor.x, _cursor.y);
	}
	float	getStickTiltPercentage(LONG tilt_level)
	{
		//	���ܻ�Ȳó��
		if (!tilt_level) // ������ ó���Ǿ� ���� ��� 
			return 0.0f;

		//	�����ڵ�:	
		//	�ۼ�Ƽ�� => �׳�, �ƽ������� ~ �ּ�ġ ���� �ۼ�Ƽ���� �Ѵ�.
		//			 =  (ƿƮ�� - �� ������) / (�ƽ�������-�ε�����) �� �Ұ��, �������� �ִ� ���� ���� �κ��� �ۼ�Ƽ���� �ٸ��ԵǱ⶧���̴�. (�ּ��� �� �ʿ�)		
		return (float) tilt_level / g_ini.getAnalogstickMaxDeadzone();
	}
	void	setMouseCursorDeadzone()
	{
		if (_cursor.x < 0)
			_cursor.x = 0;
		if (_cursor.x > g_ini._client_rect.right)
			_cursor.x = g_ini._client_rect.right;
		if (_cursor.y < 0)
			_cursor.y = 0;
		if (_cursor.y > g_ini._client_rect.bottom)
			_cursor.y = g_ini._client_rect.bottom;
	}



private:
	POINT	_cursor; 
};