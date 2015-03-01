#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ArgumentTagger : public NEModule
	{
	public:
		typedef ArgumentTagger ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEModuleSelector>	arg_module;
		NETArgument<NEIntSetKey>		arg_args_index;
		NETArgument<NEIntKey>			arg_purpose;
		NETArgument<NETStringKey>		arg_keyname;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_args_index.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_purpose.setEnable(false);
			arg_purpose.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_keyname.setEnable(false);
			arg_keyname.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);			

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			NEModuleSelector& ms = arg_module.getValue();
			type_result result = RESULT_SUCCESS;

			while(NEModule* m = &ms.getModule())
			{
				NEArgumentSet& args = m->getArguments();
				const NEIntSet& is = arg_args_index.getValue();
				for (int n = 0; n < is.getLength(); n++)
				{
					NEArgumentBase& arg = args[is[n]];
					if( ! &arg)
					{
						ALERT_WARNING(" : ");
						continue;
					}

					result |= _passPipes(arg);
				}
			}

			return result;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_module);
			tray.push(arg_args_index);
			tray.push(arg_purpose);
			tray.push(arg_keyname);

			return RESULT_SUCCESS;
		}
		type_result _passPipes(NEArgumentBase& arg)
		{
			type_result result = RESULT_SUCCESS;

			if(arg_purpose.isEnable())
			{
				if(arg_purpose.getPurpose() == NEArgumentBase::READ_BY)
					result |= arg.setPurpose(NEArgumentBase::Purpose(arg_purpose.getValue()));
				else
					result |= arg_purpose.setValue(arg.getPurpose());
			}
			if(arg_keyname.isEnable())
			{
				if(arg_keyname.getPurpose() == NEArgumentBase::READ_BY)
					result |= arg.setKeyName(arg_keyname.getValue());
				else
					result |= arg_keyname.setValue(arg.getKeyName());
			}			

			return result;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "ArgumentTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/05";
				_instance.getComment() =
					"Argument�� �Ӽ����� �������ų�, ������ �� �ֽ��ϴ�.\n"
					"ModuleSelector�� �����ϱ� ���ϴ� Argument�� ������ Module�� ����Ű��,\n"
					"Argument Indexes�� �ش� Module�� Argument Index�� �����ϸ� �˴ϴ�. �������� Argument�� ������ �� �ֽ��ϴ�.\n"
					"Argument�� �Ӽ��� ������ �����ϴ�.\n"
					"\tPurpose:\tArgument�� ��� �����Դϴ�.\n"
					"\tKeyName:\tArgument�� ���� Node�� KeySet �߿���, � Ű�� ���ε� �Ǵ��� �����ϴ� ���Դϴ�.";

				NETStringSet& args = _instance.getArgumentsComments();
				args.create(4);
				args.push("Target Modules\n�����ϰ��� Argument�� ���� ����� ����ŵ�ϴ�.");
				args.push("Arugment Index\nTarget Module�� ���° Argument�� Tagging �� ������ �����մϴ�. �������� �� �� �ֽ��ϴ�.");
				args.push("Argument�� Purpose ��\nInput�� �ϸ� �� ���� �Ф��� Target�� Index�� �ش��ϴ� Argument�� �ֽ��ϴ�.\nOutput�� �ϸ� Target's Argument���� ���� ������ ���⿡ �ֽ��ϴ�.");
				args.push("Argument Name");
			}

			return _instance;
		}

		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}