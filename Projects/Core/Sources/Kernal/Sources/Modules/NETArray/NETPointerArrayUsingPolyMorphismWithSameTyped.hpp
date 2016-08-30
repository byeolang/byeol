#pragma once

namespace NE
{
#define _NAME	NETArray<InsideType*, true, InsideType*>

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), template <typename InsideType>)
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	_NAME::NETArray()
		: Super(0), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	_NAME::NETArray(type_count size)
		: Super(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	_NAME::NETArray(const This& source)
		: Super(source), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	_NAME::~NETArray()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	const _NAME& _NAME::operator=(const This& source) 
	{
		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	_NAME _NAME::operator+(const This& source) const 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return This(*this);



		//	main:				
		//		�ӽ� ���� ����:
		This buffer(source._length + _length);
		//		���ۿ� ����:
		buffer.push(*this);
		buffer.push(source);



		//	post:
		//		���۸� ������ ��ȯ:
		return buffer;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_bool  _NAME::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(Super::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_bool  _NAME::operator!=(const This& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	const _NAME& _NAME::operator+=(const This& source)
	{
		if(this == &source)
		{
			This copyied(*this);
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



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	InsideType& _NAME::getElement(type_index index) 
	{
		//	pre:
		//		�� ���۷���:
		InsideType* null_pointer = NE_NULL;
		//		�Ķ���� �˻�:
		if(index < 0) return *null_pointer;
		if(index > getLengthLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;



		//	post:
		return *(_data[index]);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	const InsideType& _NAME::getElement(type_index index) const 
	{			
		//	pre:
		//		�� ���۷���:
		InsideType* null_pointer = NE_NULL;
		//		�Ķ���Ͱ˻�:
		if(index < 0) return *null_pointer;
		if(index > getLengthLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;



		//	post:
		return *(_data[index]);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::setElement(type_index index, const InsideType* const source)  
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if( !(&source) ) return NE_INDEX_ERROR;
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getLengthLastIndex()) return NE_INDEX_ERROR; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return NE_INDEX_ERROR;



		//	main:
		//		������ ����:
		//			�޸� ����:	�����Ͱ��� �ٲٱ����� ���������� �޸𸮸� ���� ���־� �Ѵ�.
		_returnHeapMemory(index);
		//			������ ����:
		_data[index] = static_cast<InsideType*>( &(source->clone()) ); // clone (= virtual ��������� )���� ���� �����Ѵ�

		return NE_INDEX_ERROR;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �����Ϳ� ��ġ�ϴ� ������Ʈ�� �����ϴ��� �˻���, �ε����� ����
	//				�Ѵ�.
	//	��������:	���ø� ���� InsideType�� operator==�� ���ǵǾ� �־�� �Ѵ�.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_index _NAME::find(const InsideType* const source) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if( !(&source) ) return RESULT_TYPE_ERROR | RESULT_WRONG_POINTER_ADDRESSING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(_length < 0) return NE_INDEX_ERROR;



		//	main:
		//		�˻�:
		for(type_index index=0; index < _length ;index++)
			if( *_data[index] == *source)
				return index;



		//	post:
		//		������ȯ:	��ã�Ҵ�
		return NE_INDEX_ERROR; // -1
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		This& copied_in_heap = static_cast<This&>(clone());
		create(size); // size�� ���ο��� ���õ�.
		push(copied_in_heap); // temp �ڵ� �Ҹ�



		//	post:
		delete &copied_in_heap;
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::push(const This& source)
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(!source._length)	return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;
		if(!source._size) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
		//		������ ����:
		for(type_index index=0; index < source._length ;index++) // length�� index���� 1ũ�Ƿ� <= ��� <�� ����Ѵ�
		{
			if(_length == _size) break;

			insert(_length, &(source.getElement(index)));
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::pushFront(const This& source)
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(!source._length)	return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;
		if(!source._size) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
		//		����:
		for(type_index index=0; index < source._length ;index++) // length�� index���� 1ũ�Ƿ� <= ��� <�� ����Ѵ�
		{
			if(_length == _size) break;

			insert(index, &(source.getElement(index)));
		}


		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�����͸� ������ �ε����� �����Ѵ�.	
	//	��������:	���̰� ũ�⺸�� �۾ƾ� �Ѵ�. �������� ������ �����ؾ� �Ѵ�.
	//	�޸�	:	���� 5�� �����͸� �����ϸ�, 5�� �ִ� �����ʹ� 6���� �з�����.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	�߰��� ���� ����. ���� == ũ�� �ϰ��, ��ü ũ�Ⱑ + 1 �����Ѵ�.
	template <typename InsideType>
	type_index _NAME::insert(type_index index, const InsideType* const source) 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if( !(&source) ) return NE_INDEX_ERROR;
		if(index < 0) return NE_INDEX_ERROR;
		if(index > _length) return NE_INDEX_ERROR;
		//		���� ������� �˻�:
		if(_length >= _size) resize(_size * 2 + 1);




		//	main:		
		for(type_index n = getLengthLastIndex(); n >= index ;n--)
			_data[n + 1] = _data[n]; // �����͸� ��ȯ. InsideType�� operator=�� �ƴ϶�, InsideType*�� operator=�� ȣ���ϰ� �ִ�



		//	post:
		_data[index] = static_cast<InsideType*>( &(source->clone()) );
		_length++;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_index _NAME::remove(type_index index) 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getLengthLastIndex()) return NE_INDEX_ERROR;



		//	main:
		//		������(= ������ ) ����:
		//			�޸� ����:	�����͸� �����ϱ� ���� �������� ������ �޸𸮸� ���� �����ؾ� �Ѵ�
		_returnHeapMemory(index);
		//			������ �̵��� ����:	��ĭ�� ������ �̵��Ѵ�. �� �������� _data[index]�� �����Ͱ� �Ҹ�ȴ�
		for(type_index n = index; n < getLengthLastIndex() ;n++)
			_data[n] = _data[n + 1];


		/*	
			post:
				���� ���� ������ ����:	
					_data[last_index] == _data[last_index] ��.
					�����Ͱ��� �ϳ��� �����ؾ� �ϹǷ� �̵��ϱ� ���� �����Ͱ��� 
					�η� �����.

					Q. �����͸� �������� �ʾƵ�, ������ insert�ɶ� ��¥�� ���ο�
					������ ������ ��������ٵ�?

					A. ��������� _returnHeapMemory�� ȣ���ϸ�, ���� �����͸� ������
					�־�� �� �����Ͱ� ��۸�ȭ �Ǿ������.
		*/					
		_data[getLengthLastIndex()] = NE_NULL;
		//		���� ���� ����:
		_length--;
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �迭�� ���� �迭���� Ȯ���Ѵ�.
	//	��������:	���� �迭�� �����Ǳ� ���ؼ��� 2���� ������ �ʿ��ϴ�.
	//					���� 1.	���� ũ��� ���̸� ������ �־�� ��.
	//					���� 2. ���Ұ� ���ƾ� ��.
	//					���� 3. ���ø� ���� InsideType�� operator==�� �������� ��.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�				
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_bool  _NAME::isEqualSizeAndElement(const This& source) const
	{
		//	pre:
		if(isEqualElement(source) == false) return false;
		if(_size != source._size) return false;

		//	post:
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	ũ��� ���̸� ������� �ʰ�, ���� ���Ҹ� ������ �ִ��� ���� ������.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_bool  _NAME::isEqualElement(const This& source) const
	{
		//	pre:
		if(getLengthLastIndex() != source.getLengthLastIndex()) return false;

		//	main:
		for(type_index index=0; index < _length ;index++)
		{				
			if(getElement(index) != source.getElement(index))
				return false;
		}

		//	post:
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::isValid() const  
	{
		//	pre:
		//		���� Ŭ������ VALID üũ:
		type_result result = Super::isValid();
		if(NEResult::hasError(result) == true) return result;
		//		�������:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;

		//	post:
		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	void  _NAME::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/
		return Super::release();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ũ�⸸ŭ�� �޸� ������ �Ҵ��Ѵ�.
	//				������ �����ϴ� �����ʹ� ��� �ʱ�ȭ�Ǿ �޸𸮿��� ���� �ȴ�.
	//	��������:
	//	�޸�	:	���� �����͸� ������ ä ũ�⸦ �����ϰ� �ʹٸ� resize�� ����϶�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::create(type_count size)
	{
		release();

		if(size > 0)
		{
			//	����, InsideType�� primitive�ϰ�쿡, 0���� ������ �ؾ��Ѵ�. �̰� �迭�� ǥ���ϸ�
			//		new InsideType[size] = {0, }; 
			//	�� �ȴٴ� �� �˰��ִ�.
			//	�׷��� InsideType�� Ŭ�����ϰ�쿡��, ���� �ڵ忡�� int �����ڰ� ȣ��ȴ�. ��, �������� int�����ڸ� ������ ������
			//	Ŭ������ �� ���ø�Ŭ������ ���� �� ����.
			//	�׷���, new int(); �� 0���� �ʱ�ȭ�� �ȴٴ� ��ǿ��� ����,
			//	���� �ڵ��, InsideType�� Ŭ�����̰�, primitive�̰� ���� �����ڸ� ȣ���ϰ� ������ִ� �� �ϳ��� �ڵ��.			
			_data = new InsideType*[size](); //����: Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.
			_size = size;
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	
	
	
	
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	const _NAME& _NAME::_assign(const This& source)
	{	//	pre:
		if(this == &source) return *this;

		//	main:
		create(source._size);
		push(source);

		//	pre:
		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	void  _NAME::_release() 
	{	
		if(_data)
		{
			while(_size)
			{				
				_returnHeapMemory(getSizeLastIndex());
				_size--;
			}

			delete [] _data;
		}
		_data = NE_NULL;
	}		



	//	---------------------------------------------------------------------------------
	//	����	:	�޸��Ҵ��� �����Ѵ�.
	//	��������:
	//	�޸�	:	setElement, push ��, �����͸� �Ҵ��ϱ� ���� ȣ��ȴ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType>
	type_result _NAME::_returnHeapMemory(type_index index)
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(index < 0) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(_length <= 0) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY | RESULT_ABORT_ACTION;



		//	main:
		//		�޸�����:	�����Ͱ� ��ȿ�ϸ�, �޸𸮸� ���� �����ϰ� �����͸� NULL�� �Ѵ�.
		if(_data[index])
		{
			delete _data[index];

			_data[index] = NE_NULL;
		} 

		return RESULT_SUCCESS;
	}



	template <typename InsideType>
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		/*
			Serialize�� ����:
				�������� �ݷ��ʹ� serialize�� ������ �� ����. �ִ��� ������ ��
				(���⼭�� �ݷ����� ũ�⸦ ���ϴ� ��)�� �����Ѵ�.
		*/
		//	pre:
		Super::serialize(loader);



		//	main:
		//		�������� ����:
		type_index length = getLength();
		//		���� �ʱ�ȭ:
		create(getSize());


	
		//	post:
		//		���� ���� ����:
		_length = length;
		return loader;
	}



	template <typename InsideType>
	type_index _NAME::push(const InsideType* const source)
	{
		return insert(getLength(), source);
	}

	template <typename InsideType>
	type_index _NAME::insert(type_index index, const InsideType& source)
	{
		return insert(index, &source);
	}

	template <typename InsideType>
	type_index _NAME::pop()
	{
		return remove(getLengthLastIndex());
	}

	template <typename InsideType>
	type_index _NAME::find(const InsideType& source) const
	{
		return find(&source);
	}

	template <typename InsideType>
	type_index _NAME::pushFront(const InsideType& source)
	{
		return insert(0, &source);
	}

	template <typename InsideType>
	type_index _NAME::pushFront(const InsideType* const source)
	{
		return insert(0, source);
	}

	template <typename InsideType>
	type_index _NAME::popFront()
	{
		return remove(0);
	}

#undef _NAME
}