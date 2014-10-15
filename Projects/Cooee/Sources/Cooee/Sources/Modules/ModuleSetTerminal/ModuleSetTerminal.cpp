#include "ModuleSetTerminal.hpp"
#include "../ModuleEncyclo/ModuleEncyclo.hpp"
#include "../ModuleTerminal/ModuleTerminal.hpp"

class NameInputWindow : public ::LG::InputWindow
{
public:
	NameInputWindow(const NEString& default_string) : InputWindow("���ο� Ű�� �̸��� �Է����ֽñ� �ٶ��ϴ�.", BLACK, LIGHTCYAN, default_string) {}

	FUNC_TO_CALLER(ModuleSetTerminal)
	FUNC_CLONE(NameInputWindow)

	virtual void onInputed()
	{
		toCaller().real_key->getName() = input.text;
		delete_me = true;
	}
};

void ModuleSetTerminal::ModuleNameList::onKeyPressed(char inputed) 
{
	ListGliph::onKeyPressed(inputed);

	switch(inputed) {
	case CONFIRM:
		{
			const NEModuleSet& ms = Kernal::getInstance().getModuleManager().getModuleSet();
		
			if(choosed < 0 || choosed > ms.getLengthLastIndex())	return;

			int index = toOwner()->real_key ? choosed-1 : choosed;
			if(index == -1)
				toOwner()->call(NameInputWindow(toOwner()->real_key->getName()));
			else
			{
				if(index <= items.getLengthLastIndex())
					LG::Core::open(ModuleTerminal(toOwner()->getPath() + "/" + index));
			}
				//LG::Core::windows.pushFront(ModuleEncyclo(false, &toOwner()->castObject()[index]));
		}	
		break;
	case CANCEL:
		if(LG::Core::windows.getLength() == 1)
			Core::pushMessage("���Ⱑ �ֻ��� ���.\n�츮����, �� ���ư� ��ҵ��� �����.");
		else
			toOwner()->delete_me = true;
		break;

	case ADD:		
		{	
			NEString path = toOwner()->getPath() + "/";
			NEObject& obj = ::Core::getObjectBy(path);
			NEModuleCodeSet& mcs = static_cast<NEModuleCodeSet&>(obj);
			if( ! obj.isSubClassOf(NEType::NEMODULE_CODESET))
				return;

			path += (choosed < 0 || items.getLength() < 0) ? mcs.getLength() : choosed;
			LG::Core::windows.pushFront(ModuleEncyclo(0, path));
		}
		break;
	}	
}

