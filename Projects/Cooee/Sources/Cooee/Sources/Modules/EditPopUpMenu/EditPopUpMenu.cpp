#include "EditPopUpMenu.hpp"
#include "../HeaderModifier/HeaderModifier.hpp"
#include "../Core/Core.hpp"

void EditPopUpMenu::onUpdateData()
{
	list.items.create(5);
	list.items.push("���⿡ �߰�");
	list.items.push("�̰� ����");
	list.items.push("���⿡ ����");
	list.items.push("�̰� �߶󳻱�");	
	list.items.push("���⿡ �ٿ��ֱ�");
}

void EditPopUpMenu::onItemChoosed(type_index index, const NEString& chosen_content)
{
	NEEventHandler& handler = Editor::getInstance().getEventHandler();

	Window& caller = toCaller().caller;

	switch(index)
	{
	case 0:	//	���⿡ �߰�
		caller.onKeyPressed(ADD);
		toCaller().delete_me = delete_me = true;
		break;

	case 1:	//	�̰� ����
		caller.onKeyPressed(REMOVE);
		toCaller().delete_me = delete_me = true;
		break;

	case 2:	//	���⿡ ����
		caller.onKeyPressed(COPY);
		toCaller().delete_me = delete_me = true;
		break;

	case 3:	//	�̰� �߶󳻱�
		caller.onKeyPressed(CUT);
		toCaller().delete_me = delete_me = true;
		break;

	case 4:	//	���⿡ �ٿ��ֱ�
		caller.onKeyPressed(PASTE);
		toCaller().delete_me = delete_me = true;
		break;
	}
}

EditPopUpMenu::EditPopUpMenu()
: LG::ListWindow("���� > ", 30, 8, 17, 8, BLACK, WHITE, WHITE, LIGHTGRAY)
{

}
