#pragma once

#include "../AgingTemplate/AgingTemplate.hpp"

namespace NE
{
	template <typename T, NEType::Type TYPE>
	class NE_DLL ManagedAgingTemplate : public AgingTemplate<T, TYPE>
	{
	public:
		typedef ManagedAgingTemplate<T, TYPE> ThisClass;
		typedef AgingTemplate SuperClass;

	public:
		T& getMinimum()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<T&>(casted->getMinimum());
		}
		const T& getMinimum() const
		{
			const NEKey& somekey = getKeySet()[3];
			if( ! somekey.isSubClassOf(TYPE))
			{
				ALERT_ERROR("3�� Ÿ���� %s���� �մϴ�.", NEType::getTypeName(TYPE));
				const T* nullpoint = 0;
				return *nullpoint;
			}

			return (static_cast<const KeyClass&>(somekey)).getValue();			
		}
		T& getMaximum()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<T&>(casted->getMaximum());
		}
		const T& getMaximum() const
		{
			const NEKey& somekey = getKeySet()[4];
			if( ! somekey.isSubClassOf(TYPE))
			{
				ALERT_ERROR("3�� Ÿ���� %s���� �մϴ�.", NEType::getTypeName(TYPE));
				const T* nullpoint = 0;
				return *nullpoint;
			}

			return (static_cast<const KeyClass&>(somekey)).getValue();			
		}

	public:
		virtual type_result initialize()
		{
			SuperClass::initialize();
			NEKeyCodeSet& keyset = getKeySet();
			keyset.resize(keyset.getLength() + 2);
			keyset.push(KeyClass());
			keyset.push(KeyClass());

			return RESULT_SUCCESS;
		}
		virtual type_result execute()
		{
			SuperClass::execute();			
			NEKeySelector& sel = getKeySelector();
			KeyClass	min = getMinimum(),
						max = getMaximum();
			

			//	main:
			while(NEKey* key = &sel.getKey())
			{
				if(*key < min)
					*key = min;
				if(*key > max)
					*key = max;
			}
			

			return RESULT_SUCCESS;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "ManagedAgingTemplate";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013/10/09";
				_instance.getComment() = "0��: Ÿ��Key \n1��: +�� �� \n2��: �����Լ�\n3��. ���� �ּҰ�\n4��. ������ �ִ밪";
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}