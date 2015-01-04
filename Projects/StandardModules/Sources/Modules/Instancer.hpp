#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL Instancer : public NEModule
	{
	public:
		typedef Instancer ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NENodeSelector> arg_targets;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_targets.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);
			arg_targets.setPurpose(NEArgumentBase::WRITTEN);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_targets);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			NENodeSelector& ns = arg_targets.getValue();

			while(NENode* node = &ns.getNode())
			{
				NENodeCodeSet& ncs = node->getOwner();
				if (!&ncs)
				{
					ALERT_WARNING(" : ");
					continue;
				}

				for(int n = 0; n < ncs.getLength(); n++)
					if (&ncs[n] == node)
					{
						type_result result = ncs.remove(n);
						if (NEResult::hasError(result))
							ALERT_ERROR(" : ");

						break;
					}
			}

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Instancer";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2012/04/14";
				_instance.getComment() =
					"Manager�� Script ��ü�� �߰��ϰų�, Node�� ������ �� �ֽ��ϴ�.\n"
					"Target Argument�� Purpose�� READ�� �ϸ� ������, WRITE�� �ϸ� �߰��� �˴ϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(1);
				args.push("Target\nNodeManager�� �߰��ϰų�, ������ Node�� ����ŵ�ϴ�.\nPurpose�� READ�̸� ������, WRITE�̸� �߰��� �ǹ��մϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}