#pragma once

#include "NodeSelectorTaggerBase.hpp"

namespace NE
{
	class NE_DLL ModuleSelectorTagger : public NodeSelectorTaggerBase
	{
	public:
		typedef ModuleSelectorTagger ThisClass;
		typedef NodeSelectorTaggerBase SuperClass;

	public:
		NETArgument<NEModuleSelector>	arg_target;
		NETArgument<NECodeSetKey>		arg_module_codes;

	protected:
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_module_codes.setEnable(false);
			arg_module_codes.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_target);
			tray.push(arg_module_codes);

			return SuperClass::_onFetchArguments(tray);
		}

	protected:
		using SuperClass::_passPipes;
		type_result _passPipes(NEModuleSelector& m)
		{
			type_result result = RESULT_SUCCESS;

			if(arg_module_codes.isEnable())
			{
				const NECodeSet& codes = arg_module_codes.getValue();

				if (arg_module_codes.getPurpose() == NEArgumentBase::READ_BY)
					m.setModuleCodes(codes);
				else
					result |= arg_module_codes.setValue(m.getModuleCodes());
			}			

			return result;
		}
		virtual type_result _onExecute()
		{
			NEModuleSelector& ms = arg_target.getValue();
			NENodeSelector& ns = ms;

			type_result result = _passPipes(ns);			

			return result |= _passPipes(ms);
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				NEExportable::ModuleHeader supers = SuperClass::getHeader();

				_instance.getName() = "ModuleSelectorTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"������ Module ��ü���� Ư�� �Ӽ������� �������ų�(get) �Ҵ��� �� �ֽ��ϴ�(set).\n"
					"\tTargets:\t������ ��ü��\n"
					"\tModule Codes:Module Ž���� ����ϰԵ� Codes\n" +
					supers.getComment();
				//"�־��� ��ü���� ������ ���� �Ӽ��鿡 ���ؼ� ���� �������ų� �Ҵ� �� �� �ֽ��ϴ�.\n"
				//"\tEnable:\t��ü�� Ȱ��ȭ ����.\n"
				//"\tCodes:\t��ü�� �־��� Code����";					
				NETStringSet& args = _instance.getArgumentsComments();
				const NETStringSet& super_args = supers.getArgumentsComments();

				args.create(super_args.getLength() + 2);
				args.push("Targets\n�Ӽ��� �������ų� �Ҵ��ϰԵ� Module ���Դϴ�.");
				args.push("Module Codes\nModule Ž���� ����ϰԵ� Codes �Դϴ�.");
				args.push(super_args);
			}

			return _instance;
		}
	};
}