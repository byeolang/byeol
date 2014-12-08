#include "NEModuleCodeSet.hpp"
#include "../Kernal/Kernal.hpp"
#include "../NEIndexedModuleSet/NEIndexedModuleSet.hpp"

namespace NE
{
	NEModuleCodeSet::NEModuleCodeSet()
		: SuperClass()
	{
		
	}
	NEModuleCodeSet::NEModuleCodeSet(type_index size)
		: SuperClass(size)
	{
		
	}
	NEModuleCodeSet::NEModuleCodeSet(const NEModuleCodeSet& source)
		: SuperClass(source)
	{
		_assign(source);	//	����, NENodeCodeSet::insert�� ���������� ȣ��ȴ�.
	}

	NEModuleCodeSet::~NEModuleCodeSet()
	{
		release();	//	���⼭ ���� ���� ������ ����Ŭ���� �Ҹ��ڿ��� ����Ŭ���� release�� ȣ���ع��� ���̴�.
	}

	const NEModuleCodeSet& NEModuleCodeSet::operator+=(const NEModuleCodeSet& source)
	{
		if(this == &source)
		{
			NEModuleCodeSet copyied(*this);
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

	NEModuleCodeSet NEModuleCodeSet::operator+(const NEModuleCodeSet& source) const
	{
		ThisClass buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	type_index NEModuleCodeSet::insert(type_index index, const NEModule& source)
	{
		//	pre:
		NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}


		//	main:
		type_index modulecode = moduleset.pushFront(source);
		if(modulecode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ")
				return NE_INDEX_ERROR;
		}
		if( NEResult::hasError(_insert(index, modulecode)) )
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}
		//	Owner ����:
		moduleset[modulecode]._owner = this;
		moduleset[modulecode]._index = index;
		


		//	post:
		return index;
	}
	type_index NEModuleCodeSet::remove(type_index index)
	{
		//	pre:
		NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		type_index modulecode = moduleset.remove(_getElement(index));
		if(modulecode == NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}

		return _remove(index);
	}

	NEModule& NEModuleCodeSet::getElement(type_index index)
	{
		//	pre:
		NEModule* nullpointer = 0x00;
		NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return *nullpointer;
		}



		//	main:
		type_index& modulecode = _getElement(index);
		if( ! &modulecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return *nullpointer;
		}

		return moduleset[modulecode];
	}
	const NEModule& NEModuleCodeSet::getElement(type_index index) const
	{
		//	pre:
		NEModule* nullpointer = 0x00;
		const NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return *nullpointer;
		}



		//	main:
		const type_code& modulecode = _getElement(index);
		if( ! &modulecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return *nullpointer;
		}

		return moduleset[modulecode];
	}
	type_result NEModuleCodeSet::setElement(type_index index, const NEModule& source)
	{
		//	pre:
		NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}



		//	main:
		//	TEST: ���ڵ常���ε� ����� node::unlist�� �����ϴ°�?
		type_index& modulecode = _getElement(index);
		if( ! &modulecode)
		{
			KERNAL_ERROR(" : �־��� �ε����� ������ ��� �߸��� �����Դϴ�.");

			return RESULT_TYPE_ERROR;
		}
		return moduleset.setElement(modulecode, source);
	}
	type_index NEModuleCodeSet::find(const NEModule& source) const
	{
		//	pre:
		const NEIndexedModuleSet& moduleset = _getModuleSet();
		if(! &moduleset)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		//	main:
		for(int n=0; n < getLength() ;n++)
		{
			type_index modulecode = _getElement(n);
			const NEModule& unit = moduleset[modulecode];
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
	NEObject& NEModuleCodeSet::clone() const
	{
		return *(new ThisClass(*this));
	}
	NEType::Type NEModuleCodeSet::getType() const
	{
		return NEType::NEMODULE_CODESET;
	}
	type_result NEModuleCodeSet::execute()
	{
		//	pre:
		//		Ÿ����:
		type_result result = RESULT_SUCCESS;
		NENodeManager::LocalStack& localstack = Kernal::getInstance().getNodeManager().getLocalStack();
		//		���� ���� ����:
		//			���ο� ���ý����� �ֱ����� ���� �� ����:
		//				�� �������� �����ϴ°�:
		//					���� �ȿ� ������ �ִ� �������, ���� ������ ó���ϰ� �ٽ�
		//					�ܺ� �������� ���ƿ����� ���� ���°� ���ý��ÿ� ����� ���
		//					���� ��߳� �� �ֱ� �����̴�.
		NEModuleCodeSet& before_stage_moduleset = localstack.getRecentModuleSet();
		localstack._setRecentModuleSet(*this);
		localstack._setRecentResultCode(RESULT_NOTHING);


		
		//	main:
		//		��� ����:
		for(type_index n=0; n < _length ;n++)
		{
			NEModule& module = getElement(n);

			//		����:
			//			���� ���� ����:
			if( ! module.isEnable()) continue;
			localstack._setRecentResultCode(module.execute());
			//			����� ����:	����, ����� �ϳ��� false Ȥ�� ������ �ִٸ�, �������θ� �����ؾ��Ѵ�
			localstack._setRecentModuleSetIndex(n);
			if(localstack.getRecentResultCode() & RESULT_FALSE)
				result = result | RESULT_FALSE;			
#ifdef DEVELOPER			
			//			����� Flag ����:	���� Ŭ����(node.execute)�� ������ ������.
			if(NEResult::hasError(localstack.getRecentResultCode()) == true)
				NEResult::addErrorFlag(result, localstack.getRecentResultCode());
			//			����� ���:
			if(NEResult::hasError(localstack.getRecentResultCode()) == true)
			{
				const NEExportable::Identifier& identifier = module.getHeader();
				type_result result_value = localstack.getRecentResultCode();
				KERNAL_ERROR("E100031C4 : ����� ���� �߻�\n���¿��� ������ ����� ������ ��ȯ�߽��ϴ�\n������ ī��Ʈ : %d\t����ȣ : %d\n����ڵ� : %d\t�����ڵ� : %d\n��� �ĺ���:\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d", localstack.getFrameCount(), n, result_value, NEResult::getErrorCode(result_value), identifier.getName().toCharPointer(), identifier.getDeveloper().toCharPointer(), identifier.getRevision())
			}
#endif
		}



		//	post:
		//		���� �Լ����� �Ҵ�� �ּҸ� �ٽ� ����:
		localstack._setRecentModuleSet(before_stage_moduleset);

		return RESULT_TRUE;
	}

	const NEModuleCodeSet& NEModuleCodeSet::_assign(const NEModuleCodeSet& source)
	{
		if(this == &source) return *this;

		create(source.getSize());
		push(source);

		return *this;
	}

	type_result NEModuleCodeSet::resize(type_index new_size)
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

	void NEModuleCodeSet::release()
	{
		while(getLength())
			pop();

		SuperClass::release();
	}

	NEIndexedModuleSet& NEModuleCodeSet::_getModuleSet()
	{
		NEIndexedModuleSet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
				return *nullpointer;
		}
		return getManager()._getModuleSet();
	}

	const NEIndexedModuleSet& NEModuleCodeSet::_getModuleSet() const
	{
		const NEIndexedModuleSet* nullpointer = 0x00;
		if( ! isManaged())
		{
			KERNAL_ERROR(" : ")
			return *nullpointer;
		}
		return getManager().getModuleSet();
	}
}