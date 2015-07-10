#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL Binder : public NEModule
	{
	public:
		typedef Binder ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKey> arg_sources;
		NETArgument<NEKey> arg_targets;
		NETArgument<NEModuleCodeSetKey> arg_filters;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_sources.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_targets.setPurposeLimitation(NEArgumentBase::WRITTEN);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_sources);
			tray.push(arg_targets);
			tray.push(arg_filters);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			NEKey& target = arg_targets.getValueKey();
			NEKey& source = arg_sources.getValueKey();
			if( ! target.isSubClassOf(NEType::NENODE_SELECTOR)	||
				! source.isSubClassOf(NEType::NENODE_SELECTOR)	)
				return ALERT_ERROR("�־��� Target Ȥ�� Source�� NodeSelector�� ����Ŭ������ �ƴմϴ�.");
			if (!target.isSubClassOf(source))
				return ALERT_ERROR("Target�� Source���� ����Ŭ���� Ȥ�� ���� Ŭ������ �ƴմϴ�.");
			NENodeSelector& node_sel = static_cast<NENodeSelector&>(source);
			NEModuleCodeSet& filters = arg_filters.getValue();

			//	main:
			//		���� ����:
			while(_moveForward(node_sel))
			{
				for(int n = 0; n < filters.getLength(); n++)
				{
					if (NEResult::hasFalse(filters[n].execute()))
						goto NextObject;
				}

				//	Found:
				target = source;
				break;

NextObject:;
			}			

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Binder";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/07/06";
				_instance.getComment() = "Source Selector�� ����Ű�� ��ü �߿� Filter�� ���(��� true)�� ��ü�� �ִٸ�, Target Selector�� �׶��� ��ü�� ���ε���ŵ�ϴ�.\n��ü�� ã���� Target Selector = Source Selector�� �̷�����Ƿ�, Target Selector�� Source Selector�� ���ų� �� ��ü���� Ŭ����Ÿ���̾�� �մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(3);
				args.push("Source Selector\n���ε� ��ų ������ ����Ű�� �������Դϴ�.");
				args.push("Target Selector\nSource Selector�� ����Ű�� ��ü �߿���, Filter�� ��� ����� ��ü�� ���ε��˴ϴ�.\n���� Target Selector�� NodeSelectorKey�� ���� Node��ü�� ���ε��˴ϴ�.");
				args.push("Filter\n������ �ִ� ��� Module�� TRUE�� ��ȯ�ؾ� Filter�� ����� ������ �����մϴ�.");				
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		bool _moveForward(NENodeSelector& target)
		{
			target.unbind();

			bool is_there_more = false;
			if (target.isSubClassOf(NEType::NEKEY_SELECTOR))
				is_there_more = &(static_cast<NEKeySelector&>(target)).getKey() != 0x00;
			else if (target.isSubClassOf(NEType::NEMODULE_SELECTOR))
				is_there_more = &(static_cast<NEModuleSelector&>(target)).getModule() != 0x00;
			else
				is_there_more = &target.getNode() != 0x00;

			target.bind();

			return is_there_more;
		}
	};
}