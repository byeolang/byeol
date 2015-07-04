#include "ScriptFilePopUpMenu.hpp"
#include "../HeaderModifier/HeaderModifier.hpp"
#include "../Core/Core.hpp"

void ScriptFilePopUpMenu::onUpdateData()
{
	list.items.create(4);
	list.items.push("���� ����");
	list.items.push("�ҷ�����");
	list.items.push("�����ϱ�");
	list.items.push("��� ����");
}

void ScriptFilePopUpMenu::LoadScript::onItemChoosed(type_index chosen, const NEString& content)
{
	if (chosen < 0) return;
	if (!&content) return;

	::Core::commander.command("load " + content);

	delete_me = true;

	if(&getCaller())
		getCaller().delete_me = true;
}

void ScriptFilePopUpMenu::SaveScript::onInputed()
{
	::Core::commander.command("save " + input.text);

	delete_me = true;

	if(&getCaller())
		getCaller().delete_me = true;
}

void ScriptFilePopUpMenu::onItemChoosed(type_index index, const NEString& chosen_content)
{
	NEEventHandler& handler = Editor::getInstance().getEventHandler();

	switch(index)
	{
	case 0:	//	���� ����		
		::Core::commander.command("new");
		break;

	case 1:	//	�ҷ� ����
		call( LoadScript() );
		break;

	case 2:	//	�����ϱ�
		call(SaveScript(Kernal::getInstance().getScriptManager().getScriptFilePath()) );
		break;

	case 3:	//	��� ����
		call(HeaderModifier());
		delete_me = true;
		break;
	}
}

ScriptFilePopUpMenu::ScriptFilePopUpMenu()
: LG::ListWindow("��ũ��Ʈ ���� > ", 30, 8, 11, 7, BLACK, WHITE, WHITE, LIGHTCYAN)
{

}
