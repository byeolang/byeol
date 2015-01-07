#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ForEach : public NEModule
	{
	public:
		typedef ForEach ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKey>				arg_selector;
		NETArgument<NEModuleCodeSetKey>	arg_actions;		

	protected:
		virtual type_result _onFetchModule()
		{
			arg_actions.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			NEKey& sel_key = arg_selector.getValueKey();
			if( ! sel_key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_WARNING(" : �־��� arg_selector�� SELECTOR Ű �� �ƴմϴ�.\n\t�־��� Ű�� Ÿ�� : %s", sel_key.getTypeName());
				return RESULT_TYPE_WARNING | RESULT_INVALID_DATA;
			}	


			//	main:
			NENodeSelector& minimum_casted = static_cast<NENodeSelector&>(sel_key);
			//		�ʱ���� ���:	�۾��� ������ ��������� �Ѵ�.
			bool origins_peeking_lock = minimum_casted.isPeekingLocked();
			//		���� ����:
			while(_moveForward(minimum_casted))
				arg_actions.getValue().execute();


			//	post:
			minimum_casted.isPeekingLocked() = origins_peeking_lock;
			return RESULT_SUCCESS;
		}

	private:
		bool _moveForward(NENodeSelector& target)
		{
			target.isPeekingLocked() = true;

			bool is_end_of_searching = false;
			if (target.isSubClassOf(NEType::NEKEY_SELECTOR))
				is_end_of_searching = ! &(static_cast<NEKeySelector&>(target)).getKey();
			else if (target.isSubClassOf(NEType::NEMODULE_SELECTOR))
				is_end_of_searching = ! &(static_cast<NEModuleSelector&>(target)).getModule();
			else
				is_end_of_searching = ! &target.getNode();

			target.isPeekingLocked() = false;

			return is_end_of_searching;
		}

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_selector);
			tray.push(arg_actions);

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "ForEach";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/07";
				_instance.getComment() =
					"Selector Argument�� ���� NodeSelector, ModuleSelector, KeySelector�� �ϳ��� "
					"�����ϴ� ��ü�鸶�� Actions Argument�� �����ŵ�ϴ�."
					"������ ������ �����ϴ�.\n"
					"\t1. ���ϴ� Selector�� ���� Key�� �ϳ� ����ϴ�.\n"
					"\t2. �� Ű�� SelectorArgument�� �ֽ��ϴ�.\n"
					"\t3. Actions�� ����� �����, �ռ� ���� SelectorKey�� Argument�� �־��ݴϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(2);
				args.push("Selector\n��ȯ(����)�ϰ� �� ��ü�� ����Ű�� Selector �Դϴ�.\nNodeSelector, ModuleSelector, KeySelector ���� ȣȯ�˴ϴ�.");
				args.push("Actions\nSelector�κ��� ���� ��ü �ϳ����� ���⿡ �ִ� ��� ������ �����մϴ�.");
			}

			return _instance;
		}

		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}