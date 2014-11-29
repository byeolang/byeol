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
		NETArgument<NEKeySelector> target;
		NETArgument<NEFloatKey> value;
		NETArgument<NEIntKey> way;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(target);
			tray.push(value);
			tray.push(way);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			NEKey& binded = target.getConcreteInstance().getBindedKey();
			if(binded.isSubClassOf(NEType::NEKEY_SELECTOR))
				while(NEKey* key = &target.getValue().getValue())
					*key += value.getKey();
			else
				binded += value.getKey();
			
			return RESULT_SUCCESS;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "AgingModule";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/08/16";
				_instance.getComment() = "������ Ű�� ���� �Ź� �����Ӹ��� ������ ������ ��ȭ��ŵ�ϴ�.\n���� ������ �ϸ� �ż��� ������ Ű�� ���� ���ҵǰ� �� �� �ֽ��ϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(3);
				args.push("��ȭ��ų Ű�� �����մϴ�. Set�� Key�� ������� �ʽ��ϴ�.");
				args.push("�� �����Ӹ��� ��ȭ��ų ���Դϴ�.");
				args.push("��ȭ��Ű�� ����Դϴ�. ����� ���� 1�� �Լ��θ� ��ȭ�Ǹ�, �̰��� ������ �ʽ��ϴ�.\n ������� ��ȭ��ų���� -5�̸� �� �����Ӹ��� Ű�� -5�� �������ϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}