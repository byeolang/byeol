#pragma once

#include "NamedUnitTagger.hpp"

namespace NE
{
	class NE_DLL KeyTagger : public NEModule
	{
	public:
		typedef KeyTagger ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKeySelector>	arg_targets;
		NETArgument<NETStringKey>	arg_name;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);
			tray.push(arg_name);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_name.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_name.setEnable(false);
			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_result result = RESULT_SUCCESS;
			NEKeySelector& ks = arg_targets.getValue();
			while (NEKey* n = &ks.getKey())
			{
				if(arg_name.getPurpose() == NEArgumentBase::READ_BY)
					n->getName() = arg_name.getValue();
				else
					result |= arg_name.setValue(n->getName());
			}

			return result;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;
			static bool is_triggered_once = false;

			if (!is_triggered_once)
			{
				is_triggered_once = true;
				NEExportable::ModuleHeader supers = SuperClass::getHeader();

				_instance.getName() = "KeyTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"������ Module ��ü���� Ư�� �Ӽ������� �������ų�(get) �Ҵ��� �� �ֽ��ϴ�(set).\n"
					"\tTargets:\t������ ��ü��\n"
					"\tKeyName:\tŰ�� �̸�";
					supers.getComment();
				//"�־��� ��ü���� ������ ���� �Ӽ��鿡 ���ؼ� ���� �������ų� �Ҵ� �� �� �ֽ��ϴ�.\n"
				//"\tEnable:\t��ü�� Ȱ��ȭ ����.\n"
				//"\tCodes:\t��ü�� �־��� Code����";					
				NETStringSet args = supers.getArgumentsComments();
				args.resize(4);
				args.pushFront("KeyName\nŰ�� �̸��Դϴ�.");
				args.pushFront("Targets\n�Ӽ��� �������ų� �Ҵ��ϰԵ� Module ���Դϴ�.");
			}

			return _instance;
		}
	};
}