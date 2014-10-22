#include "ModuleTerminal.hpp"
#include "../Core/Core.hpp"
#include "../MainPopUpMenu/MainPopUpMenu.hpp"

void ModuleTerminal::ArgumentNameList::onKeyPressed(char inputed)
{
	class CodeInputer : public LG::InputWindow
	{
	public:
		FUNC_TO_CALLER(ModuleTerminal)
		virtual NEObject& clone() const { return *(new CodeInputer(*this)); }

		CodeInputer(NENode& new_owner) 
			: LG::InputWindow("���� �߰��� Ű�� �̸��� �¿����Ű�� �����ϼ���. \n���� ���� �̸��� �Է� �� ���� �־��.", BLACK, YELLOW),
			owner(new_owner)
		{
			
		}

		virtual void onUpdateData()
		{
			if( ! &owner) return;
			const NEKeyCodeSet& ks = owner.getKeySet();

			for(int n=0; n < ks.getLength() ;n++)
				if(input.history.find(ks[n].getName()) == NE_INDEX_ERROR)
					input.history.push(ks[n].getName() + "(" + ks[n].getTypeName() + ")");

			input.history_idx = 0;
			input.text = input.history[input.history_idx];
		}

		virtual void onInputed()
		{
			NEModule& m = toCaller().castObject();
			NEArgumentBase& arg = m.getArguments()[toCaller().argument_namelist.choosed];

			NEStringSet splited;
			input.text.split("(", splited);
			splited.pop();
			NEString keyname;
			for(int n=0; n < splited.getLength(); n++)
				keyname += splited[n];
		
			arg.setKeyName(keyname);
			
			delete_me = true;
		}

		NENode& owner;
	};

	ListGliph::onKeyPressed(inputed);

	switch(inputed)
	{
	case CONFIRM:
		toOwner()->call(CodeInputer(_getOwnerNodeOf(toOwner()->castObject())));
		break;

	case CANCEL:	
		LG::Core::open(MainPopUpMenu(*toOwner()));
		break;

	case CLOSE:
		toOwner()->delete_me = true;
		break;
	}		
}

NENode& ModuleTerminal::ArgumentNameList::_getOwnerNodeOf(NEModule& target)
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
	Core::getObjectBy(toOwner()->getPath(), myhandler);
	
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
	text =	"name:	" + header.getName() + " rev#" + header.getRevision() + "\n" +
		"madeby:" + header.getDeveloper() + "\n" +
		"on:	" + header.getReleaseDate();
}
