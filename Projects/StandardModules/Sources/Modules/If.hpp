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
		NETArgument<NEModuleCodeSetKey> arg_action;
		NETArgument<NEModuleCodeSetKey> arg_else;
		NETArgument<NEBooleanSetKey>	arg_flags;
		NETArgument<NEBooleanKey>		arg_logic;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_else.setEnable(false);

			return RESULT_SUCCESS;
		}
<<<<<<< HEAD
		virtual type_result _onFetchArguments(NEArgumentList& tray)
=======
		virtual type_result _onFetchArgument(NEArgumentSet& tray)
>>>>>>> 7cc208e99f52313036b5accd74a9b207b90d2e65
		{
			tray.push(arg_condition);
			tray.push(arg_flags);
			tray.push(arg_logic);
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
			if(arg_condition.isEnable())
				return arg_condition.getValue().execute();

			return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		}
		bool _judgeConditions()
		{
			NEModuleCodeSet& conditions	= arg_condition.getValue();
			NEBooleanSet& flags			= arg_flags.getValue();


			for(int n=0; n < conditions.getLength() ;n++)
			{
				type_bool	flag = n <= flags.getLengthLastIndex() ? flags[n] : true,
					condition = NEResult::hasTrue(conditions[n].execute());

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
					"Flags�� \"Conditions�� Ư�� �ε����� ��ġ�� Module�� �����\"�� Flags�� �� �ε����� ���ҿ� ��ġ�ϴ� ���� �Ǵ��մϴ�.\n"
					"Logic�� Flags�� Conditions�� ���� ������ or Ȥ�� and�� �����մϴ�.\n"
					"\t�������, Flags[0] = TRUE, Flags[1] = FALSE, Condtions[0] = TRUE, conditions[1] = TRUE �̸�,\n"
					"\tLogic�� \"or\" �϶� Actions ����� �����մϴ�. �ݸ� \"and\"�̸� Elses�� enable�� �Ǿ������� Elses�� �����մϴ�.\n"
					"����� Module�� �����ڰ� ��ȯ���� �������� ���� ��� FALSE�� �⺻���̸�,\n"
					"Flags�� �������� ���� ���, TRUE�� �������ϴ�.\n";					
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("Conditions\n�������Դϴ�. �� ������ �����ϰ�, �� ��ȯ���� TRUE���� FALSE���� Ȯ���մϴ�.");
				args.push("Flags\n�� Boolean �ε����� Conditions�� ���ҿ� ��Ī�˴ϴ�.\nCondition�� �� Module�� �� Boolean���� ��ġ�ϸ� TRUE�� �˴ϴ�. ");
				args.push("Use \"or\" Operation?\n\"or\"�� ������ n�� ���� Condtion[n]�� Flags[n]�� ���� ��ġ�ϴ� ��찡 ������ TRUE�Դϴ�.\n\"and\"��, ��� ��ġ�ؾ� TRUE �Դϴ�.");
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