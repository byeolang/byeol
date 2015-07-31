#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ArrTagger : public NEModule
	{
	public:
		NETArgument<NEIntKey>	arg_method;
		NETArgument<NEKey>		arg_collector;
		NETArgument<NEIntKey>	arg_index;
		NETArgument<NEKey>		arg_unit;
		NETArgument<NEIntKey>	arg_size;
		NETArgument<NEIntKey>	arg_length;

		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;
			if (NEResult::hasError(_header.isValid()))
			{
				_header.getName() = "ArrTagger";
				_header.getDeveloper() = "kniz";
				_header.setRevision(2);
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2014-12-03";
				_header.getComment() =
					"NEByteSetKey, NEStringSetKey ��, Collector Key�� ���Ͽ�,\n"
					"Ư�� index�� ��ҿ� ���� get, insert, remove�� �����ϴ� Module �Դϴ�.\n"
					"������ ������ ������ �����ϴ�\n"
					"\t0:\tget. �־��� index�� ��ġ�� ��Ҹ� Unit Argument�� ����ϴ�.\n"
					"\t1:\tset. �־��� index�� UnitArgument�� �Ҵ��մϴ�.\n"
					"\t2:\tinsert. �־��� index�� Unit Argument�� ���� �߰��մϴ�.\n"
					"\t3:\tremove. �־��� index�� �ִ� ���Ҹ� �����մϴ�. ���� Index Argument�� disabled �� ���, Unit�� ��ġ�ϴ� ���Ҹ� �����մϴ�.\n"
					"\t4:\tfind. Unit�� ��ġ�ϴ� ���� Collector���� ã�� �� index�� �����մϴ�.��ã�� ��� -1�� ������ �˴ϴ�.\n"
					"����, ������ ���� �Ӽ����� ���� ������ �� �ֽ��ϴ�.\n"
					"\tSize: Container�� ũ�� �Դϴ�.\n"
					"\tLength: Length �Դϴ�.";
				NETStringSet& argcomments = _header.getArgumentsComments();
				argcomments.create(6);
				argcomments.push("Method\n������ �����Դϴ�.\n0: get[�⺻��], 1: set, 2: insert, 2: remove, 3: find");
				argcomments.push("Collector\n������ ������ Collector Key�Դϴ�.");
				argcomments.push("Index\n�ش� Collector�� Index �Դϴ�. insert���� -1�� push, remove���� -1�� pop�� �˴ϴ�.");
				argcomments.push("Unit\n���꿡 ���� ��� �� ���ڷ� ���˴ϴ�.\nget������ index�� ���Ұ� �Ҵ�Ǿ�����, insert������ �־��� index�� ���Ե˴ϴ�.");
				argcomments.push("Length\n�ش� Collector�� Length �Ӽ��� �����ɴϴ�.");
				argcomments.push("Size\n�ش� Collector�� Size �Ӽ��� �����ɴϴ�,");
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
			if( ! collector_key_e && ! unit_key_e) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

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
#define MAKE_COLLECTOR_BRANCH(TYPE, COLL)	\
	if(	arg_length.isEnable()					&&	\
	collector_key_e->isSubClassOf(NEType::##TYPE##))	\
	_fetchLength<##COLL##::Trait>(static_cast<COLL&>(*collector_key_e).getValue());	\
	if( arg_size.isEnable()						&&	\
	collector_key_e->isSubClassOf(NEType::##TYPE##))	\
	_fetchSize<##COLL##::Trait>(static_cast<COLL&>(*collector_key_e).getValue());
				MAKE_COLLECTOR_BRANCH(NEBOOLEAN_SET_KEY, NEBooleanSetKey);
				MAKE_COLLECTOR_BRANCH(NEBYTE_SET_KEY, NEByteSetKey);
				MAKE_COLLECTOR_BRANCH(NEUBYTE_SET_KEY, NEUByteSetKey);
				MAKE_COLLECTOR_BRANCH(NESHORT_SET_KEY, NEShortSetKey);
				MAKE_COLLECTOR_BRANCH(NEUSHORT_SET_KEY, NEUShortSetKey);
				MAKE_COLLECTOR_BRANCH(NEINT_SET_KEY, NEIntSetKey);
				MAKE_COLLECTOR_BRANCH(NEUINT_SET_KEY, NEUIntSetKey);
				MAKE_COLLECTOR_BRANCH(NEINT64_SET_KEY, NEInt64SetKey);
				MAKE_COLLECTOR_BRANCH(NEFLOAT_SET_KEY, NEFloatSetKey);
				MAKE_COLLECTOR_BRANCH(NEDOUBLE_SET_KEY, NEDoubleSetKey);
				MAKE_COLLECTOR_BRANCH(NESTRING_SET_KEY, NEStringSetKey);
				MAKE_COLLECTOR_BRANCH(NEWSTRING_SET_KEY, NEWStringSetKey);
				MAKE_COLLECTOR_BRANCH(NECODE_SET_KEY, NECodeSetKey);
				MAKE_COLLECTOR_BRANCH(NESTRING_KEY, NEStringKey);
				MAKE_COLLECTOR_BRANCH(NEWSTRING_KEY, NEWStringKey);
#undef MAKE_COLLECTOR_BRANCH

				while (unit_key_e)
				{
					_executeKey(*collector_key_e, *unit_key_e);

					if (unit_key)
						unit_key_e = &unit_key->getKey();
					else
						break;
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
			arg_method.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_index.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_length.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_length.setEnable(false);
			arg_size.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_size.setEnable(false);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_method);
			tray.push(arg_collector);
			tray.push(arg_index);
			tray.push(arg_unit);
			tray.push(arg_length);
			tray.push(arg_size);

			return RESULT_SUCCESS;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ArrTagger(*this));
		}

	private:
		void _executeKey(NEKey& collector, NEKey& unit)
		{
#define MAKE_COLLECTOR_BRANCH(TYPE, COLL, ELEM)	\
	if (collector.isSubClassOf(NEType::##TYPE##)) \
	return _operate<##COLL##::Trait, NEType::##TYPE##, ##ELEM##>(static_cast<COLL&>(collector).getValue(), arg_index.getValue(), unit)

			MAKE_COLLECTOR_BRANCH(NEBOOLEAN_SET_KEY,	NEBooleanSetKey,	NEBooleanKey);
			MAKE_COLLECTOR_BRANCH(NEBYTE_SET_KEY,		NEByteSetKey,		NEByteKey);
			MAKE_COLLECTOR_BRANCH(NEUBYTE_SET_KEY,		NEUByteSetKey,		NEUByteKey);
			MAKE_COLLECTOR_BRANCH(NESHORT_SET_KEY,		NEShortSetKey,		NEShortKey);
			MAKE_COLLECTOR_BRANCH(NEUSHORT_SET_KEY,		NEUShortSetKey,		NEUShortKey);
			MAKE_COLLECTOR_BRANCH(NEINT_SET_KEY,		NEIntSetKey,		NEIntKey);
			MAKE_COLLECTOR_BRANCH(NEUINT_SET_KEY,		NEUIntSetKey,		NEUIntKey);
			MAKE_COLLECTOR_BRANCH(NEINT64_SET_KEY,		NEInt64SetKey,		NEInt64Key);
			MAKE_COLLECTOR_BRANCH(NEFLOAT_SET_KEY,		NEFloatSetKey,		NEFloatKey);
			MAKE_COLLECTOR_BRANCH(NEDOUBLE_SET_KEY,		NEDoubleSetKey,		NEDoubleKey);
			MAKE_COLLECTOR_BRANCH(NESTRING_SET_KEY,		NEStringSetKey,		NEStringKey);
			MAKE_COLLECTOR_BRANCH(NEWSTRING_SET_KEY,	NEWStringSetKey,	NEWStringKey);
			MAKE_COLLECTOR_BRANCH(NECODE_SET_KEY,		NECodeSetKey,		NECodeKey);
			MAKE_COLLECTOR_BRANCH(NESTRING_KEY,			NEStringKey,		NECharKey);
			MAKE_COLLECTOR_BRANCH(NEWSTRING_KEY,		NEWStringKey,		NEWCharKey);
#undef MAKE_COLLECTOR_BRANCH
		}
		template <typename C, NEType::Type TYPE, typename UK>
		void _operate(C& c, type_index index, NEKey& unit)
		{
			switch (arg_method.getValue())
			{
			case 0:	//	GET
				if (index < 0 || index > c.getLengthLastIndex()) return;
				unit = UK(c[index]);
				break;

			case 1:	//	SET
				if (index < 0 || index > c.getLengthLastIndex()) return;
				c.setElement(index, UK(unit).getValue());
				break;

			case 2:	//	INSERT
				if (index < -1 || index > c.getLengthLastIndex()) return;
				if (index == -1) index = c.getLength();

				if (c.getLength() >= c.getSize())
					c.resize(c.getLength() + (c.getLength() * 0.5 + 1));

				c.insert(index, UK(unit).getValue());
				break;

			case 3:	//	REMOVE
				if (index < -1 || index > c.getLengthLastIndex()) return;
				if (index == -1) index = c.getLengthLastIndex();
				{
					type_index n = NE_INDEX_ERROR;
					if (!arg_index.isEnable())
						n = c.find(UK(unit).getValue());

					c.remove(n);
				}
				break;

			case 4:	//	FIND
				arg_index.setValue(c.find(UK(unit).getValue()));
				break;
			}

			return;
		}

		template <typename C>
		void _fetchLength(C& c)
		{
			if (arg_length.isEnable())
				arg_length.setValue(c.getLength());
			return;
		}
		template <typename C>
		void _fetchSize(C& c)
		{
			if (arg_size.isEnable())
				arg_size.setValue(c.getSize());
			return;
		}
	};
}
