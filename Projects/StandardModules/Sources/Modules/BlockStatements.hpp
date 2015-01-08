#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL BlockStatements : public NEModule
	{
	public:
		typedef BlockStatements ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEModuleCodeSetKey> arg_actions;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_actions);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_result result = RESULT_SUCCESS;
			if(arg_actions.isEnable())
				result = arg_actions.getValue().execute();

			return result;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "BlockStatements";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/01/18";
				_instance.getComment() = "�־��� ModuleCodeSet�� Actions�ν� �����մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(1);
				args.push("Actions\n�����ϴ� Module ���Դϴ�.\ndisabled �Ǹ� �������� �ʽ��ϴ�.");				
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}