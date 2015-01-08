#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL Children : public NEModule
	{
	public:
		typedef Children ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NENodeCodeSetKey> arg_children;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_children);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_result result = RESULT_SUCCESS;
			if (arg_children.isEnable())
				result = arg_children.getValue().execute();

			return result;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Children";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/01/18";
				_instance.getComment() = "�־��� �ڽ� Node���� ���� �� �ְ� �մϴ�. execute��, NodeCodeSet�� Children���ν� �����մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(1);
				args.push("Children\n�ڽ� Node���Դϴ�.\ndisabled �Ǹ� execute���� �ʽ��ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}