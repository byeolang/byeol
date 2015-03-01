#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL NodeSelectorTaggerBase : public NEModule
	{
	public:
		typedef NodeSelectorTaggerBase ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEIntKey>		arg_manager_type;
		NETArgument<NECodeSetKey>	arg_codes;
		NETArgument<NEIntKey>		arg_countlimit;
		NETArgument<NEBooleanKey>	arg_use_and_operation;
		NETArgument<NEBooleanKey>	arg_use_auto_binding;
		NETArgument<NEIntKey>		arg_initializer_count;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_manager_type.setEnable(false);
			arg_manager_type.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_codes.setEnable(false);
			arg_codes.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_countlimit.setEnable(false);
			arg_countlimit.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_use_and_operation.setEnable(false);
			arg_use_and_operation.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_use_auto_binding.setEnable(false);
			arg_use_auto_binding.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_initializer_count.setEnable(false);			
			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_manager_type);
			tray.push(arg_codes);
			tray.push(arg_countlimit);
			tray.push(arg_use_and_operation);
			tray.push(arg_use_auto_binding);
			tray.push(arg_initializer_count);

			return RESULT_SUCCESS;
		}
		type_result _passPipes(NENodeSelector& n)
		{
			type_result result = RESULT_SUCCESS;

			if(arg_manager_type.isEnable())
			{
				if (arg_manager_type.getPurpose() == NEArgumentBase::READ_BY)
					result |= n.setManager(NEType::Type(arg_manager_type.getValue()));
				else
					result |= arg_manager_type.setValue(n.getManagerType());
			}
			if(arg_codes.isEnable())
			{
				const NECodeSet& codes = arg_codes.getValue();

				if (arg_codes.getPurpose() == NEArgumentBase::READ_BY)
					result |= n.setCodes(codes);
				else
					result |= arg_codes.setValue(n.getCodes());
			}
			if(arg_countlimit.isEnable())
			{
				if (arg_countlimit.getPurpose() == NEArgumentBase::READ_BY)
					n.setCountLimit(arg_countlimit.getValue());
				else
					result |= arg_countlimit.setValue(n.getCountLimit());
			}
			if(arg_use_and_operation.isEnable())
			{
				if (arg_use_and_operation.getPurpose() == NEArgumentBase::READ_BY)
					n.setUsingAndOperation(arg_use_and_operation.getValue());
				else
					result |= arg_use_and_operation.setValue(n.isUsingAndOperation());
			}			
			if(arg_use_auto_binding.isEnable())
			{
				if (arg_use_auto_binding.getPurpose() == NEArgumentBase::READ_BY)
					n.isUsingAutoBinding() = arg_use_auto_binding.getValue();
				else
					result |= arg_use_auto_binding.setValue(n.isUsingAutoBinding());
			}
			type_int initializer_count = arg_initializer_count.getValue();
			if (initializer_count != 0)
			{
				result |= n.initializeReferingPoint();

				if(initializer_count > 0)
					arg_initializer_count.getValueKey()--;
			}		

			return result;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "NodeSelectorTaggerBase";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/09";
				_instance.getComment() =
					"- Initializer Count�� ������ ���ڸ�ŭ execute�ø��� �ʱ�ȭ ��ŵ�ϴ�."
					"-1�̸�, �������Ѿ��� execute �ɶ����� �׻� �ʱ�ȭ �˴ϴ�."
					"- ManagerType : Selector�� Ž���� Manager�� ������ ���մϴ�.\n"
					"- Node Codes : Node Ž���� ���� Code �Դϴ�."
					"- CountLimit : Node Ž���� �ѹ��� �ִ� Ž���� ���� �Դϴ�."
					"- Use AND Operation : ã�� ��尡 Node Codes�鿡 ��� �ش�Ǿ�� �ϴ����� �ǹ��մϴ�.\n";

				NETStringSet& args = _instance.getArgumentsComments();
				args.create(6);
				args.push("ManagerType");
				args.push("Codes for Node Selector");
				args.push("CountLimit for Node Selector");
				args.push("UseAndOperation for Node Selector");
				args.push("UseAutoBinding for Node Selector");
				args.push("InitializerCount");
			}

			return _instance;
		}
	};
}