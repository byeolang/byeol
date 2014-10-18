#include "CommandInputWindow.hpp"
#include "../Core/Core.hpp"

CommandInputWindow::CommandInputWindow()
: LG::InputWindow("������ ��ɾ �Է��ϼ���. (�� �𸣸� help�� �Է��ϼ���)", WHITE, DARKGRAY, "") 
{
	for(int n=0; n < ::Core::commander.getLength() ;n++)
		input.history.push(::Core::commander[n].names[0]);		
}

void CommandInputWindow::onInputed()
{
	::Core::commander.command(input.text);
	delete_me = true;
}