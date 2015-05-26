#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL DoOnce : public NEModule
	{
	public:
		typedef DoOnce ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEIntKey>			arg_count;
		NETArgument<NEModuleCodeSetKey> arg_actions;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_count.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_count.setValue(1);
			arg_actions.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_count);
			tray.push(arg_actions);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{	
			if(arg_count.getValue() <= 0)
				return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			arg_count.getKey()--;
			return arg_actions.getValue().execute();
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "DoOnce";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/05/26";
				_instance.getComment() = 
					"Ư�� ������ Ƚ��(FrameCount)��ŭ NEModuleCodeSet�� �����մϴ�.\n";
					"�ܼ��� �ݺ������� �ൿ�� �ٸ��ϴ�. �������, FrameCount�� 3�� ���, 3 Frame(��� Module�� 1�� ����Ǵ� �� 1 �������̶� �մϴ�) ���� Action�� ����˴ϴ�.\n"
					"�ݸ鿡 \"For\" Module�� start index = 0, end index = 2�� �� ���� �� Frame���� ���������� Actions�� 3�� ����Ǵ� ����� ���ɴϴ�.\n";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(2);
				args.push("FrameCount\n����� ���� Actions�� �� Frame�� 1�� ����˴ϴ�. Actions�� ����� ������ FrameCount�� 1 �����մϴ�.");
				args.push("Actions\nFrameCount�� ����̸� ����˴ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}