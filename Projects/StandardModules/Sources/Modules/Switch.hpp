#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL Switch : public NEModule
	{
	public:
		typedef Switch ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEModuleCodeSetKey> arg_actions;
		NETArgument<NEIntKey>			arg_state;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_actions.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_state.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_actions);
			tray.push(arg_state);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_index n = arg_state.getValue();
			NEModuleCodeSet& ms = arg_actions.getValue();
			if (n >= ms.getLength())
				return ALERT_WARNING(" : State = %d�� ���� Actions�� �ִ� ���� = %d���� ���ų� Ů�ϴ�.", n, ms.getLength());

			return ms[n].execute();
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Switch";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/06/10";
				_instance.getComment() = "State ���� �ش��ϴ� Actions[n]�� �����մϴ�. ";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("Actions\nState ���� ��ġ�ϴ� Actions �迭���Ҹ� �����մϴ�.");
				args.push("State\switch(State)");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}