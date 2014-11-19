#include "HelpPopUpMenu.hpp"
#include "../Core/Core.hpp"

void HelpPopUpMenu::onUpdateData()
{
	list.items.create(3);
	list.items.push("�ָܼ�ɾ� ��� > ");
	list.items.push("��� ���� > ");
	list.items.push("�ʺ��ڸ� ���� ���̵� > ");
}

void HelpPopUpMenu::onItemChoosed(type_index item_index, const NEString& chosen_content)
{
	switch(item_index)
	{
	case 0:
		::Core::commander.command("help -command");
		break;

	case 1:
		::Core::commander.command("help -module");
		break;

	case 2:
		::Core::commander.command("help");
		break;
	}

	getCaller().delete_me = delete_me = true;
}

HelpPopUpMenu::HelpPopUpMenu()
: LG::ListWindow("���� > ", 30, 8, 24, 6, BLACK, WHITE, BLACK, YELLOW)
{

}