#pragma once

#include "NodeSelectorTaggerBase.hpp"

namespace NE
{
	class NE_DLL NodeSelectorTagger : public NodeSelectorTaggerBase
	{
	public:
		typedef NodeSelectorTagger ThisClass;
		typedef NodeSelectorTaggerBase SuperClass;

	public:
		NETArgument<NENodeSelector>	arg_targets;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);

			return SuperClass::_onFetchArguments(tray);
		}		
		virtual type_result _onExecute()
		{
			return _passPipes(arg_targets.getValue());
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
				NEExportable::ModuleHeader super = SuperClass::getHeader();
				_instance.getName() = "NodeSelectorTagger";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/02";
				_instance.getComment() =
					"NodeSelector�� �Ӽ����� �������ų�, ������ �� �ֽ��ϴ�."
					"���� �� �� �ִ� �Ӽ��� ������ �����ϴ�.\n" + super.getComment();

				NETStringSet& args = _instance.getArgumentsComments();
				const NETStringSet& super_args = super.getArgumentsComments();
				args.create(super_args.getLength() + 1);
				args.push("Target\n�Ӽ��� ������ ����Դϴ�.");
				args.push(super_args);
			}

			return _instance;
		}
	};
}