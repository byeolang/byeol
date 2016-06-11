#include "NENodeCodeSet.hpp"
#include "../Kernal/Kernal.hpp"
#include "../NEIndexedNodeSet/NEIndexedNodeSet.hpp"

namespace NE
{
	NENodeCodeSet::NENodeCodeSet()
		: SuperClass()
	{
		
	}

	NENodeCodeSet::NENodeCodeSet(type_index size)
		: SuperClass(size)
	{
		
	}
	NENodeCodeSet::NENodeCodeSet(const NENodeCodeSet& source)
		: SuperClass(source)
	{
		_assign(source);	//	����, NENodeCodeSet::insert�� ���������� ȣ��ȴ�.
	}

	NENodeCodeSet::~NENodeCodeSet()
	{
		release();	//	���⼭ ���� ���� ������ ����Ŭ���� �Ҹ��ڿ��� ����Ŭ���� release�� ȣ���ع��� ���̴�.
	}

	const NENodeCodeSet& NENodeCodeSet::operator+=(const NENodeCodeSet& source)
	{
		if(this == &source)
		{
			NENodeCodeSet copyied(*this);
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

	NENodeCodeSet NENodeCodeSet::operator+(const NENodeCodeSet& source) const
	{
		ThisClass buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	type_index NENodeCodeSet::insert(type_index index, const NENode& source)
	{
		//	pre:
		
		//	TEST: serialize�� ������ insert�����ε� �ùٸ��� _enlist�� ������ ���̴�.

		NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		type_index nodecode = nodeset.pushFront(source);
		if(nodecode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : �ν��Ͻ��� ����� ���� �ʾҽ��ϴ�.\n\t�ڵ�: %d", nodecode);
			return NE_INDEX_ERROR;
		}
		if( NEResult::hasError(_insert(index, nodecode)) )
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}
		//		Owner ����:
		nodeset[nodecode]._owner = this;

		if(isEnlisted())
		{
			NENode& node = getElement(index);			
			if( NEResult::hasError(_onEnlisted(node)) )
			{
				KERNAL_ERROR(" : ");
				return NE_INDEX_ERROR;
			}
		}

		return index;
	}
	type_index NENodeCodeSet::remove(type_index index)
	{
		//	pre:
		if(	index < 0					|| 
			index > getLengthLastIndex())
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ �ѽ��ϴ�.");
			return NE_INDEX_ERROR;
		}
		NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		type_index nodecode = nodeset.remove(_getElement(index));
		if(nodecode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}

		return _remove(index);
	}

	NENode& NENodeCodeSet::getElement(type_index index)
	{
		//	pre:
		NENode* nullpointer = 0x00;
		NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return *nullpointer;
		}



		//	main:
		type_index& nodecode = _getElement(index);
		if( ! &nodecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return *nullpointer;
		}



		//	post:
		return nodeset[nodecode];
	}
	const NENode& NENodeCodeSet::getElement(type_index index) const
	{
		//	pre:
		NENode* nullpointer = 0x00;
		const NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return *nullpointer;
		}



		//	main:
		const type_index& nodecode = _getElement(index);
		if( ! &nodecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");
			
			return *nullpointer;
		}



		//	post:
		return nodeset[nodecode];
	}
	type_result NENodeCodeSet::setElement(type_index index, const NENode& source)
	{
		//	pre:
		NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}



		//	main:
		type_index& nodecode = _getElement(index);
		if( ! &nodecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return RESULT_TYPE_ERROR;
		}
		return nodeset.setElement(nodecode, source);
	}
	type_index NENodeCodeSet::find(const NENode& source) const
	{
		//	pre:
		const NEIndexedNodeSet& nodeset = _getNodeSet();
		if(! &nodeset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		for(int n=0; n < getLength() ;n++)
		{
			type_index nodecode = _getElement(n);
			const NENode& unit = nodeset[nodecode];
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
	NEObject& NENodeCodeSet::clone() const
	{
		return *(new ThisClass(*this));
	}
	NEType::Type NENodeCodeSet::getType() const
	{
		return NEType::NENODE_CODESET;
	}
	NEIndexedNodeSet& NENodeCodeSet::_getNodeSet()
	{
		NEIndexedNodeSet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
				return *nullpointer;
		}
		return getManager()._getNodeSet();
	}
	const NEIndexedNodeSet& NENodeCodeSet::_getNodeSet() const
	{
		NEIndexedNodeSet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
				return *nullpointer;
		}
		return getManager().getNodeSet();
	}



	type_result NENodeCodeSet::execute()
	{
		//	pre:
		//		Ÿ����:
		type_result result = RESULT_SUCCESS;
		NEEnlistableManager& manager = getManager();
		if( ! &manager || manager.getType() != NEType::NENODE_MANAGER)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}
		NENodeManager& noder = static_cast<NENodeManager&>(manager);
		NENodeManager::LocalStack& localstack = noder.getLocalStack();
		//		���� ���� ����:
		//			���ο� ���ý����� �ֱ����� ���� �� ����:
		//				�� �������� �����ϴ°�:
		//					���� �ȿ� ������ �ִ� �������, ���� ������ ó���ϰ� �ٽ�
		//					�ܺ� �������� ���ƿ����� ���� ���°� ���ý��ÿ� ����� ���
		//					���� ��߳� �� �ֱ� �����̴�.
		NENodeCodeSet& before_stage_nodeset = localstack.getRecentNodeSet();
		localstack._setRecentNodeSet(*this);
		localstack._setRecentResultCode(RESULT_NOTHING);



		//	main:
		//		����:
		//			���� ���� ����:
		for(int n=0; n < getLength() ;n++)
		{
			NENode& node = getElement(n);
			if( ! node.isEnable()) continue;
			localstack._setRecentNodeSetIndex(n);
			localstack._setRecentNode(node);
			localstack._setRecentNodeRealIndex(_getElement(n));
			type_result result = node.getModuleSet().execute();
			localstack._setRecentResultCode(result);				
#ifdef DEVELOPER			
			//			����� ���:
			if(NEResult::hasError(result))
			{
				type_result result_value = localstack.getRecentResultCode();
				KERNAL_ERROR("E100031C4 : ���¿��� ���� �߻�\n���¿��� ������ ��尡 ������ ��ȯ�߽��ϴ�\n������ ī��Ʈ : %d\t����ε��� : %d\n����ڵ� : %d\t�����ڵ� : %d", localstack.getFrameCount(), n, result_value, NEResult::getErrorCode(result_value));
			}
#endif
		}



		//	post:
		//		���� �Լ����� �Ҵ�� �ּҸ� �ٽ� ����:
		localstack._setRecentNodeSet(before_stage_nodeset);

		return RESULT_TRUE;
	}

	type_result NENodeCodeSet::_onEnlisted()
	{
		if( ! isManaged()	||
			isEnlisted()	) 
			return RESULT_TYPE_ERROR;



		//	main:
		_setEnlisted(true);

		type_result result = RESULT_SUCCESS;
		for(int n=0; n < getLength() ;n++)
			result |= _onEnlisted(getElement(n));

		return result;
	}

	type_result NENodeCodeSet::_onEnlisted(NENode& node)
	{
		if( ! isManaged()	||
			! isEnlisted()	) 
			return RESULT_TYPE_ERROR;
		if( ! &node) return RESULT_TYPE_ERROR;
		
		return node._onEnlisted();
	}

	NEBinaryFileLoader& NENodeCodeSet::serialize(NEBinaryFileLoader& loader)
	{
		SuperClass::serialize(loader);

		if(isEnlisted())
			_onEnlisted();

		return loader;
	}

	const NENodeCodeSet& NENodeCodeSet::_assign(const NENodeCodeSet& source)
	{
		if(this == &source) return *this;

		create(source.getSize());
		push(source);

		return *this;
	}

	type_result NENodeCodeSet::resize(type_index new_size)
	{		
		/*
			�� resize�� ������ �ϴ°�:
				NETArray<T, bool, Q>���� ������ resize 
				�ٸ� ������ �����ϱ� �����̴�. �ش� �Լ������� clone
				�Լ��� ����ؼ� ��Ƶ� �ν��Ͻ��� �ӽ� ���ۿ� ��Ƶδ�
				������ �����ϰ� �ִµ�, ���������� �� �ڵ�� NEIndexedNodeSet
				���� ������ �Ұ����� �ڵ��̱� �����̴�.
				
			�� clone�� ����� �� ���°�:
				������ ������ NETEnlistableSet.hpp�� ��������� �ּ��� 
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

	void NENodeCodeSet::release()
	{
		while(getLength())
			pop();

		SuperClass::release();
	}
}