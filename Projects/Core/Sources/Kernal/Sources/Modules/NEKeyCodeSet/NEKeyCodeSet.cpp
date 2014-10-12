#include "NEKeyCodeSet.hpp"

#include "../Kernal/Kernal.hpp"
#include "../NEIndexedKeySet/NEIndexedKeySet.hpp"
#include "../NEModuleCodeSetKey/NEModuleCodeSetKey.hpp"
#include "../NENodeCodeSetKey/NENodeCodeSetKey.hpp"

namespace NE
{
	NEKeyCodeSet::NEKeyCodeSet()
		: SuperClass()
	{
		
	}
	NEKeyCodeSet::NEKeyCodeSet(type_index size)
		: SuperClass(size)
	{
		
	}
	NEKeyCodeSet::NEKeyCodeSet(const NEKeyCodeSet& source)
		: SuperClass(source)
	{
		_assign(source);	//	����, NENodeCodeSet::insert�� ���������� ȣ��ȴ�.
	}

	NEKeyCodeSet::~NEKeyCodeSet()
	{
		release();	//	���⼭ ���� ���� ������ ����Ŭ���� �Ҹ��ڿ��� ����?Ŭ���� release�� ȣ���ع��� ���̴�.
	}

	const NEKeyCodeSet& NEKeyCodeSet::operator+=(const NEKeyCodeSet& source)
	{
		if(this == &source)
		{
			NEKeyCodeSet copyied(*this);
			resize(_length + copyied._length);
			push(copyied);
		}
		else 
		{
			resize(_length + source._length);
			push(source);
		}

		return *this;
	}

	NEKeyCodeSet NEKeyCodeSet::operator+(const NEKeyCodeSet& source) const
	{
		ThisClass buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	type_index NEKeyCodeSet::insert(type_index index, const NEKey& source)
	{
		//	pre:
		NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ")
			return NE_INDEX_ERROR;
		}



		//	main:
		type_index keycode = keyset.pushFront(source);
		if(keycode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ")
			return NE_INDEX_ERROR;
		}

		if( NEResult::hasError(_insert(index, keycode)) )
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}
		//	Owner ����:
		keyset[keycode]._owner = this;

		if(isEnlisted())
		{
			NEKey& key = getElement(index);
			if( NEResult::hasError(_onEnlisted(key)) )
			{
				KERNAL_ERROR(" : ");
				return NE_INDEX_ERROR;
			}
		}		

		return index;
	}
	type_index NEKeyCodeSet::remove(type_index index)
	{
		//	pre:
		NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ")
			return NE_INDEX_ERROR;
		}



		//	main:
		type_index keycode = keyset.remove(_getElement(index));
		if(keycode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ")
			return NE_INDEX_ERROR;
		}

		return _remove(index);
	}

	NEKey& NEKeyCodeSet::getElement(type_index index)
	{
		//	pre:
		NEKey* nullpointer = 0x00;
		NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ")
			return *nullpointer;
		}



		//	main:
		type_index& keycode = _getElement(index);
		if( ! &keycode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return *nullpointer;
		}
		
		return keyset[keycode];
	}
	const NEKey& NEKeyCodeSet::getElement(type_index index) const
	{
		//	pre:
		NEKey* nullpointer = 0x00;
		const NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ")
			return *nullpointer;
		}



		//	main:
		const type_code& keycode = _getElement(index);
		if( ! &keycode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return *nullpointer;
		}

		return keyset[keycode];
	}
	type_result NEKeyCodeSet::setElement(type_index index, const NEKey& source)
	{
		//	pre:
		NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ")
			return RESULT_TYPE_ERROR;
		}



		//	main:
		type_index& keycode = _getElement(index);
		if( ! &keycode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return RESULT_TYPE_ERROR;
		}

		return keyset.setElement(keycode, source);
	}
	type_index NEKeyCodeSet::find(const NEKey& source) const
	{
		//	pre:
		const NEIndexedKeySet& keyset = _getKeySet();
		if(! &keyset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		for(int n=0; n < getLength() ;n++)
		{
			type_index keycode = _getElement(n);
			const NEKey& unit = keyset[keycode];
			if( ! &unit)
			{
				KERNAL_ERROR(" : ");
				return NE_INDEX_ERROR;
			}

			if(unit == source)
				return n;
		}

		return NE_INDEX_ERROR;
	}
	NEObject& NEKeyCodeSet::clone() const
	{
		return *(new ThisClass(*this));
	}
	NEType::Type NEKeyCodeSet::getType() const
	{
		return NEType::NEKEY_CODESET;
	}
	NEIndexedKeySet& NEKeyCodeSet::_getKeySet()
	{
		NEIndexedKeySet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
			return *nullpointer;
		}
		return getManager()._getKeySet();
	}
	const NEIndexedKeySet& NEKeyCodeSet::_getKeySet() const
	{
		NEIndexedKeySet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
			return *nullpointer;
		}
		return getManager().getKeySet();
	}

	type_result NEKeyCodeSet::_onEnlisted(NEKey& key)
	{
		if( ! &key)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}
		NEType::Type type = key.getType();

		switch(type)
		{
		case NEType::NENODE_CODESET_KEY:
			{
				NENodeCodeSetKey& casted = static_cast<NENodeCodeSetKey&>(key);
				NENodeCodeSet& node_codeset = casted.getValue();

				return node_codeset._onEnlisted();
			}
			break;

		default:
			return RESULT_SUCCESS;
		}
	}

	type_result NEKeyCodeSet::_onEnlisted()
	{
		_setEnlisted(true);

		type_result result = RESULT_SUCCESS;
		for(int n=0; n < getLength() ;n++)
		{
			NEKey& key = getElement(n);
			if( ! &key)
				continue;

			result = result | _onEnlisted(key);
		}

		return result;
	}

	const NEKeyCodeSet& NEKeyCodeSet::_assign(const NEKeyCodeSet& source)
	{
		if(this == &source) return *this;

		create(source.getSize());
		push(source);

		return *this;
	}

	type_result NEKeyCodeSet::resize(type_index new_size)
	{
		/*
			�� resize�� ������ �ϴ°�:
				NEArrayTemplate<T, bool, Q, Type>���� ������ resize 
				�ٸ� ������ �����ϱ� �����̴�. �ش� �Լ������� clone
				�Լ��� ����ؼ� ��Ƶ� �ν��Ͻ��� �ӽ� ���ۿ� ��Ƶδ�
				������ �����ϰ� �ִµ�, ���������� �� �ڵ�� NEIndexedNodeSet
				���� ������ �Ұ����� �ڵ��̱� �����̴�.
				
			�� clone�� ����� �� ���°�:
				������ ������ NEEnlistableSetTemplate.hpp�� ��������� �ּ��� 
				�����϶�.

			�˰���:	ShallowCopy�� ȿ���� ���δ�.
				�� ����� �ν��Ͻ��� shallow copy�����ν� ȿ���� ���̴� �˰����̴�.
				InnerType�� OuterType���� ������ ��쿡�� ����� ������ ����̸�,
				���� ������ ũ�⸦ ��ҽ�Ű�� resize���, �� ���̸�ŭ �ν��Ͻ��� �����ؾ��Ѵ�.

				���� size�� 5 -> 10���� ����Ǵ� ���, ���� �˰�����
					buffer.create()
					buffer.push(src);	<---	5 * (type_code + NEIndexedNodeSet���� NENode) ����
					src.resize(10);
					src.push(buffer)	<---	5 * (type_code + NEIndexedNodeSet���� NENode) ����
										+)		10 * (type_code + NENode) ��� �Ҹ�.
				�ݸ�, �� �˰�����,
					type_code buffer[5];
					buffer.push(src);	<---	5 * type_code ����.
					delete src._data;	//	delete[]�� �ƴԿ� ����.
					src = new type_code[10];
					src.push(buffer);	<---	5 * type_code ����.
										+)		10 * type_code ���� ��� �Ҹ�.
				�̹Ƿ�, ��ü������ ū ȿ���� ���� �� �ִ�.
		*/
		//	pre:		
		int instance_count = getLength() - new_size;	//	�����ϰԵ� �ν��Ͻ� ����
		if(instance_count > 0)
			for(int n=0; n < instance_count ;n++)
				pop();	//	�翬�� �����Ǵ� �� �׻� �ڿ������ʹ�. 


		//	main:
		//		���� ���縦 �����Ѵ�:	���۸� �����. ArrayTemplate�� ����� �� ����.
		int length = getLength();
		InnerType* buffer = new InnerType[_length];
		for(int n=0; n < length ;n++)
			buffer[n] = _data[n];
		//		resize:
		_length = 0;	//	_length = 0�� �����Ƿ� InnerElement�� delete�ϴ� release()�� ȣ����� �ʰ� �ȴ�.		
		create(new_size);	//	���ο��� release()�� ȣ���Ѵ�.


		//		post:
		for(int n=0; n < length ;n++)
			_data[n] = buffer[n];
		_length = length;
		delete [] buffer;
		return RESULT_SUCCESS;
	}

	void NEKeyCodeSet::release()
	{
		while(getLength())
			pop();

		SuperClass::release();
	}
}