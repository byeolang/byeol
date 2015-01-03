#pragma once

#include "NamedUnitTagger.hpp"

namespace NE
{
	class NE_DLL NodeTagger : public NamedUnitTagger
	{
	public:
		typedef NodeTagger ThisClass;
		typedef NamedUnitTagger SuperClass;

	public:
		NETArgument<NENodeSelector>	arg_targets;

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
			NENodeSelector& ns = arg_targets.getValue();
			while(NENode* n = &ns.getNode())
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

			if( ! is_triggered_once)
			{
				is_triggered_once = true;
				NEExportable::ModuleHeader supers = SuperClass::getHeader();

				_instance.getName() = "NodeTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"������ Node ��ü���� Ư�� �Ӽ������� �������ų�(get) �Ҵ��� �� �ֽ��ϴ�(set).\n"
					"\tTargets:\t������ ��ü��\n" +
					supers.getComment();
				//"�־��� ��ü���� ������ ���� �Ӽ��鿡 ���ؼ� ���� �������ų� �Ҵ� �� �� �ֽ��ϴ�.\n"
				//"\tEnable:\t��ü�� Ȱ��ȭ ����.\n"
				//"\tCodes:\t��ü�� �־��� Code����";					
				NETStringSet args = supers.getArgumentsComments();
				args.resize(args.getLength() + 1);
				args.pushFront("Targets\n�Ӽ��� �������ų� �Ҵ��ϰԵ� Node ���Դϴ�.");
			}

			return _instance;
		}
	};
}