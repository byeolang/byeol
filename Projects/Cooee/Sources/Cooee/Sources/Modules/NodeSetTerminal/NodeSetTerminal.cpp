#include "NodeSetTerminal.hpp"
#include "../ModuleEncyclo/ModuleEncyclo.hpp"
#include "../Core/Core.hpp"

void NodeSetTerminal::ContentList::onKeyPressed(char inputed) 
{
	ListGliph::onKeyPressed(inputed);

	switch(inputed) 
	{
	case UP:
	case DOWN:
		{
			toOwner()->gate.text = createCommand();
			toOwner()->gate.hinted = true;
			onDraw();
		}
		break;
	case ADD:
		LG::Core::windows.pushFront(ModuleEncyclo(true));
		break;
	case REMOVE:
		NEString& msg = ::Core::commander.command(NEString("remove ") + choosed);
		if(msg.getLength() > 0)
			Core::pushMessage(msg);
		break;
	}			
}

void NodeSetTerminal::GeniusGate::onKeyPressed(char inputed) 
{
	switch(inputed) 
	{
	case BACKSPACE:		
		break;
	case ADD: case REMOVE:
		return;	//	Ű ��ȿ
	case CONFIRM:
		{
			if(text == "" || text.getLength() <= 0)
				text = toOwner()->contents.createCommand();

			LG::Core::windows.pushFront(ModuleSetTerminal("/0/m"));
			return;

			NEString& msg = Core::commander.command(text);
			if(msg.getLength() > 0)
				Core::pushMessage(msg);
		}
		break;
	case CANCEL:

		class Really : public ::LG::QueryWindow
		{
		public:
			Really()
				: QueryWindow("�����ҷ����? �̷��� ��մ� ���α׷��� ���ΰ�?", WHITE, LIGHTBLUE)
			{
				panel.text = "�׸��ҷ���";
				panel.width = 15;
				no.text = "��ư�� �߸� ������";
				no.width = 20;
			}
			virtual void onButtonPressed(bool witch_button)
			{
				if(witch_button)
					getCaller().delete_me = delete_me = true;

				delete_me = true;
			}
			NEObject& clone() const { return *(new Really(*this)); }
		};

		if(LG::Core::windows.getLength() == 1)
			toOwner()->call(Really());
		else
			toOwner()->delete_me = true;
		break;
	}

	TextGliph::onKeyPressed(inputed);
}