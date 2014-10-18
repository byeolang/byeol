#include "MainPopUpMenu.hpp"
#include "../ScriptFilePopUpMenu/ScriptFilePopUpMenu.hpp"
#include "../RunDebuggingPopUpMenu/RunDebuggingPopUpMenu.hpp"
#include "../ObservePopUpMenu/ObservePopUpMenu.hpp"
#include "../HelpPopUpMenu/HelpPopUpMenu.hpp"
#include "../CommandInputWindow/CommandInputWindow.hpp"
#include "../Really/Really.hpp"

void MainPopUpMenu::onUpdateData()
{
	ListWindow::onUpdateData();

	list.items.create(8);
	list.items.push("��ũ��Ʈ ���� >");	
	/*
		list.items.push("���� ����");
		list.items.push("�ҷ�����");
		list.items.push("�����ϱ�");		
		list.items.push("��� ����");
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

	case 1:	//	���� & ����� >
		call(RunDebuggingPopUpMenu());
		break;

	case 2:	//	���� >
		call(ObservePopUpMenu());
		break;

	case 3:	//	�ܼ� ������
		call(CommandInputWindow());
		break;

	case 5:	//	������ ��
		call(HelpPopUpMenu());
		break;

	case 7:	//	�ڷΰ���
		if(LG::Core::windows.getLength() <= 2)
			call(Really());
		else
			LG::Core::windows[1].delete_me = true;
		delete_me = true;
		break;		
	}
}

MainPopUpMenu::MainPopUpMenu()
	: LG::ListWindow("Main Menu", 20, 5, 20, 9, BLACK, WHITE, WHITE, LIGHTRED)
{

}
