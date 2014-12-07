#pragma once

#include "../../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL AgingModule : public NEModule
	{
	public:
		typedef AgingModule ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEFloatKey> target;
		NETArgument<NEFloatKey> value;
		NETArgument<NEIntKey> way;
		NETArgument<NEFloatKey> top_border;
		NETArgument<NEFloatKey> bottom_border;

	protected:
		virtual type_result _onFetchArgument(NEArgumentInterfaceList& tray)
		{
			tray.push(target);
			tray.push(value);
			tray.push(way);
			tray.push(top_border);
			tray.push(bottom_border);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			target.getValue() += value.getValue();

			if(	top_border.getConcreteInstance().isBinded()	&&
				target.getValue() > top_border.getValue())
				target.getValue() = top_border.getValue();
			if (bottom_border.getConcreteInstance().isBinded() &&
				target.getValue() < bottom_border.getValue())
				target.getValue() = bottom_border.getValue();				

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Aging";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/08/16";
				_instance.getComment() = "������ Ű�� ���� �Ź� �����Ӹ��� ������ ������ ��ȭ��ŵ�ϴ�.\n���� ������ �ϸ� �ż��� ������ Ű�� ���� ���ҵǰ� �� �� �ֽ��ϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("Target\nSet�� Key�� ������� �ʽ��ϴ�.");
				args.push("Delta\n�� �����Ӹ��� Target�� ������ų ���Դϴ�. ����� �Է��ϸ� ������, ������ �Է��ϸ� ���Ұ� �˴ϴ�.");
				args.push("\nDelta�� ������ Function\n����� ���� 1�� �Լ��θ� ��ȭ�Ǹ�, �̰��� ������ �ʽ��ϴ�.\n������� ��ȭ��ų���� -5�̸� �� �����Ӹ��� Ű�� -5�� �������ϴ�.");
				args.push("���Ѱ�\nTarget�� �����ɶ� �� ������ Ŀ���� �ʵ��� ����ϴ�.\nŰ�� ���� ������ �������� �ʽ��ϴ�.");
				args.push("���Ѱ�\nTarget�� ���ҵɶ� �� ������ ���� �ʵ��� ����ϴ�.\nŰ�� ���� ������ �������� �ʽ��ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}