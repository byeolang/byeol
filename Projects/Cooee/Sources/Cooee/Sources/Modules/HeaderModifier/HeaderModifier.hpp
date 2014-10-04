#pragma once

#include "../../Includes/Includes.hpp"

/*
	ScriptHeader�� ����:
		Name,
		Developer,
		Revision,
		ReleaseDate,
		Version,
		Comment,
		Contact,
		MaxNameCodeIndex,		// AUTO
		MaxScriptCodeIndex,		// AUTO
		MaxGroupCodeIndex,		// AUTO
		MaxPriorityCodeIndex,	// AUTO
		ModuleLength,			// AUTO
		KeyLength,				// AUTO
		NodeLength,				// AUTO
*/

class HeaderModifier : public LG::ListWindow
{
public:
	HeaderModifier() 
		: LG::ListWindow("Header Modifier", 25, 6, 30, 11, BLACK, WHITE, WHITE, LIGHTGREEN)
	{
		codes_display_indexes.create(4);
		codes_display_indexes.push(-1);
		codes_display_indexes.push(-1);
		codes_display_indexes.push(-1);
		codes_display_indexes.push(-1);
	}
	HeaderModifier(const HeaderModifier& rhs)
		: LG::ListWindow(rhs), codes_display_indexes(rhs.codes_display_indexes) {}
	FUNC_CLONE(HeaderModifier)
	virtual void onUpdateData()
	{
		ListWindow::onUpdateData();

		NEScriptManager::ScriptHeader& h = Editor::getInstance().getScriptEditor().getScriptHeader();

		list.items.create(6+codes_display_indexes.getLength());
		list.items.push("�̸�\t:"	+ h.getName());
		list.items.push("������\t:"	+ h.getDeveloper());
		list.items.push("����Ƚ��\t:"+ h.getRevision());
		list.items.push("�����\t:"	+ h.getReleaseDate());
		list.items.push("����\t:"	+ h.getComment());
		list.items.push("����ó\t:"	+ h.getContact());

		_pushCodeLists();
	}

	void _pushCodeLists();
	NEString _generateCodeListNames(int index);

	class StringInputWindow : public LG::InputWindow
	{
	public:
		StringInputWindow(NETString& data)
			: LG::InputWindow("���ο� ���� �Է��ϼ���", BLACK, YELLOW, data),
			string_to_be_changed(data) {}
		StringInputWindow(const StringInputWindow& rhs)
			: LG::InputWindow(rhs), string_to_be_changed(rhs.string_to_be_changed) {}
		
		virtual void onInputed()
		{
			string_to_be_changed = input.text;
		}

		NETString& string_to_be_changed;
	};

	//	������ �ڵ忡 ���Ͽ� ��� �۾��� ������ �������� �����ϴ� �޴�
	class CodePopUpMenu : public LG::ListWindow
	{
	public:
		CodePopUpMenu(NECodeType::CodeType codetype, type_index code)
			: LG::ListWindow("Code PopUp Menu", 40, 9, 60, 5, BLACK, WHITE, WHITE, LIGHTBLUE), _codetype(codetype), _code(code) {}
		CodePopUpMenu(const CodePopUpMenu& rhs)
			: LG::ListWindow(rhs), _codetype(rhs._codetype), _code(rhs._code) {}
		FUNC_CLONE(CodePopUpMenu)
		FUNC_TO_CALLER(HeaderModifier)
		
		virtual void onUpdateData()
		{
			LG::ListWindow::onUpdateData();

			if(_codetype == NECodeType::SCRIPT)
			{
				list.items.create(1);
				list.items.push("�̸� �����ϱ�");
			}
			else
			{
				list.items.create(3);
				list.items.push("[" + NEString(_code) + "]�� ���ο� �ڵ� ����");
				list.items.push("�̸� �����ϱ�");
				list.items.push("�����ϱ�");
			}
			
		}

		virtual void onItemChoosed(type_index index, const NEString& chosen_content);
		void _onModifyCodeName();
		void _onAddNewCode();
		void _onRemoveCode();

		NECodeType::CodeType _codetype;
		type_index _code;
	};
	
	virtual void onItemChoosed(type_index index, const NEString& chosen_content);
	virtual void onKeyPressed(char inputed);

	NEIntSet codes_display_indexes;
};