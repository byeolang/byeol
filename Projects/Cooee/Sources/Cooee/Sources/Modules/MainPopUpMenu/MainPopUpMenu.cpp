#include "MainPopUpMenu.hpp"
#include "../ScriptFilePopUpMenu/ScriptFilePopUpMenu.hpp"
#include "../RunDebuggingPopUpMenu/RunDebuggingPopUpMenu.hpp"
#include "../ObservePopUpMenu/ObservePopUpMenu.hpp"
#include "../HelpPopUpMenu/HelpPopUpMenu.hpp"
#include "../CommandInputWindow/CommandInputWindow.hpp"
#include "../EditPopUpMenu/EditPopUpMenu.hpp"
#include "../Really/Really.hpp"
#include "../Core/Core.hpp"

void MainPopUpMenu::onUpdateData()
{
	ListWindow::onUpdateData();

	list.items.create(9);
	list.items.push("��ũ��Ʈ ���� >");	
	/*
		list.items.push("���� ����");
		list.items.push("�ҷ�����");
		list.items.push("�����ϱ�");		
		list.items.push("��� ����");
	*/		
	list.items.push("���� > ");
	/*
		list.items.push("���⿡ �߰�");
		list.items.push("�̰� ����");
		list.items.push("����");
		list.items.push("�߶󳻱�");
		list.items.push("�ٿ��ֱ�");		
	*/
	list.items.push("���� & ����� >");
	/*
		list.items.push("����");
		list.items.push("�� ������ �����");
		list.items.push("����� ����");
	*/
	list.items.push("���� > ");
	/*
		list.items.push("�ö��Ÿ����");			
		list.items.push("���� ���� ��ȯ");
	*/
	list.items.push("�ܼ� ������");
	list.items.push("--------");
	list.items.push("���� >");
	list.items.push("--------");
	list.items.push("�ڷΰ���");
}


void MainPopUpMenu::onKeyPressed(char inputed)
{
	ListWindow::onKeyPressed(inputed);

	const NEString& chosen = list.items[list.choosed];

	switch(inputed)
	{
	case UP:		
		if(chosen == "--------")
			list.choosed--;
		break;
	case DOWN:
		if(chosen == "--------")
			list.choosed++;
		break;
	}
}

void MainPopUpMenu::onItemChoosed(type_index index, const NEString& chosen_content)
{ 
	switch(index)
	{
	case 0:	//	��ũ��Ʈ ���� >
		call(ScriptFilePopUpMenu());
		break;

	case 1:	//	���� >
		call(EditPopUpMenu());
		break;

	case 2:	//	���� & ����� >
		call(RunDebuggingPopUpMenu());
		break;

	case 3:	//	���� >
		call(ObservePopUpMenu());
		break;

	case 4:	//	�ܼ� ������
		call(CommandInputWindow());
		break;

	case 6:	//	������ ��
		call(HelpPopUpMenu());
		break;

	case 8:	//	�ڷΰ���
		if(LG::Core::getWindowList().getLength() <= 2)
			::Core::commander.command("exit");
		else
			LG::Core::getWindowList()[1].delete_me = true;
		delete_me = true;
		break;		
	}
}

MainPopUpMenu::MainPopUpMenu(Window& new_caller)
	: LG::ListWindow("Main Menu", 20, 5, 20, 12, BLACK, WHITE, WHITE, LIGHTRED), 
	caller(new_caller)
{

}
