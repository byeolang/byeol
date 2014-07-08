#include "CInputInterface.hpp"

CInputInterface::CInputInterface() : _enable(false)
{

}
CInputInterface::~CInputInterface()
{}

CInputInterfaceSet*	CInputInterface::getInterfaceSet(int index)
{
	//	���ܻ�Ȳó��:
	if (	index < 0										||
			index >= DEFAULT_CINPUTINTERFACESET_MAX_KEYSET	)
	{
		CError e
		(	
			_T("���XX:\t�� ��ǲ�� �����Ҽ��ִ� �������̽����� �Ѿ���ϴ�."), 
			_T(""),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
		);
		e.log();
	}

	//	�����ڵ�:
	return &_interface_set[index];
}

CInputInterface&	CInputInterface::operator = (CInputInterface& rhs)
{
	_enable = rhs._enable;
	for (int n=0; n < DEFAULT_CINPUTINTERFACESET_MAX_KEYSET ;n++)
		_interface_set[n] = *(rhs.getInterfaceSet(n));
			
	return *this;
}
void	CInputInterface::update(CInputConsoleKeyboardAndMouse* keyboard_mouse, CInputConsoleXbox360Pad* xbox360pad)
{
	//	���ܻ�Ȳó��:
	if (!_enable)
		return;

	//	�����ڵ�:
	for (int n=0; n < DEFAULT_CINPUTINTERFACESET_MAX_KEYSET ;n++)
		updateOneInterfaceSet(n, keyboard_mouse, xbox360pad);		
}

void	CInputInterface::updateOneInterfaceSet(int index, CInputConsoleKeyboardAndMouse* keyboard_mouse, CInputConsoleXbox360Pad* xbox360pad)
{
	//	�����ڵ�:
	if (isXbox360Pad(_interface_set[index].getLabel()))
	{
		if ( xbox360pad->isJoypadPlayerConnected( _interface_set[index].getJoypadPlayer()) )
			updateXbox360Pad(xbox360pad, index);
	}
	else if (isKeyboardMouse(_interface_set[index].getLabel()))
		updateKeyboardMouse(keyboard_mouse, index);
	else
	{
		CError e
		(	
			_T("���XX:\t��ǲ ��ġ�� ���¸� ������Ʈ�Ϸ��� ������, �˼����� ��ġ�Դϴ�. (��ȣ�ʰ�)"), 
			_T(""),
			_T(__FUNCTION__),
			true // �޼����ڽ�, �α׸� ��������ʴ´�
		);
		e.log();		
	}	
}

