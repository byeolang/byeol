#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL If : public NEModule
	{
	public:
		typedef If ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEModuleCodeSetKey> arg_condition;
		NETArgument<NEIntKey>			arg_logic;
		NETArgument<NEBooleanSetKey>	arg_flags;
		NETArgument<NEModuleCodeSetKey> arg_action;
		NETArgument<NEModuleCodeSetKey> arg_else;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_else.setEnable(false);
			arg_flags.setEnable(false);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_condition);
			tray.push(arg_logic);
			tray.push(arg_flags);
			tray.push(arg_action);
			tray.push(arg_else);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			if(_judgeConditions())
				return _executeModules(arg_action);

			return _executeModules(arg_else);
		}
	private:
		type_result _executeModules(NETArgument<NEModuleCodeSetKey>& modules)
		{
			if (modules.isEnable())
				return modules.getValue().execute();

			return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		}
		bool _judgeConditions()
		{
			NEModuleCodeSet& conditions	= arg_condition.getValue();
			NEBooleanSet& flags				= arg_flags.getValue();
			type_int arg_logic				= arg_logic.getValue();
			if(arg_logic == 4) return true;	//	ALWAYS

			for(int n=0; n < conditions.getLength() ;n++)
			{
				type_bool	flag = (flags.isEnable() && n <= flags.getLengthLastIndex()) ?
					flags[n] :
				true;
				type_bool	condition = NEResult::hasTrue(conditions[n].execute());

				if( ! arg_logic.getValue())
				{
					if(flag == condition)
						return true;
				}
				else
				{	//	And ����:
					if(flag != condition)
						return false;
				}
			}

			if(arg_logic.getValue())	
				return false;	//	OR���꿡�� ���δ� FALSE�� ���´ٸ�
			else
				return true;	//	AND���꿡�� ���δ� TRUE�� ������
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "If";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2012/04/14";
				_instance.getComment() =
					"Ư���� Module�� �����Ͽ� �� ���� TRUE���� FALSE������ ���� ���� �ٸ� ����� �����ϰ� �մϴ�.\n"
					"Logic�� True�� �Ǹ�, ACTIONS�� �����ϰ�, FALSE�� �Ǹ� ELSES�� �����ϳ�, disabled�Ǹ� �������� �ʽ��ϴ�.\n"
					"Flags[n] == Conditions[n] �� ��������� ����, Logic���� ���������� TRUE / FALSE�� �����մϴ�."
					"Logic �� 5������ �ֽ��ϴ�.\n"
					"\t0: ��δ� ���� �� (AND TRUE)""\t1: ��δ� �����϶� (AND FALSE)\n"
					"\t2: �ϳ��� ���� �� (OR TRUE)""\t3: �ϳ��� �����϶� (OR FALSE)\n"
					"\t4: �׻� (ALWAYS)\n"
					"�������, Flags Enable, Flags[0] = TRUE, Flags[1] = FALSE, Condtions[0] = TRUE, conditions[1] = TRUE �̸�,\n"
					"\tLogic�� \"AND TRUE\" �϶� Flags[1] != conditions[1] �̹Ƿ� Elses ����� �����մϴ�. �ݸ� \"OR FALSE\"�̸� conditions[1] != Flags[1]�̹Ƿ� Actions�� �����մϴ�.\n"
					"����� Conditons�� ������ Module��, �����ڰ� ���� ���� ���� ������ FALSE�� �⺻���Դϴ�.\n";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("Conditions\n�������Դϴ�. �� ������ �����ϰ�, �� ��ȯ���� TRUE���� FALSE���� Ȯ���մϴ�.");
				args.push("Logic \"Conditions�� �ִ� ����� ����� ���� TRUE�� Actions��, FALSE�� ELSES�� �����մϴ�.\n0 : \"AND TRUE\"   1:\"AND FALSE\"    2:\"OR TRUE\"    3:\"OR FALSE\"    4:\"ALWAYS\"");
				args.push("Flags\nEnable �ɶ��� �����մϴ�. �� Flag�� Conditions�� ������� �񱳿����մϴ�.\nCondition[n]�� ����� Flags[n]�� ������ TRUE�� �ǰ�, �̷� ������ ���� Logic���� ���� �Ǵ��մϴ�.");
				args.push("Actions\nLogic�� ���������� TRUE�϶� �����ϴ� Module ���Դϴ�.\ndisabled �Ǹ� �������� �ʽ��ϴ�.");
				args.push("Elses\nLogic�� ���������� False�϶� �����ϴ� Module ���Դϴ�.\ndisabled �Ǹ� �������� �ʽ��ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}