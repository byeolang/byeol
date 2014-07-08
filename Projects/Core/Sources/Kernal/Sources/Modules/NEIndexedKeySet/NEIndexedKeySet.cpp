#include "NEIndexedKeySet.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	type_id NEIndexedKeySet::_last_generated_id = 0;

	NEIndexedKeySet::NEIndexedKeySet(NEEnlistableManager& manager)
		: SuperClass(manager)
	{

	}

	NEIndexedKeySet::NEIndexedKeySet(type_count size)
		: SuperClass(size)
	{

	}

	NEIndexedKeySet::NEIndexedKeySet(const NEIndexedKeySet& source)
		: SuperClass(source)
	{

	}

	NEIndexedKeySet::~NEIndexedKeySet()
	{

	}

	const NEIndexedKeySet& NEIndexedKeySet::operator+=(const NEIndexedKeySet& source)
	{
		SuperClass::operator+=(source);

		return *this;
	}

	NEIndexedKeySet NEIndexedKeySet::operator+(const NEIndexedKeySet& source) const
	{
		ThisClass buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	NEType::Type NEIndexedKeySet::getType() const
	{
		return NEType::NEINDEXED_KEYSET;
	}

	NEObject& NEIndexedKeySet::clone() const
	{
		return static_cast<NEObject&>( *(new ThisClass(*this)) );
	}	

	type_id NEIndexedKeySet::peekLastGeneratedId() const
	{
		return _last_generated_id;
	}

	type_id NEIndexedKeySet::_generateId()
	{
		_last_generated_id++;
		if(_last_generated_id <= 0)
			KERNAL_ERROR(" : ");

		return _last_generated_id;
	}

	type_index NEIndexedKeySet::insert(type_index index, const NEKey* const source)
	{
		if( ! source)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}
		if(_occupiedset[index])
		{
			type_result result = setElement(index, source);
			if(NEResult::hasError(result))
			{
				KERNAL_ERROR(" : ");
				return result;
			}
		}
		
		
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_index inputed_index = SuperClass::insert(index, source);
		//		���̵� �Ҵ�:
		//			Ÿ����:
		NEKey& key = getElement(inputed_index);
		if(&key)
			key._id = _generateId();

		NEGlobalManagerOffer::_setGlobalManager(push);

		getElement(inputed_index)._id = _generateId();

		return inputed_index;
	}

	type_result NEIndexedKeySet::setElement(type_index index, const NEKey* const source)
	{
		if( ! source)
		{
			KERNAL_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}



		NEKey* const_casted = const_cast<NEKey*>(source);

		type_id original_id = const_casted->getId();
		const_casted->_id = _generateId();

		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_result result = SuperClass::setElement(index, source);

		NEGlobalManagerOffer::_setGlobalManager(push);

		const_casted->_id = original_id;

		return result;
	}

	NEBinaryFileSaver& NEIndexedKeySet::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);



		//	main:
		//		���� ������ ����:
		for(type_index n=0; n < getSize() ;n++)
		{	
			const NEKey& key = getElement(n);
			if( ! &key)
				continue;
			//		��� ����
			NEType::Type type = key.getType();
			//		��� ����:	Ÿ���ڵ��, load�� ��ν��Ͻ��� �����ؾ��ϴ°��� ���� ������ �����
			saver << type; 
			//		���� ������ ����:	NEModule::serialize()�� ȣ���Ѵ�.
			saver << key;
		}

		return saver << _last_generated_id;
	}

	NEBinaryFileLoader& NEIndexedKeySet::serialize(NEBinaryFileLoader& loader)
	{
		NEKeyManager& keyer = Kernal::getInstance().getKeyManager();		
		SuperClass::serialize(loader);



		//	main:
		//		���� ���� �ʱ�ȭ:	push�� ����ϱ� ���ؼ�
		_length = 0;
		//		Occupied Ž�� �ε��� �ʱ�ȭ:
		//			occupiedSet���� index�κ��� ������� �ø��������� �˻��Ѵ�.
		//			���� ����� ���߿� OccupiedSet[index] == true�� ���� ����� �߰��ؾ��ϴ� 
		//			�κ��� �ȴ�.
		type_index occupied_index = -1;
		for(int n=0; n < getSize() ;n++)
		{
			if( ! _occupiedset[n])
				continue;

			NEType::Type type = NEType::UNDEFINED;
			loader >> type;

			const NEKey& source_key = keyer.getKey(type);
			if( ! &source_key)
			{
				KERNAL_ERROR(" : ")
				continue;
			}

			//	Ű ����:
			//		��� ������ ���ؼ� �������� ��Ż:
			_occupiedset[n] = false;
			//		����:
			/*
				���ο��� NodeManager::KeySet�� �ν��Ͻ��� �����Ͽ� 
				�߰��ϰ�, �ε����� _getElement�Ѵ�.
				Length�� �̹� 0�̹Ƿ� �Ƚ��ϰ� push ����.
			*/
			type_index index = insert(n, source_key);
			NEKey& unit = getElement(index);
			if( ! &unit)
			{
				KERNAL_ERROR(" : ");
				continue;
			}

			loader >> unit;
		}



		return loader >> _last_generated_id;
	}

	type_result NEIndexedKeySet::_onEnlisted()
	{
		return RESULT_SUCCESS;
	}

	type_result NEIndexedKeySet::resize(type_index new_size)
	{
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_result result = SuperClass::resize(new_size);

		NEGlobalManagerOffer::_setGlobalManager(push);
		return result;
	}
}