#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ArrElementer : public NEModule
	{
	public:
		NETArgument<NEIntKey>	arg_method;
		NETArgument<NEKey>		arg_collector;
		NETArgument<NEIntKey>	arg_index;
		NETArgument<NEKey>		arg_unit;

		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;
			if (NEResult::hasError(_header.isValid()))
			{
				_header.getName() = "ArrElementer";
				_header.getDeveloper() = "kniz";
				_header.setRevision(2);
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2014-12-03";
				_header.getComment() =
					"NEByteSetKey, NEStringSetKey ��, Collector Key�� ���Ͽ�,\n"
					"Ư�� index�� ��ҿ� ���� get, insert, remove�� �����ϴ� Module �Դϴ�.\n"
					"������ ������ ������ �����ϴ�\n"
					"\t0:\tget. �־��� index�� ��ġ�� ��Ҹ� Unit Argument�� ����ϴ�.\n"
					"\t1:\tinsert. �־��� index�� Unit Argument�� ���� �߰��մϴ�.\n"
					"\t2:\tremove. �־��� index�� �ִ� ���Ҹ� �����մϴ�. ���� Index Argument�� disabled �� ���, Unit�� ��ġ�ϴ� ���Ҹ� �����մϴ�.\n"
					"\t3:\tfind. Unit�� ��ġ�ϴ� ���� Collector���� ã�� �� index�� �����մϴ�.��ã�� ��� -1�� ������ �˴ϴ�.\n";
				NETStringSet& argcomments = _header.getArgumentsComments();
				argcomments.create(4);
				argcomments.push("Method\n������ �����Դϴ�.\n0: get[�⺻��], 1: insert, 2: remove, 3: find");
				argcomments.push("Collector\n������ ������ Collector Key�Դϴ�.");
				argcomments.push("Index\n�ش� Collector�� Index �Դϴ�.");
				argcomments.push("Unit\n���꿡 ���� ��� �� ���ڷ� ���˴ϴ�.\nget������ index�� ���Ұ� �Ҵ�Ǿ�����, insert������ �־��� index�� ���Ե˴ϴ�.");
			}

			return _header;
		}

	protected:
		virtual type_result _onExecute()
		{
			NEKey			*collector_key_e = &arg_collector.getValueKey(),
				*unit_key_e = &arg_unit.getValueKey();
			NEKeySelector	*collector_key		= 0,
				*unit_key			= 0;
			if (!collector_key_e || !unit_key_e) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			if (arg_collector.getValueKey().isSubClassOf(NEType::NEKEY_SELECTOR))
			{
				collector_key = static_cast<NEKeySelector*>(&arg_collector.getValueKey());
				collector_key_e = &collector_key->getKey();
			}
			if (arg_unit.getValueKey().isSubClassOf(NEType::NEKEY_SELECTOR))
			{
				unit_key = static_cast<NEKeySelector*>(&arg_unit.getValueKey());
				unit_key_e = &unit_key->getKey();
			}


			//	main:
			while (collector_key_e)
			{
				while (unit_key_e)
				{
					_executeKey(*collector_key_e, *unit_key_e);

					if (unit_key)
						unit_key_e = &unit_key->getKey();
				}

				if (collector_key)
					collector_key_e = &collector_key->getKey();
			}

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentSet& tray)
		{
			tray.push(arg_method);
			tray.push(arg_collector);
			tray.push(arg_index);
			tray.push(arg_unit);
			return RESULT_SUCCESS;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ArrElementer(*this));
		}

	private:
		void _executeKey(NEKey& collector, NEKey& unit)
		{
			if (collector.isSubClassOf(NEType::NEBOOLEAN_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEBOOLEAN_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEBYTE_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEBYTE_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEUBYTE_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEUBYTE_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NESHORT_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NESHORT_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEUSHORT_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEUSHORT_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEINT_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEINT_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEUINT_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEUINT_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEINT64_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEINT64_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEFLOAT_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEFLOAT_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEDOUBLE_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEDOUBLE_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NESTRING_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NESTRING_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);
			else if (collector.isSubClassOf(NEType::NEWSTRING_SET_KEY))
				return _operate<NEBooleanSetKey, NEType::NEWSTRING_SET_KEY, NEBooleanKey>(collector, arg_index.getValue(), unit);			
		}
		template <typename CK, NEType::Type TYPE, typename UK>
		void _operate(NEKey& ck, type_index index, NEKey& unit)
		{
			if( ! ck.isSubClassOf(TYPE)) return;
			CK& casted = static_cast<CK&>(ck);
			if(index < 0 || index > casted.getValue().getLengthLastIndex()) return;

			switch(arg_method.getValue())
			{
			case 0:	//	GET
				unit = UK(casted.getValue()[index]);
				break;

			case 1:	//	INSERT				
				casted.getValue().insert(index, UK(unit).getValue());
				break;

			case 2:	//	REMOVE
				{
					type_index n = NE_INDEX_ERROR;
					if (!arg_index.isEnable())
						n = casted.getValue().find(UK(unit).getValue());

					casted.getValue().remove(n);
				}
				break;

			case 3:	//	FIND
				arg_index.setValue(casted.getValue().find(UK(unit).getValue()));
				break;
			}


			//	post:
			return;
		}
	};
}
