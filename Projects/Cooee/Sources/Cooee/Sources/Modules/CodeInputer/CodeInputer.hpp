#pragma once

#include "../../Includes/Includes.hpp"

class CodeInputer : public LG::InputWindow
{
public:
	virtual NEObject& clone() const { return *(new CodeInputer(*this)); }
	CodeInputer(NECode& target) 
		: LG::InputWindow("���� �߰��� CODE�� �Է��ϼ���.", BLACK, WHITE),
		_target(target)
	{

	}

	//	������ true ��ȯ
	bool pushHistoryWithBank()
	{
		NEBank& bank = Editor::getInstance().getScriptEditor().getBanks().getBank(_target);
		if (!&bank)
			return false;

		NETString& name = bank[_target.getCode()];
		if (!&name)
		{
			text = "�߸��� ��ũ �ε����Դϴ�. ����.";
			return true;
		}

		input.history.release();
		for (NETStringList::Iterator* e = bank.getIterator(0); e; e = e->getNext())
			input.history.push(e->getValue());

		input.history_idx = _target.getCode();

		return true;
	}

	bool pushHistoryWithScriptSet()
	{
		if(_target.getCodeType() != NECodeType::MODULE_SCRIPT) return false;
		const NEModuleSet& ms = Kernal::getInstance().getModuleManager().getModuleSet();

		input.history.release();
		for (int n = 0; n < ms.getLength(); n++)
			input.history.push(ms[n].getHeader().getName());		

		return true;
	}

	virtual void onFocused()
	{
		if( ! pushHistoryWithBank())
			if( ! pushHistoryWithScriptSet())
				if (_target == NECodeType::MODULE_NAME)
					text = "MODULE_NAME �� Code�� �Է��ϼ���.";
				else
					text = "�� �� ���� Code�� Ȥ�� Type �Դϴ�.";

		input.history_idx = _target.getCode() > input.history.getLengthLastIndex() ? input.history.getLengthLastIndex() : _target.getCode();
	}
	virtual void onUpdateData()
	{
		NEString& history = input.history[input.history_idx];
		if (&history)
			input.text = history;
	}

	virtual void onInputed()
	{
		int code = input.history_idx;
		if (code == -1)
			code = input.text.toInt();
		_target.setCode(code);

		delete_me = true;
	}

private:
	NECode& _target;
};