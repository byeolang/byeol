#pragma once

#include "../InputTerminal/InputTerminal.hpp"
#include "../FilterModifier/FilterModifier.hpp"

template <typename KEY>
class Modifier : public InputTerminal
{
public:
	Modifier(KEY& key)
		: InputTerminal("", "�־��� Ű�� ���ο� ���� �Է��ϼ���", KEY().getType()),
		value(key.getValue())
	{

	}
	Modifier(typename KEY::Trait& new_value)
		: InputTerminal("", "�־��� Ű�� ���ο� ���� �Է��ϼ���", NEType::NEKEY), 
		value(new_value)
	{

	}
	Modifier(const Modifier& rhs) : InputTerminal(rhs), value(rhs.value) {}

	virtual void onUpdateData()
	{
		textbox.text = value;
	}

	virtual void onTextInputed()
	{		
		KEY temp(value);
		temp = NEStringKey(textbox.text);
		value = temp.getValue();
	}

	typename KEY::Trait& value;
	FUNC_CLONE(Modifier<KEY>)
};

template <>
class Modifier<NEBooleanKey> : public Terminal
{
public:
	Modifier(NEBooleanKey& key);
	Modifier(NEBooleanKey::Trait& new_value);
	Modifier(const Modifier& rhs);

	virtual void onUpdateData();
	virtual void onKeyPressed(char inputed);
	FUNC_CLONE(Modifier<NEBooleanKey>)

	LG::SwitchGliph sw;
	NEBooleanKey::Trait& value;
};

template <>
class Modifier<NENodeSelector> : public FilterModifier
{
public:
	Modifier(NENodeSelector& new_key);
	Modifier(const Modifier& rhs);

	class MenuList : public ListGliph
	{
	public:
		MenuList() : ListGliph(0, 25, 18, 30, 6, BLACK, WHITE, WHITE, LIGHTRED), codelist_display_index(-1) {}
		FUNC_CLONE(MenuList)
		FUNC_TO_OWNER(Modifier<NENodeSelector>)

		NEString createNodeTypeStringFrom(NECodeType::CodeType type);
		const NETStringList& getProperBankFrom(NECodeType::CodeType type);
		void updateList();
		virtual void onUpdateData();

		virtual void onKeyPressed(char inputed);

		int codelist_display_index;
	};

	FUNC_CLONE(Modifier)
	FUNC_TO_OWNER(Planetarium::Filter)

	NENodeSelector& key;
	Gliph header, count_header, bind_header, code_type_header, use_and_header, codelist_header;
	MenuList menulist;
};

template <>
class Modifier<NEModuleSelector> : public FilterModifier
{
public:
	Modifier(NEModuleSelector& new_key)
		: Window(4, 17, 52, 8, LIGHTGRAY, DARKGRAY), key(new_key),
		bind_header		(0, 15, 18, 10, 1, WHITE, LIGHTRED, "ModuleBind?"),
		code_type_header(0, 15, 19, 10, 1, WHITE, LIGHTRED, "ModuleType"),
		codelist_header	(0, 15, 20, 10, 1, WHITE, LIGHTRED, "ModuleCodes")
	{
		regist(4, &bind_header, &code_type_header, &codelist_header, &menulist);
	}
	Modifier(const Modifier& rhs)
		: Window(rhs), key(rhs.key), bind_header(rhs.bind_header), code_type_header(rhs.code_type_header), 
		codelist_header(rhs.codelist_header), menulist(rhs.menulist)
	{
		regist(4, &bind_header, &code_type_header, &codelist_header, &menulist);
	}

	class MenuList : public ListGliph
	{
	public:
		MenuList() : ListGliph(0, 25, 18, 30, 6, BLACK, WHITE, WHITE, LIGHTRED), codelist_display_index(-1) {}
		FUNC_CLONE(MenuList)
			FUNC_TO_OWNER(Modifier<NEModuleSelector>)

			NEString createNodeTypeStringFrom(NECodeType::CodeType type)
		{
			switch(type)
			{
			case NECodeType::ME:		return "ME";
			case NECodeType::SCRIPT:	return "SCRIPT";
			case NECodeType::NAME:		return "NAME";
			case NECodeType::RECENT:	return "RECENT";
			case NECodeType::ALL:		return "ALL";
			default:					return "ERROR! UNKNOWN!";
			}
		}
		void updateList()
		{
			NEModuleSelector& k = toOwner()->key;
			if( ! &k) return;

			items.create(3);
			items.push(NEString("   ") + k.isUsingAutoBinding());
			items.push(NEString("   ") + createNodeTypeStringFrom(k.getModuleType()));

			NEString codes_to_str;
			const NEIntSet& c = k.getModuleCodeSet();

			if(	k.getModuleType() == NECodeType::ALL	||
				k.getModuleType() == NECodeType::RECENT	||
				k.getModuleType() == NECodeType::ME		)
			{
				codes_to_str = "NOT NEEDED!";				
				codelist_display_index = -1;
			}
			else
			{
				switch(codelist_display_index)
				{
				case -2:	codes_to_str = "NEW";	break;
				case -1:	
					codes_to_str = "CODES: ";
					for(int n=0; n < c.getLength() ;n++)
						codes_to_str += NEString(c[n]) + " ";
					break;

				default:
					codes_to_str = NEString("[") + codelist_display_index + "]" + c[codelist_display_index];
					break;
				}
			}

			items.push("   " + codes_to_str);
		}
		virtual void onUpdateData()
		{
			if(items.getLength() < 3)
				updateList();

			for(int n=0; n < 3 ;n++)
			{
				NEString& item = items[n];
				if(n == choosed)
				{
					item[0] = '<';
					item[1] = '-';
					item[2] = ' ';
					item += " ->";
				}
				else if(item.getLength() > 0 && item[0] == '<')
				{
					item[0] = ' ';
					item[1] = ' ';
					item[2] = ' ';
					item.pop();
					item.pop();
					item.pop();
					item[item.getLengthLastIndex()] = '\0';
				}
			}

		}

		virtual void onKeyPressed(char inputed);

		int codelist_display_index;
	};

	FUNC_CLONE(Modifier)

		NEModuleSelector& key;
	Gliph bind_header, code_type_header, use_and_header, codelist_header;
	MenuList menulist;
};

template <>
class Modifier<NEKeySelector> : public FilterModifier
{
public:
	Modifier(NEKeySelector& new_key)
		: Window(4, 17, 52, 8, LIGHTGRAY, DARKGRAY), key(new_key),
		bind_header		(0, 15, 18, 10, 1, WHITE, LIGHTRED, "AutoBind?"),
		keyname_header	(0, 15, 19, 10, 1, WHITE, LIGHTRED, "KeyName")
	{
		regist(3, &bind_header, &keyname_header, &menulist);
	}
	Modifier(const Modifier& rhs)
		: Window(rhs), key(rhs.key), bind_header(rhs.bind_header), keyname_header(rhs.keyname_header), menulist(rhs.menulist)
	{
		regist(3, &bind_header, &keyname_header, &menulist);
	}

	class MenuList : public ListGliph
	{
	public:
		MenuList() : ListGliph(0, 25, 18, 30, 6, BLACK, WHITE, WHITE, LIGHTRED) {}
		FUNC_CLONE(MenuList)
			FUNC_TO_OWNER(Modifier<NEKeySelector>)

			void updateList()
		{
			NEKeySelector& k = toOwner()->key;
			if( ! &k) return;

			items.create(2);
			items.push(NEString("   ") + k.isUsingAutoBinding());
			items.push(NEString("   ") + k.getKeyName());
		}
		virtual void onUpdateData()
		{
			if(items.getLength() < 2)
				updateList();

			for(int n=0; n < 2 ;n++)
			{
				NEString& item = items[n];
				if(n == choosed)
				{
					item[0] = '<';
					item[1] = '-';
					item[2] = ' ';
					if(item.getLength() > 1 && item[item.getLengthLastIndex()-1] != '>')
						item += " ->";
				}
				else if(item.getLength() > 0 && item[0] == '<')
				{
					item[0] = ' ';
					item[1] = ' ';
					item[2] = ' ';
					item.pop();
					item.pop();
					item.pop();
					item[item.getLengthLastIndex()] = '\0';
				}
			}

		}

		virtual void onKeyPressed(char inputed);
	};

	FUNC_CLONE(Modifier)

		NEKeySelector& key;
	Gliph bind_header, keyname_header;
	MenuList menulist;
};

template <>
void Modifier<NENodeSelector>::MenuList::onKeyPressed(char inputed)
{
	ListGliph::onKeyPressed(inputed);
	NENodeSelector& key = toOwner()->key;

	switch(inputed)
	{
	case CONFIRM:
		if(choosed == 5)
		{
			if(codelist_display_index == -2)
			{
				class CodeInputer : public LG::InputWindow
				{
				public:
					FUNC_TO_CALLER(Modifier<NENodeSelector>)
						FUNC_CLONE(CodeInputer)
						CodeInputer() : LG::InputWindow("���� �߰��� CODE�� �¿� ����Ű�� �����ϼ���", BLACK, WHITE) 
					{

					}

					virtual void onUpdateData()
					{
						const NETStringList& bank = toCaller().menulist.getProperBankFrom(toCaller().key.getNodeType());								

						int n=0;
						for(const NETStringList::Iterator* itr=bank.getIterator(0); itr ;itr=itr->getNext())
							input.history.push(itr->getValue() + "(" + n++ + ")");

						input.history_idx = 0;
						input.text = input.history[input.history_idx];
					}
					virtual void onKeyPressed(char inputed)
					{
						switch(inputed)
						{
						case CANCEL:
						case CONFIRM:
						case LEFT:
						case RIGHT:
							InputWindow::onKeyPressed(inputed);
							break;
						}
					}

					virtual void onInputed()						
					{
						NEIntSet copied = toCaller().key.getCodeSet();
						if(copied.getLength() == copied.getSize())
							copied.resize(copied.getSize() + 1);
						copied.push(input.history_idx);
						toCaller().key.setCodeSet(copied);
						toCaller().getPlanetarium().onUpdateData();
						toCaller().getPlanetarium().onDraw();
						toCaller().menulist.items.release();
						toCaller().menulist.onUpdateData();
						toCaller().menulist.onDraw();
						delete_me = true;
					}
				};

				NECodeType::CodeType type = toOwner()->key.getNodeType();
				switch(type)
				{
				case NECodeType::SCRIPT:	case NECodeType::GROUP: 
				case NECodeType::PRIORITY:	case NECodeType::NAME:
					toOwner()->call(CodeInputer());
					break;
				}

			}
			else if(codelist_display_index >= 0)
			{
				NEIntSet copied = toOwner()->key.getCodeSet();
				copied.remove(codelist_display_index);
				copied.resize(copied.getLength());
				toOwner()->key.setCodeSet(copied);
				if(codelist_display_index > copied.getLengthLastIndex())
					codelist_display_index = copied.getLengthLastIndex();

				toOwner()->getPlanetarium().onUpdate();
				toOwner()->getPlanetarium().onDraw();

				items.release();
				onUpdateData();
				onDraw();
			}
		}
		break;

	case UP:
	case DOWN:
		onUpdateData();
		onDraw();
		break;

	case LEFT:
		switch(choosed)
		{
		case 0:	//	ManageType
			switch(key.getManagerType())
			{
			case NEType::NENODE_MANAGER:	
				key.setManager(NEType::NESCRIPT_MANAGER);						
				break;
			case NEType::NESCRIPT_EDITOR:
				key.setManager(NEType::NENODE_MANAGER);
				break;
			}					
			break;

		case 1:	//	Count
			if(key.getCountLimit() > 0)
				key.setCountLimit(key.getCountLimit()-1);
			break;

		case 2:	//	AutoBind
			if(key.isUsingAutoBinding())
				key.isUsingAutoBinding() = false;
			break;

		case 3:	//	CodeType
			switch(key.getNodeType())
			{
			case NECodeType::RECENT:
				key.setNodeType(NECodeType::ME);
				break;
			case NECodeType::SCRIPT:
				key.setNodeType(NECodeType::RECENT);
				break;
			case NECodeType::NAME:
				key.setNodeType(NECodeType::SCRIPT);
				break;
			case NECodeType::GROUP:
				key.setNodeType(NECodeType::NAME);
				break;
			case NECodeType::PRIORITY:
				key.setNodeType(NECodeType::GROUP);
				break;					
			case NECodeType::ALL:
				key.setNodeType(NECodeType::PRIORITY);
				break;
			}
			break;
		case 4:	//	Use &&
			if(key.isUsingAndOperation())
				key.setUsingAndOperation(false);
			break;

		case 5:
			if(codelist_display_index > -2)
				codelist_display_index--;
			break;
		}
		items.release();

		toOwner()->getPlanetarium().onUpdate();
		toOwner()->getPlanetarium().onDraw();
		onUpdateData();
		onDraw();
		break;

	case RIGHT:
		switch(choosed)
		{
		case 0:	//	ManageType
			switch(key.getManagerType())
			{
			case NEType::NESCRIPT_MANAGER:	
				key.setManager(NEType::NENODE_MANAGER);
				break;
			case NEType::NENODE_MANAGER:
				key.setManager(NEType::NESCRIPT_EDITOR);
				break;
			}					
			break;

		case 1:	//	Count
			key.setCountLimit(key.getCountLimit()+1);
			break;

		case 2:	//	AutoBind
			if( ! key.isUsingAutoBinding())
				key.isUsingAutoBinding() = true;
			break;

		case 3:	//	CodeType
			switch(key.getNodeType())
			{
			case NECodeType::ME:
				key.setNodeType(NECodeType::RECENT);
				break;
			case NECodeType::RECENT:
				key.setNodeType(NECodeType::SCRIPT);
				break;
			case NECodeType::SCRIPT:
				key.setNodeType(NECodeType::NAME);
				break;
			case NECodeType::NAME:
				key.setNodeType(NECodeType::GROUP);
				break;
			case NECodeType::GROUP:
				key.setNodeType(NECodeType::PRIORITY);
				break;					
			case NECodeType::PRIORITY:
				key.setNodeType(NECodeType::ALL);
				break;
			}
			break;

		case 4:	//	Use &&
			if( ! key.isUsingAndOperation())
				key.setUsingAndOperation(true);
			break;

		case 5:
			if(codelist_display_index < key.getCodeSet().getLengthLastIndex())
				codelist_display_index++;
		}
		items.release();
		toOwner()->getPlanetarium().onUpdate();
		toOwner()->getPlanetarium().onDraw();
		onUpdateData();
		onDraw();
		break;

	case CANCEL:
		toOwner()->delete_me = true;
	}
}

template <>
void Modifier<NEModuleSelector>::MenuList::onKeyPressed(char inputed)
{
	ListGliph::onKeyPressed(inputed);
	NEModuleSelector& key = toOwner()->key;

	switch(inputed)
	{
	case CONFIRM:
		if(choosed == 2)
		{
			if(codelist_display_index == -2)
			{
				class CodeInputer : public LG::InputWindow
				{
				public:
					FUNC_TO_CALLER(Modifier<NEModuleSelector>)
						FUNC_CLONE(CodeInputer)
						CodeInputer() : LG::InputWindow("���� �߰��� CODE�� �Է��ϼ���.", BLACK, WHITE) 
					{

					}

					virtual void onInputed()
					{
						NECodeSet copied = toCaller().key.getModuleCodeSet();
						if(copied.getLength() == copied.getSize())
							copied.resize(copied.getSize() + 1);
						copied.push(input.text.toInt());
						toCaller().key.setModuleCodeSet(copied);
						toCaller().getPlanetarium().onUpdateData();
						toCaller().getPlanetarium().onDraw();
						toCaller().menulist.items.release();
						toCaller().menulist.onUpdateData();
						toCaller().menulist.onDraw();
						delete_me = true;
					}
				};

				NECodeType::CodeType type = toOwner()->key.getModuleType();
				switch(type)
				{
				case NECodeType::SCRIPT:	case NECodeType::NAME:
					toOwner()->call(CodeInputer());
					break;
				}

			}
			else if(codelist_display_index >= 0)
			{
				NECodeSet copied = toOwner()->key.getModuleCodeSet();
				copied.remove(codelist_display_index);
				copied.resize(copied.getLength());
				toOwner()->key.setModuleCodeSet(copied);
				if(codelist_display_index > copied.getLengthLastIndex())
					codelist_display_index = copied.getLengthLastIndex();

				toOwner()->getPlanetarium().onUpdateData();
				toOwner()->getPlanetarium().onDraw();

				items.release();
				onUpdateData();
				onDraw();
			}
		}
		break;

	case UP:
	case DOWN:
		onUpdateData();
		onDraw();
		break;

	case LEFT:
		switch(choosed)
		{
		case 0:	//	AutoBind
			if(key.isUsingAutoBinding())
				key.isUsingAutoBinding() = false;
			break;

		case 1:	//	CodeType
			switch(key.getModuleType())
			{
			case NECodeType::RECENT:
				key.setModuleType(NECodeType::ME);
				break;
			case NECodeType::SCRIPT:
				key.setModuleType(NECodeType::RECENT);
				break;
			case NECodeType::NAME:
				key.setModuleType(NECodeType::SCRIPT);
				break;				
			case NECodeType::ALL:
				key.setModuleType(NECodeType::NAME);
				break;
			}
			break;

		case 2:
			if(codelist_display_index > -2)
				codelist_display_index--;
			break;
		}
		items.release();
		toOwner()->onUpdateData();
		toOwner()->onDraw();
		break;

	case RIGHT:
		switch(choosed)
		{
		case 0:	//	AutoBind
			if( ! key.isUsingAutoBinding())
				key.isUsingAutoBinding() = true;
			break;

		case 1:	//	CodeType
			switch(key.getModuleType())
			{
			case NECodeType::ME:
				key.setModuleType(NECodeType::RECENT);
				break;
			case NECodeType::RECENT:
				key.setModuleType(NECodeType::SCRIPT);
				break;
			case NECodeType::SCRIPT:
				key.setModuleType(NECodeType::NAME);
				break;
			case NECodeType::NAME:
				key.setModuleType(NECodeType::ALL);
				break;
			}
			break;
		case 2:
			if(codelist_display_index < key.getModuleCodeSet().getLengthLastIndex())
				codelist_display_index++;
		}
		items.release();
		toOwner()->onUpdateData();
		toOwner()->onDraw();
		break;

	case CANCEL:
		toOwner()->delete_me = true;
	}			
}

template <>
void Modifier<NEKeySelector>::MenuList::onKeyPressed(char inputed)
{
	ListGliph::onKeyPressed(inputed);
	NEKeySelector& key = toOwner()->key;

	switch(inputed)
	{
	case CONFIRM:
		if(choosed == 1)
		{			
			class CodeInputer : public LG::InputWindow
			{
			public:
				FUNC_TO_CALLER(Modifier<NEKeySelector>)
					FUNC_CLONE(CodeInputer)

					CodeInputer(const NEStringList& names) : LG::InputWindow("���� �߰��� Ű�� �̸��� �¿����Ű�� �����ϼ���. \n���� ���� �̸��� �Է� �� ���� �־��.", BLACK, WHITE) 
				{
					input.history = names;
					input.history_idx = 0;
					input.text = input.history[input.history_idx];
				}

				virtual void onInputed()
				{
					toCaller().key.setKeyName(input.text);
					toCaller().getPlanetarium().onUpdateData();
					toCaller().getPlanetarium().onDraw();
					toCaller().menulist.items.release();
					toCaller().menulist.onUpdateData();
					toCaller().menulist.onDraw();
					delete_me = true;
				}
			};

			//	ScriptEditor�� �Ŵ��� ����:
			//		����� ���� NodeManager, ScriptManager�� ���� ������, �������� �����Ͱ�
			//		ã���� �Ǿ��ִ� ����, ����� �����͸� ������ �� ����.
			NEType::Type manager_backup = key.getManagerType();
			key.setManager(NEType::NESCRIPT_EDITOR);

			//	���� Selector�� ������ �� �ִ� ���鿡 �ִ� ��� Ű �̸��� Preset���� ������:
			NEStringList bucket;
			NENode* itr=0;
			while(itr = &key.getNode())
			{
				NEKeyCodeSet& ks = itr->getKeySet();

				for(int n=0; n < ks.getLength() ;n++)
					if(bucket.find(ks[n].getName()) == NE_INDEX_ERROR)
						bucket.push(ks[n].getName());
			}

			key.initializeReferingPoint();
			key.setManager(manager_backup);
			toOwner()->call(CodeInputer(bucket));					
		}
		break;

	case UP:
	case DOWN:
		onUpdateData();
		onDraw();
		break;

	case LEFT:
		if(choosed == 0)
		{
			key.isUsingAutoBinding() = false;
			items.release();
			onUpdateData();
			onDraw();		
		}				
		break;

	case RIGHT:
		if(choosed == 0)
		{
			key.isUsingAutoBinding() = true;
			items.release();
			onUpdateData();
			onDraw();
		}
		break;

	case CANCEL:
		toOwner()->delete_me = true;
	}			
}