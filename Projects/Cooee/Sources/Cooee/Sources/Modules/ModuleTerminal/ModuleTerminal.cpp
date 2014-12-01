#include "ModuleTerminal.hpp"
#include "../Core/Core.hpp"
#include "../MainPopUpMenu/MainPopUpMenu.hpp"
#include "Windows.h"
#pragma comment(lib, "winmm.lib")

void ModuleTerminal::ArgumentNameList::onKeyPressed(char inputed)
{
	class ArgumentPopUp : public LG::ListWindow
	{
	public:
		ArgumentPopUp() : LG::ListWindow("�� Argument��...", 30, 10, 20, 7, BLACK, WHITE, WHITE, LIGHTCYAN) {}
		FUNC_TO_CALLER(ModuleTerminal)
			virtual NEObject& clone() const {
				return *(new ArgumentPopUp(*this));
		}

		virtual void onUpdateData()
		{
			ListWindow::onUpdateData();

			list.items.create(3);
			list.items.push("�⺻ ���� �����Ѵ�.");
			list.items.push("���� Ű�� ���Ѵ�.");
			list.items.push("Ű�� ���� �ʴ´�.");
		}		
		virtual void onItemChoosed(type_index index, const NEString& chosen_content)
		{
			class CodeInputer : public LG::InputWindow
			{
			public:
				FUNC_TO_CALLER(ArgumentPopUp)
					virtual NEObject& clone() const { return *(new CodeInputer(*this)); }

				CodeInputer(NENode& new_owner)
					: LG::InputWindow("���� �߰��� Ű�� �̸��� �¿����Ű�� �����ϼ���. \n���� ���� �̸��� �Է� �� ���� �־��.", BLACK, YELLOW),
					owner(new_owner)
				{

				}

				virtual void onFocused()
				{
					if (!&owner) return;
					const NEKeyCodeSet& ks = owner.getKeySet();
					for (int n = 0; n < ks.getLength(); n++)
						if (input.history.find(ks[n].getName()) == NE_INDEX_ERROR)
							input.history.push(ks[n].getName() + "(" + ks[n].getTypeName() + ")");

					input.history_idx = 0;
				}
				virtual void onUpdateData()
				{
					NEString& history = input.history[input.history_idx];
					if (&history)
						input.text = history;
				}

				virtual void onInputed()
				{
					NEModule& m = toCaller().toCaller().castObject();
					NEArgumentBase& arg = m.getArguments()[toCaller().toCaller().argument_namelist.choosed];

					NEStringSet splited;
					input.text.split("(", splited);
					if (splited.getLength() > 0)
						splited.pop();
					NEString keyname;
					for (int n = 0; n < splited.getLength(); n++)
						keyname += splited[n];

					arg.setKeyName(keyname);

					toCaller().delete_me = delete_me = true;
				}

				NENode& owner;
			};

			NEModule& m = toCaller().castObject();
			NEArgumentSet& args = m.getArguments();

			NEArgumentBase& arg = args[toCaller().argument_namelist.choosed];
			if (!&arg)
			{
				::Core::pushMessage("����: ������ Argument�� �ε����� ITArgument�� �ε����� �Ѿ����ϴ�.");
				return;
			}

			switch (index)
			{
			case 0:
				::Core::openModifierFrom(arg.getDefaultKey(), true);
				delete_me = true;
				break;

			case 1:
				{
					ModuleTerminal& mt = toCaller();
					call(CodeInputer(mt.getOwnerNodeOf(mt.castObject())));
				}				
				break;

			case 2:
				arg.setKeyName(NETString());
				delete_me = true;
				break;
			}
		}
	};

	ListGliph::onKeyPressed(inputed);

	switch(inputed)
	{
	case CONFIRM:
		toOwner()->call(ArgumentPopUp());
		break;

	case CANCEL:	
		LG::Core::open(MainPopUpMenu(*toOwner()));
		break;

	case CLOSE:
		toOwner()->delete_me = true;
		break;
	}		
}

NENode& ModuleTerminal::getOwnerNodeOf(NEModule& target)
{
	class MyHandler : public Core::onObjectFound
	{
	public:
		MyHandler() : last_pointer(0) {}
		virtual void onNodeFound(NENode& node)
		{
			last_pointer = &node;
		}
		NENode* last_pointer;
	};

	MyHandler myhandler;
	Core::getObjectBy(getPath(), myhandler);
	
	return *myhandler.last_pointer;
}

void ModuleTerminal::ModulePanel::onUpdateData()
{
	if( ! &toOwner()->castObject())
	{
		toOwner()->delete_me = true;

		::Core::pushMessage("�߸��� ��η� �����߽��ϴ�.");				
	}
	const NEExportable::ModuleHeader& header = toOwner()->castObject().getHeader();
	text =	"\t"	+ header.getName() + " #" + header.getRevision() + "\n" +
			"\tby " + header.getDeveloper() + "\ton " + header.getReleaseDate();
}

ModuleTerminal::FloatingPanel::FloatingPanel()
: LG::FloatingGliph(0, 0, 60, 5, DARKGRAY, WHITE)
{

}
