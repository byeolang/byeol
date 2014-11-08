#include "RunDebuggingPopUpMenu.hpp"
#include "../Core/Core.hpp"

RunDebuggingPopUpMenu::RunDebuggingPopUpMenu()
: LG::ListWindow("���� & ����� > ", 30, 8, 15, 5, BLACK, WHITE, WHITE, LIGHTBLUE)
{

}

void RunDebuggingPopUpMenu::onUpdateData()
{
	list.items.create(3);
	bool is_test_running = Editor::getInstance().getEventHandler().isTestRunning();
	if(is_test_running)
		list.items.push("��� �����");
	else
		list.items.push("���Ӱ� �����");
	if(is_test_running)
		list.items.push("Ư�� Ƚ���� ��� �����");
	else
		list.items.push("Ư�� Ƚ���� ���Ӱ� �����");
	list.items.push("����� ����");
}

void RunDebuggingPopUpMenu::onItemChoosed(type_index item_index, const NEString& chosen_content)
{
	switch(item_index)
	{
	case 0:    //    ��� / ���Ӱ� �����
		if(chosen_content == "��� �����")
			::Core::commander.command("run");
		else
			::Core::commander.command("run -new");
		break;

	case 1:    //    Ư�� Ƚ���� �����    
		class DebuggingCount : public LG::InputWindow
		{
		public:
			DebuggingCount(bool is_debugging_newly) 
				: LG::InputWindow("��ũ��Ʈ�� �� �� �������� Ƚ���� �����ϼ���.", WHITE, LIGHTRED, "1"),
				_is_debugging_newly(is_debugging_newly) {}
			virtual NEObject& clone() const
			{
				return *(new DebuggingCount(*this));
			}

			virtual void onInputed()
			{
				int count = input.text.toInt();

				NEString command = "run ";
				if(_is_debugging_newly)
					command += "-new ";

				::Core::commander.command(command + count);

				getCaller().delete_me = delete_me = true;
			}

			bool _is_debugging_newly;
		};

		if(chosen_content == "Ư�� Ƚ���� ��� �����")
			call(DebuggingCount(false));
		else
			call(DebuggingCount(true));
		break;        

	case 2:    //    ����� ����
		::Core::commander.command("run -stop");
		delete_me = true;
		break;
	}
}