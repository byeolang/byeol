#include "RunDebuggingPopUpMenu.hpp"

RunDebuggingPopUpMenu::RunDebuggingPopUpMenu()
: LG::ListWindow("���� & ����� > ", 30, 8, 15, 5, BLACK, WHITE, WHITE, LIGHTBLUE)
{

}

void RunDebuggingPopUpMenu::onUpdateData()
{
	list.items.create(3);
	list.items.push("����");
	list.items.push("�� ������ �����");
	list.items.push("����� ����");
}

void RunDebuggingPopUpMenu::onItemChoosed(type_index item_index, const NEString& chosen_content)
{

}