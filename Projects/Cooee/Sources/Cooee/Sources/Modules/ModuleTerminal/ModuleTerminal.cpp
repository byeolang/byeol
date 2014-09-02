#include "ModuleTerminal.hpp"
#include "../Core/Core.hpp"

void ModuleTerminal::ArgumentNameList::onKeyPressed(char inputed)
{
	class CodeInputer : public LG::InputWindow
	{
	public:
		FUNC_TO_CALLER(ModuleTerminal)
		virtual NEObject& clone() const { return *(new CodeInputer(*this)); }

		CodeInputer(NENode& new_owner) 
			: LG::InputWindow("���� �߰��� Ű�� �̸��� �¿����Ű�� �����ϼ���. \n���� ���� �̸��� �Է� �� ���� �־��.", BLACK, WHITE),
			owner(new_owner)
		{
			
		}

		virtual void onUpdateData()
		{
			if( ! &owner) return;
			const NEKeyCodeSet& ks = owner.getKeySet();

			for(int n=0; n < ks.getLength() ;n++)
				if(input.history.find(ks[n].getName()) == NE_INDEX_ERROR)
					input.history.push(ks[n].getName());

			input.history_idx = 0;
			input.text = input.history[input.history_idx];
		}

		virtual void onInputed()
		{
			NEModule& m = toCaller().castObject();
			NEArgumentBase& arg = m.getArguments()[toCaller().argument_namelist.choosed];
		
			arg.setKeyName(input.text);
			
			delete_me = true;
		}

		NENode& owner;
	};

	ListGliph::onKeyPressed(inputed);

	if(inputed == CONFIRM)
		toOwner()->call(CodeInputer(_getOwnerNodeOf(toOwner()->castObject())));
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
	Core::getObjectBy(toOwner()->path, myhandler);
	
	return *myhandler.last_pointer;
}
