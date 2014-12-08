#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL Ager : public NEModule
	{
	public:
		typedef Ager ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEFloatKey> arg_target;
		NETArgument<NEFloatKey> arg_value;
		NETArgument<NEIntKey> arg_way;
		NETArgument<NEFloatKey> arg_top_border;
		NETArgument<NEFloatKey> arg_bottom_border;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_top_border.setEnable(false);
			arg_bottom_border.setEnable(false);			

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArgument(NEArgumentSet& tray)
		{
			tray.push(arg_target);
			tray.push(arg_value);
			tray.push(arg_way);
			tray.push(arg_top_border);
			tray.push(arg_bottom_border);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			arg_target.setValue(arg_target.getValue() + arg_value.getValue());

			if(	arg_top_border.isEnable()						&&
				arg_target.getValue() > arg_top_border.getValue()	)
				arg_target.setValue(arg_top_border.getValue());
			if (arg_bottom_border.isEnable()					&&
				arg_target.getValue() < arg_bottom_border.getValue())
				arg_target.setValue(arg_bottom_border.getValue());				

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Ager";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/08/16";
				_instance.getComment() = "������ Ű�� ���� �Ź� �����Ӹ��� ������ ������ ��ȭ��ŵ�ϴ�.\n���� ������ �ϸ� �ż��� ������ Ű�� ���� ���ҵǰ� �� �� �ֽ��ϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("Target\nSet�� Key�� ������� �ʽ��ϴ�.");
				args.push("Delta\n�� �����Ӹ��� Target�� ������ų ���Դϴ�. ����� �Է��ϸ� ������, ������ �Է��ϸ� ���Ұ� �˴ϴ�.");
				args.push("\nDelta�� ������ Function\n����� ���� 1�� �Լ��θ� ��ȭ�Ǹ�, �̰��� ������ �ʽ��ϴ�.\n������� ��ȭ��ų���� -5�̸� �� �����Ӹ��� Ű�� -5�� �������ϴ�.");
				args.push("���Ѱ�\nTarget�� �����ɶ� �� ������ Ŀ���� �ʵ��� ����ϴ�.\ndisabled �Ǹ� �������� �ʽ��ϴ�.");
				args.push("���Ѱ�\nTarget�� ���ҵɶ� �� ������ ���� �ʵ��� ����ϴ�.\ndisabled�Ǹ� �������� �ʽ��ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}