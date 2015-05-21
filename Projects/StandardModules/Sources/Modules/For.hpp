#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL For : public NEModule
	{
	public:
		typedef For ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEIntKey>			arg_iterator;
		NETArgument<NEIntKey>			arg_start_index;
		NETArgument<NEIntKey>			arg_end;
		NETArgument<NEModuleCodeSetKey> arg_actions;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_iterator.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_start_index.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_end.setValue(1);
			arg_end.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			int n		= arg_start_index.getValue(),
				end_n	= arg_end.getValue();
			for(arg_iterator.setValue(n)	; 
				n < end_n					; 
				arg_iterator.setValue(++n)	)
				arg_actions.getValue().execute();

			return arg_iterator.setValue(0);
		}

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_iterator);
			tray.push(arg_start_index);
			tray.push(arg_end);
			tray.push(arg_actions);

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "For";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/07";
				_instance.getComment() =
					"Iterator Argument�� Start���� End ���� ����������(Start ~ End-1����) �ϳ��� �������ѳ����鼭, Actions Argument�� �����մϴ�.\n"
					"C �������� ġ�� ������ �����ϴ�.\n"
					"\tfor( int Iterator = Start ; Iterator <= End ; Iterator++)\n"
					"\t\tActions;";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(4);
				args.push("Iterator\nFor���� �ѹ� ��ȯ(����)�� �������� 1�� �����մϴ�.");
				args.push("Start\nIterator�� �ʱⰪ�Դϴ�.");
				args.push("End\nIterator�� ���� �� ������ ���������� ��ȯ�ϰ� �˴ϴ�.");
				args.push("Actions\n�Ź� ��ȯ�Ҷ����� ����˴ϴ�.");					
			}

			return _instance;
		}

		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}