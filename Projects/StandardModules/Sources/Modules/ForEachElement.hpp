#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ForEachElement : public NEModule
	{
	public:
		typedef ForEachElement ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKey>				arg_collector;
		NETArgument<NEKey>				arg_element;
		NETArgument<NEIntKey>			arg_count;
		NETArgument<NEModuleCodeSetKey>	arg_actions;

	protected:
		virtual type_result _onExecute()
		{
			NEKey			*collector_key_e = &arg_collector.getValueKey();
			NEKeySelector	*collector_key = 0;
			if( ! collector_key_e) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			if(arg_collector.getValueKey().isSubClassOf(NEType::NEKEY_SELECTOR))
				collector_key = static_cast<NEKeySelector*>(&arg_collector.getValueKey());


			//	main:
			arg_count.setValue(0);

			while(collector_key_e)
			{
				while(_detachElementInto(*collector_key_e, arg_count.getValue()))
				{
					arg_actions.getValue().execute();

					arg_count++;
				}					

				if (collector_key)
					collector_key_e = &collector_key->getKey();
				else
					break;
			}

			return RESULT_SUCCESS;			
		}

		virtual type_result _onFetchModule()
		{
			arg_element.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_count.setPurposeLimitation(NEArgumentBase::WRITTEN);

			return RESULT_SUCCESS;
		}

		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_collector);
			tray.push(arg_element);
			tray.push(arg_count);
			tray.push(arg_actions);

			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "ForEachElement";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2015/01/07";
				_instance.getComment() =
					"Collector�� Element ���� Actions ������ ���ϴ�.\n"
					"Collector�� �ִ� Element�� ������ ElementArgument�� �ְ� Actions�� �����ŵ�ϴ�."
					"�̶�, Count�� 0���� 1�� �����մϴ�.\n"
					"Collector���� NEString, NEStringSet, NEIntSet ���� ����, "
					"ElementArgument���� ������ Ÿ���� Key�� ������ �˴ϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(4);
				args.push("Collector\nActions ������ �� Collector �Դϴ�.");
				args.push("");
			}

			return _instance;
		}

		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		//	������ �ε����� �ٴ����� false�� ��ȯ�Ѵ�.
		bool _detachElementInto(NEKey& collector, type_index n)
		{
#define MAKE_COLLECTOR_BRANCH(TYPE, COLL, ELEM)	\
	if (collector.isSubClassOf(NEType::##TYPE##)) \
	return _operate<##COLL##::Trait, NEType::##TYPE##, ##ELEM##>(static_cast<COLL&>(collector).getValue(), n)

			MAKE_COLLECTOR_BRANCH(NEBOOLEAN_SET_KEY, NEBooleanSetKey, NEBooleanKey);
			MAKE_COLLECTOR_BRANCH(NEBYTE_SET_KEY, NEByteSetKey, NEByteKey);
			MAKE_COLLECTOR_BRANCH(NEUBYTE_SET_KEY, NEUByteSetKey, NEUByteKey);
			MAKE_COLLECTOR_BRANCH(NESHORT_SET_KEY, NEShortSetKey, NEShortKey);
			MAKE_COLLECTOR_BRANCH(NEUSHORT_SET_KEY, NEUShortSetKey, NEUShortKey);
			MAKE_COLLECTOR_BRANCH(NEINT_SET_KEY, NEIntSetKey, NEIntKey);
			MAKE_COLLECTOR_BRANCH(NEUINT_SET_KEY, NEUIntSetKey, NEUIntKey);
			MAKE_COLLECTOR_BRANCH(NEINT64_SET_KEY, NEInt64SetKey, NEInt64Key);
			MAKE_COLLECTOR_BRANCH(NEFLOAT_SET_KEY, NEFloatSetKey, NEFloatKey);
			MAKE_COLLECTOR_BRANCH(NEDOUBLE_SET_KEY, NEDoubleSetKey, NEDoubleKey);
			MAKE_COLLECTOR_BRANCH(NESTRING_SET_KEY, NEStringSetKey, NEStringKey);
			MAKE_COLLECTOR_BRANCH(NEWSTRING_SET_KEY, NEWStringSetKey, NEWStringKey);
			MAKE_COLLECTOR_BRANCH(NECODE_SET_KEY, NECodeSetKey, NECodeKey);
			MAKE_COLLECTOR_BRANCH(NESTRING_KEY, NEStringKey, NECharKey);
			MAKE_COLLECTOR_BRANCH(NEWSTRING_KEY, NEWStringKey, NEWCharKey);
#undef MAKE_COLLECTOR_BRANCH
		}

		template <typename C, NEType::Type TYPE, typename UK>
		bool _operate(C& c, type_index index)
		{
			if (index < 0 || index > c.getLengthLastIndex()) return false;

			arg_element.getValueKey() = UK(c[index]);

			return true;
		}

	};
}