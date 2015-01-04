#pragma once

#include "NamedUnitTagger.hpp"

namespace NE
{
	class NE_DLL ModuleTagger : public NamedUnitTagger
	{
	public:
		typedef ModuleTagger ThisClass;
		typedef NamedUnitTagger SuperClass;

	public:
		NETArgument<NEModuleSelector>	arg_targets;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);

			return SuperClass::_onFetchArguments(tray);
		}

	protected:
		using SuperClass::_onExecute;
		virtual type_result _onExecute()
		{
			type_result result = RESULT_SUCCESS;
			NEModuleSelector& ns = arg_targets.getValue();
			while(NEModule* n = &ns.getModule())
				result |= _onExecute(*n);

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

				_instance.getName() = "ModuleTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"������ Module ��ü���� Ư�� �Ӽ������� �������ų�(get) �Ҵ��� �� �ֽ��ϴ�(set).\n"
					"\tTargets:\t������ ��ü��\n" +
					supers.getComment();
				//"�־��� ��ü���� ������ ���� �Ӽ��鿡 ���ؼ� ���� �������ų� �Ҵ� �� �� �ֽ��ϴ�.\n"
				//"\tEnable:\t��ü�� Ȱ��ȭ ����.\n"
				//"\tCodes:\t��ü�� �־��� Code����";					
				NETStringSet& args = _instance.getArgumentsComments();
				const NETStringSet& super = supers.getArgumentsComments();
				args.create(super.getLength() + 1);
				args.push("Targets\n�Ӽ��� �������ų� �Ҵ��ϰԵ� Module ���Դϴ�.");
				args.push(supers);
			}

			return _instance;
		}
	};
}