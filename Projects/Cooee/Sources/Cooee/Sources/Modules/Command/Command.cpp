#include "Command.hpp"
#include "../Core/Core.hpp"
#include "../Planetarium/Planetarium.hpp"
#include "../HeaderModifier/HeaderModifier.hpp"
#include "../CommandListWindow/CommandListWindow.hpp"
#include "../ModuleEncyclo/ModuleEncyclo.hpp"
#include "../NodeSetTerminal/NodeSetTerminal.hpp"
#include "../GuideEncyclo/GuideEncyclo.hpp"
#include "../Really/Really.hpp"

//		N:1 ����(����) �� �Լ� ���ø�
template <typename KeyContainer, typename Source>
bool _pasteInKeyContainer(NEObject& cont, const NEObject& src, NEType::Type src_type, type_index index = -1)
{
	NEType::Type cont_type = KeyContainer().getType();

	if( ! &cont							||
		! &src							||
		! cont.isSubClassOf(cont_type)	||
		! src.isSubClassOf(src_type)	)
		return false;

	KeyContainer& c = static_cast<KeyContainer&>(cont);
	const Source& s = static_cast<const Source&>(src);

	if(c.getLength() == c.getSize())
		c.resize(c.getLength() + 1);

	if(index != -1)
		c.insert(index, s);
	else
		c.push(s);

	return true;
}

//	Helper �Լ� ���ø�:
template <typename Target, typename Source>
bool _pasteTryEverything(NEObject& target, NEObject& parent, const NEObject& source, NEType::Type src_type, type_index index)
{
	if(_pasteInKeyContainer<Target, Source>(parent, source, src_type, index))
		return true;

	return _pasteInKeyContainer<Target, Source>(target, source, src_type);
}

Command::Command(const NEString& names_delimetered_with_space, const NEString& new_help)
: help(new_help)
{
	names_delimetered_with_space.split(" ", names);
}

type_result Command::isValid() const {return 0;}
NEBinaryFileSaver& Command::serialize(NEBinaryFileSaver& saver) const { return saver; }
NEBinaryFileLoader& Command::serialize(NEBinaryFileLoader& loader) { return loader; }
void Command::release() {}
bool Command::operator!=(const Command& rhs) const { return names == rhs.names && help == rhs.help; }

ListCommand::ListCommand() 
: Command("list ls", 
		  "�ش� ����� ������ ȭ�鿡 ����մϴ�.\n"
		  "��Ī	: list, ls\n"
		  "���� : list <���>") 
{
}

void printShortCutSet(const NEShortCutSet& shorts)
{
	for(int n=0; n < shorts.getLength() ;n++)
	{
		cout << n << ": ";
		for(int x=0; x < shorts[n].getLength() ;x++)
			cout << shorts[n][x] << ", ";
		cout << "\n";
	}
}

NE::NEString ListCommand::execute(const NEStringSet& parameters) 
{
	if(parameters.getLength() <= 0)
		return "ERROR: ���ڰ� �ʿ��մϴ�.";

	if(parameters[0] == "-cache")    //    shortcutset�� ����Ѵ�.
	{
		system("cls");
		const NEEnlistableManager* mng = 0;
		if( ! ::Core::isObservingDebug())
		{
			mng = &Editor::getInstance().getScriptEditor();
			cout << "ScriptEditor:\n";
		}
		else
		{
			mng = &Kernal::getInstance().getNodeManager();
			cout << "NodeManager:\n";
		}

		cout << "Script  -------------------------\n";    
		printShortCutSet(mng->getScriptShortCutSet());
		cout << "Name    -------------------------\n";    
		printShortCutSet(mng->getNameShortCutSet());
		cout << "Group   -------------------------\n";
		printShortCutSet(mng->getGroupShortCutSet());
		cout << "Priority-------------------------\n";    
		printShortCutSet(mng->getPriorityShortCutSet());

		system("pause");
		LG::Core::back_buffer.readyBufferToDraw();
	}
	else
	{
		const NEString& path = parameters[0];
		NEObject& parsed = ::Core::getObjectBy(path);
		if( ! &parsed)
			return "ERROR: " + path + "�� �߸��� ��� �Դϴ�.";

		if(parsed.isSubClassOf(NEType::NEKEY))
			::Core::openModifierFrom((NEKey&)parsed);
		else
			::Core::openModifierFrom(path);
	}

	return "";
}

NE::NEString CloseCommand::execute(const NEStringSet& parameters)
{
	LG::WindowList& wins = LG::Core::getWindowList();
	if(wins.getLength() > 1)
		wins[0].delete_me = true;
	else if( ! ::Core::isObservingDebug())
		::Core::commander.command("exit");
	else
		::Core::pushMessage("�� â�� ���� �� �����");

	return "";
}

NE::NEString VersionCommand::execute(const NEStringSet& parameters)
{
	LG::Core::getWindowList().pushFront(LG::MessageWindow(NEString(_VERSION_STRING) + "\n\tby " + _DEVELOPER + "\n\ton " + _DATE, WHITE, LIGHTRED));
	return "";
}

NE::NEString PlanetarizeCommand::execute(const NEStringSet& parameters)
{
	LG::Core::open(Planetarium());

	if(parameters.getLength() > 0)
	{
		NEObject& parsed = ::Core::getObjectBy(parameters[0]);
		if( ! &parsed) return "ERROR: �־��� ��ΰ� �߸��Ǿ��׿�.\n�־��� ��� : " + parameters[0];
		Planetarium& planetarium = dynamic_cast<Planetarium&>(LG::Core::getWindowList()[0]);
		planetarium.setFocus(parsed);
	}

	return "";
}

NE::NEString CopyCommand::execute(const NEStringSet& parameters)
{
	if(::Core::isObservingDebug())
		return "ERROR: �׽�Ʈ ���������� ������ ������ �����մϴ�.";

	type_result result = RESULT_SUCCESS;
	switch(parameters.getLength())
	{
	case 1:
		::Core::setPathToBeCopied(parameters[0]);
		return "";

	case 2:
		::Core::setPathToBeCopied(parameters[0]);
		return ::Core::commander.command("paste " + parameters[1]);

	default:
		return "ERROR: ��θ� �Է��ϼ���";
	}	
}

NE::NEString CutCommand::execute(const NEStringSet& parameters)
{
	if(::Core::isObservingDebug())
		return "ERROR: �׽�Ʈ ���������� ������ ������ �����մϴ�.";

	type_result result = RESULT_SUCCESS;
	switch(parameters.getLength())
	{
	case 1:
		::Core::setPathToBeCutOff(parameters[0]);
		return "";

	case 2:
		::Core::setPathToBeCutOff(parameters[0]);
		return ::Core::commander.command("paste " + parameters[1]);

	default:
		return "ERROR: ��θ� �Է��ϼ���";
	}	
}

NEType::Type AddCommand::_findKeyTypeBy(const NEString& type_name)
{
	const NEKeySet& ks = Kernal::getInstance().getKeyManager().getKeySet();
	for(int n=0; n < ks.getLength() ;n++)
		if(type_name == ks[n].getTypeName())
			return ks[n].getType();

	return NEType::UNDEFINED;
}

NE::NEString AddCommand::execute(const NEStringSet& parameters)
{
	if(::Core::isObservingDebug())
		return "ERROR: �׽�Ʈ ���������� ������ ������ �����մϴ�.";
	if(parameters.getLength() <= 0)    return "ERROR: Container�� ��θ� �Է��ϼ���";

	NENodeCodeSet* nsc = 0;
	NEModuleCodeSet* msc = 0;        
	type_index index_to_add = NE_INDEX_ERROR;
	NEObject* parent = 0x00;

	if(parameters[0] == "-node")
	{
		if(parameters.getLength() < 2) return NEString("ERROR: ������ ������ 2������ �մϴ�. \n���� ���� �� : ") + parameters.getLength();
		_searchParent(parameters[1], index_to_add, &parent);
		NEObject& target = ::Core::getObjectBy(parameters[1]);

		NENode source;
		if( ! _pasteTryEverything<NENodeCodeSet, NENode>(target, *parent, source, NEType::NENODE, index_to_add))
			return    NEString("ERROR: �־��� Ÿ��(") + target.getTypeName() + ")�� ����(" + 
			source.getTypeName() + ") ������ Paste�� �Ұ��� �մϴ�.";
	} 
	else if(parameters[0] == "-module")
	{
		if(parameters.getLength() < 5) return NEString("ERROR: ������ ������ 5������ �մϴ�. \n���� ���� �� : ") + parameters.getLength();
		_searchParent(parameters[4], index_to_add, &parent);
		NEObject& target = ::Core::getObjectBy(parameters[4]);

		NEExportable::Identifier identifier(parameters[1], parameters[2], parameters[3].toInt());
		const NEModule& source = Kernal::getInstance().getModuleManager().getModule(identifier);
		if( ! &source) return "ERROR: �߸��� Identifier �Դϴ�.";

		if( ! _pasteTryEverything<NEModuleCodeSet, NEModule>(target, *parent, source, NEType::NEMODULE, index_to_add))
			return    NEString("ERROR: �־��� Ÿ��(") + target.getTypeName() + ")�� ����(" + 
			source.getTypeName() + ") ������ Paste�� �Ұ��� �մϴ�.";
	}
	else if(parameters[0] == "-key")
	{
		if(parameters.getLength() < 3) return NEString("ERROR: ������ ������ 3������ �մϴ�. \n���� ���� �� : ") + parameters.getLength();

		const NEKey& source = Kernal::getInstance().getKeyManager().getKey(_findKeyTypeBy(parameters[1]));
		if( ! &source) return "ERROR: " + parameters[1] + "�� �ش��ϴ� Ű�� �����ϴ�.";
		_searchParent(parameters[2], index_to_add, &parent);
		NEObject& target = ::Core::getObjectBy(parameters[2]);

		if( ! _pasteTryEverything<NEKeyCodeSet, NEKey>(target, *parent, source, NEType::NEKEY, index_to_add))
			return    NEString("ERROR: �־��� Ÿ��(") + target.getTypeName() + ")�� ����(" + 
			source.getTypeName() + ") ������ Paste�� �Ұ��� �մϴ�.";
	}

	return "";
}

NE::NEString DeleteCommand::execute(const NEStringSet& parameters)
{
	if(::Core::isObservingDebug())
		return "ERROR: �׽�Ʈ ���������� ������ ������ �����մϴ�.";
	if(parameters.getLength() <= 0)	return "ERROR: ��θ� �Է��ϼ���.";

	type_index idx_to_del = 0;
	NEObject* parent = 0x00;
	_searchParent(parameters[0], idx_to_del, &parent);

	NENodeCodeSet* ncs = 0x00;
	NEModuleCodeSet* mcs = 0x00;
	type_result result = RESULT_SUCCESS;

	::Core::setPathToBeCopied("");
	//	�� ����� Path�� ����� ��:
	//		�̹� ������ ��θ� ���� �� �� �ֱ� �����̴�.
	//		����� �Ϸ��� Delete�ؼ� ����� Path �ڽ��̳� ����� �� 
	//		�ϳ��� �����Ѵٸ�, Path�� null�� ������ �ϳ�
	//		������ �˾Ƴ��� ����� ����� �����Ƿ� ������.

	if(parent->isSubClassOf(NEType::NENODE_CODESET_KEY))
		ncs = &((NENodeCodeSetKey*) parent)->getValue();
	if(parent->isSubClassOf(NEType::NENODE_CODESET))
	{
		if( ! ncs)
			ncs = (NENodeCodeSet*) parent;
		if(idx_to_del > ncs->getLengthLastIndex())
			return "ERROR: �ε����� �������� �� Ů�ϴ�.";
		ncs->remove(idx_to_del);
		return "";
	}

	if(parent->isSubClassOf(NEType::NEMODULE_CODESET_KEY))
		mcs = &((NEModuleCodeSetKey*) parent)->getValue();

	if(parent->isSubClassOf(NEType::NEMODULE_CODESET))
	{
		if( ! mcs)
			mcs = (NEModuleCodeSet*) parent;
		if(idx_to_del > mcs->getLengthLastIndex())
			return "ERROR: �ε����� �������� �� Ů�ϴ�.";
		mcs->remove(idx_to_del);
		return "";
	}

	if(parent->isSubClassOf(NEType::NEKEY_CODESET))
	{
		NEKeyCodeSet& kcs = (NEKeyCodeSet&) *parent;
		if(idx_to_del > kcs.getLengthLastIndex())
			return "ERROR: �ε����� �������� �� Ů�ϴ�.";
		kcs.remove(idx_to_del);
		return "";
	}

	return (NEResult::hasError(result)) ? "ERROR: ���� ���� ������ �߻��߽��ϴ�." : "";
}

NE::NEString OrphanCommand::_searchParent(const NEString& full_path, type_index& index_to_insert, NEObject** parent_to_insert)
{
	NEStringSet splited;
	full_path.split("/", splited);
	if(splited.getLength() <= 0)
		return "ERROR: �־��� ��ΰ� \"/\" ������ �ȉϴϴ�.";
	NEString parent_path = "/";
	index_to_insert = splited[splited.getLengthLastIndex()].toInt();
	for(int n=0; n < splited.getLength()-1 ;n++)
		parent_path += splited[n] + "/";

	*parent_to_insert = &::Core::getObjectBy(parent_path);
	if( ! *parent_to_insert) return "ERROR: �ش� ���(" + parent_path + ")�� �߸��Ƴ׿�.";

	return "";
}

//	Paste:
//		Paste�� ����:
//			Paste�� ���� ���� �κ�:
//				Ÿ�� ���(= parameters[0]) �� ���� ���(= ::Core::path_to_be_copied)
//				�� Ÿ���� ���� ����. ���� Ÿ��Ÿ��, ���� Ÿ���̶� �Ѵ�.
//				������, Paste ������ ������ �������� �б��ϴ� ������ �����Ѵ�.
//
//			N:1 ���� ����:	Ÿ�ٰ�ü �ȿ� ������ü�� �ϳ� �����Ѵ�. (push)
//							Ÿ�� Ÿ���� ����Ÿ���� Container �� ���.
//							Container ���δ� ���� if ������ Ȯ�� �� �� �ۿ� ����. 
//							(�����ӿ�ũ�� RTTI�� ����ϴ�)
//
//			N:1 ���� ����:	�̸� ��� index�� �ְ� �ʹٴ� �ε����� ������ �� �����̴�.
//							N:1�� �����ϵ�, push �� �ƴ϶� insert�� �����Ѵ�.
//
//		Paste �˰���:
//			Ÿ�� �˻��ϰ� ĳ������ �ϴ� �κ��� �Լ� ���ø����� ��ü�Ѵ�.
//			�� �Լ� ���ø��� ������ ���� �� �� true�� ��ȯ�Ѵ�.
//			Paste �Լ������� ��ȯ���� true�� ���ö����� ������ �� ��� ������ 
//			�Լ����ø��� ȣ���Ѵ�.
//			
NE::NEString PasteCommand::execute(const NEStringSet& parameters)
{
	if(::Core::isObservingDebug())
		return "ERROR: �׽�Ʈ ���������� ������ ������ �����մϴ�.";
	if(parameters.getLength() <= 0) return "ERROR: ��θ� �Է��ϼ���";
	if(	::Core::path_to_be_copied.getLength() <= 1) return "ERROR: ������ ������ �����ּ���";

	NENodeCodeSet* ncs = 0;
	NEModuleCodeSet* mcs = 0;
	type_result result = RESULT_SUCCESS;

	NEObject* parent = 0;
	type_index index;
	_searchParent(parameters[0], index, &parent);

	NEObject& source = ::Core::getObjectBy(::Core::path_to_be_copied);
	NEObject& target = ::Core::getObjectBy(parameters[0]);


	//	main:
	if( ! _pasteTryEverything<NENodeCodeSet, NENode>(target, *parent, source, NEType::NENODE, index))
		if( ! _pasteTryEverything<NEModuleCodeSet, NEModule>(target, *parent, source, NEType::NEMODULE, index))
			if( ! _pasteTryEverything<NEKeyCodeSet, NEKey>(target, *parent, source, NEType::NEKEY, index))
				return NEString("ERROR: �־��� Ÿ��(") + target.getTypeName() + ")�� ����(" + 
				source.getTypeName() + ") ������ Paste�� �Ұ��� �մϴ�.";


	//	post:
	if(::Core::is_cutting_off)
	{
		::Core::commander.command("delete " + ::Core::path_to_be_copied);
		::Core::setPathToBeCopied("");
	}
	return (NEResult::hasError(result)) ? "ERROR: ���� ���� ������ �߻��߽��ϴ�." : "";
}

NE::NEString RunCommand::execute(const NEStringSet& parameters)
{
	NEEventHandler& handler = Editor::getInstance().getEventHandler();

	if(parameters.getLength() <= 0)
	{
		if(handler.isTestRunning())
		{
			handler.resumeTest();
			::Core::test_running_count = -1;
		}
		else
		{
			handler.initiateTest();
			_initiateDebug();
			::Core::test_running_count = -1;
		}
	}
	else
	{
		if(parameters[0] == "-new")
		{
			handler.initiateTest();
			_initiateDebug();
			::Core::test_running_count = parameters.getLength() > 1 ? parameters[1].toInt() : -1;
		}
		else if(parameters[0] == "-stop")
		{
			handler.stopTest();
			::Core::commander.command("observe -script");
			::Core::test_running_count = 0;
		}
		else
		{
			::Core::test_running_count = parameters[0].toInt();
			if(handler.isTestRunning())
				handler.resumeTest();
			else
			{
				handler.initiateTest();
				_initiateDebug();
			}
		}
	}

	return "";
}
void RunCommand::_initiateDebug()
{
	::Core::initializeWindows(::Core::script_windows);
	::Core::initializeWindows(::Core::debug_windows);
	::Core::commander.command("observe -debug -force");
}

NE::NEString SaveCommand::execute(const NEStringSet& parameters)
{
	NEString filename = (parameters.getLength() <= 0) ?
		Kernal::getInstance().getScriptManager().getScriptFilePath()
		:
	parameters[0];

	NEEventHandler& handler = Editor::getInstance().getEventHandler();
	if(NEResult::hasError(handler.saveScriptFile(NETString(filename))))
		return NEString("ERROR: ") + filename + "���� ���� ���� ����.";

	return "";
}
NE::NEString LoadCommand::execute(const NEStringSet& parameters)
{
	if(parameters.getLength() <= 0) return "ERROR: �о���� ���ϸ��� �Է����ּ���.";

	class Really : public LG::QueryWindow
	{
	public:
		Really(const NEString& path) 
			: LG::QueryWindow("������� �ʴ� ������ ������µ�, �����ھ��?", LIGHTRED, RED, true),
			_filepath(path) {}
		virtual NEObject& clone() const { return *(new Really(*this)); }
		virtual void onButtonPressed(bool witch_button)
		{
			if(witch_button)
			{
				NEEventHandler& handler = Editor::getInstance().getEventHandler();
				if(NEResult::hasError(handler.loadScriptFile(NETString(_filepath))))
					::Core::pushMessage("ERROR: ���� �ε� ����.");
				else
				{
					::Core::initializeWindows(::Core::debug_windows);
					::Core::initializeWindows(::Core::script_windows);
					::Core::commander.command("observe -script -force");
				}
			}

			delete_me = true;
		}

		NEString _filepath;
	};

	LG::Core::open(Really(parameters[0]));

	return "";
}
NE::NEString NewCommand::execute(const NEStringSet& parameters)
{
	class Really : public LG::QueryWindow
	{
	public:
		Really() : LG::QueryWindow("������� �ʴ� ������ ������µ�, �����ھ��?", LIGHTRED, RED, true) {}
		virtual NEObject& clone() const { return *(new Really(*this)); }
		virtual void onButtonPressed(bool witch_button)
		{
			if(witch_button)
			{
				NEEventHandler& handler = Editor::getInstance().getEventHandler();
				if(NEResult::hasError(handler.initializeNewFile()))
				{
					::Core::pushMessage("ERROR: ���ο� ���� ���� ����.");
					return;
				}

				::Core::initializeWindows(::Core::debug_windows);
				::Core::initializeWindows(::Core::script_windows);
				::Core::commander.command("observe -script -force");
			}

			delete_me = true;
		}
	};

	LG::Core::open(Really());

	return "";
}
NE::NEString HelpCommand::execute(const NEStringSet& parameters)
{
	switch(parameters.getLength())
	{
	case 0:
		LG::Core::open(GuideEncyclo());
		break;

	case 1:
		if(parameters[0] == "-module")
			LG::Core::open(ModuleEncyclo());	
		else if(parameters[0] == "-command")
			LG::Core::open(CommandListWindow());
		break;

	case 2:		
		if(parameters[0] == "-module")
		{
			NEObject& obj = ::Core::getObjectBy(parameters[1]);
			if( ! obj.isSubClassOf(NEType::NEMODULE))
				return NEString("ERROR: �־��� ���(") + parameters[1] + ")�� ����� �ƴմϴ�.";

			LG::Core::open(ModuleEncyclo(static_cast<NEModule*>(&obj)));
		}
		break;

	default:
		return "ERROR: ����� �߸��Ǿ����ϴ�.";
	}

	return "";
}
NE::NEString HeaderCommand::execute(const NEStringSet& parameters)
{
	LG::Core::open(HeaderModifier());
	return "";
}
NE::NEString ObserveCommand::execute(const NEStringSet& parameters)
{
	if(parameters.getLength() <= 0)
	{
		if(::Core::isObservingDebug())
			_switchTo(::Core::script_windows);
		else
			_switchTo(::Core::debug_windows);
	}
	else
	{
		bool is_forcing = parameters.getLength() >= 2 ? parameters[1] == "-force" : false;
		if(parameters[0] == "-debug")
			_switchTo(::Core::debug_windows, is_forcing);
		else if(parameters[0] == "-script")
			_switchTo(::Core::script_windows, is_forcing);
	}

	return "";
}
void ObserveCommand::_switchTo(LG::WindowList& windows, bool is_forcing)
{
	if( ! is_forcing && windows.getLength() <= 0)
		return;

	LG::Core::setWindowList(windows);
}
NEString ExitCommand::execute(const NEStringSet& parameters)
{
	LG::Core::open(Really());

	return "";
}
