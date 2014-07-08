#pragma once

#include <Windows.h>
#include <XInput.h>
#include "define.hpp"
#include "CINI.hpp"
#include "CInputInterface.hpp"

class CInputPressButton : public CInputInterface
{
public:
	CInputPressButton() : _level(0)
	{

	}
	~CInputPressButton()
	{

	}
	CInputPressButton&	operator = (CInputPressButton& rhs)
	{
		_level = rhs.getPressLevel();

		CInputInterface::operator = (rhs);
		return *this;
	}
	BYTE	getPressLevel()
	{
		return _level;
	}
	void	initializeState()
	{
		_level = 0;
	}

protected:
	virtual void	updateKeyboardMouse(CInputConsoleKeyboardAndMouse* keyboard_mouse, int index)
	{
		// nothing
	}
	virtual	void	updateXbox360Pad(CInputConsoleXbox360Pad* xbox360pad, int index)
	{
		setPressLevel(xbox360pad, index);
	}
	virtual	CInputInterfaceSet	checkKeyboardMouse(CInputConsoleKeyboardAndMouse* keyboard_mouse) // 0�� �ν� ����. WORD�� ������, _label�� WORD�̹Ƿ�
	{
		return CInputInterfaceSet(); // Ű����, ���콺���� ���й�ư�� ����
	}
	virtual	CInputInterfaceSet	checkXbox360Pad(CInputConsoleXbox360Pad* xbox360pad) // 0�� �ν� ����. WORD�� ������, _label�� WORD�̹Ƿ�
	{
		for (int n=0; n < DEFAULT_CINPUTCONSOLEXBOX360PAD_MAX_CONTROLLERS ;n++)
		{
			if (xbox360pad->isJoypadPlayerConnected(n))
			{
				if (	xbox360pad->getStateGamepad(n)->bLeftTrigger	||
						xbox360pad->getStateGamepad(n)->bRightTrigger	)
					return	CInputInterfaceSet(VK_PAD_A, 0); // �����е忡 �ش��ϴ� ���̺��̸� ���� ����.
			}
		}

		return CInputInterfaceSet();
	}

private:
	void	setPressLevel(CInputConsoleXbox360Pad* xbox360pad, int index)
	{
		switch(_interface_set[index].getLabel())
		{
			case VK_PAD_LTRIGGER:
			{
				_level = xbox360pad->getStateGamepad(_interface_set[index].getJoypadPlayer())->bLeftTrigger;
				break;
			}
			case VK_PAD_RTRIGGER:
			{
				_level = xbox360pad->getStateGamepad(_interface_set[index].getJoypadPlayer())->bRightTrigger;
				break;
			}
			default:
			{
				CError e
				(	
					_T("���XX:\tXBOX360�е忡�� ���й�ư(?)�� �� Ʈ���Ÿ� �����ѵ�, ���� �̻��Ѱ� ���Ա���?"), 
					_T(""),
					_T(__FUNCTION__)				
				);
				e.log();
				break;
			}
		}
	}


private:
	BYTE	_level;
};