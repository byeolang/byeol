namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEIndexedArrayTemplate<Type*, true, type>::NEIndexedArrayTemplate()
		: SuperClass(0), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEIndexedArrayTemplate<Type*, true, type>::NEIndexedArrayTemplate(type_count size)
		: SuperClass(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEIndexedArrayTemplate<Type*, true, type>::NEIndexedArrayTemplate(const ThisClass& source)
		: SuperClass(source), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEIndexedArrayTemplate<Type*, true, type>::~NEIndexedArrayTemplate()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const NEIndexedArrayTemplate<Type*, true, type>& NEIndexedArrayTemplate<Type*, true, type>::operator=(const ThisClass& source) 
	{
		NEReservedCollectorTemplate::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEIndexedArrayTemplate<Type*, true, type> NEIndexedArrayTemplate<Type*, true, type>::operator+(const ThisClass& source) const 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return ThisClass(*this);



		//	main:				
		//		�ӽ� ���� ����:
		ThisClass buffer(source._length + _length);
		//		���ۿ� ����:
		buffer.push(*this);
		buffer.push(source);



		//	post:
		//		���۸� ������ ��ȯ:
		return buffer;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	bool  NEIndexedArrayTemplate<Type*, true, type>::operator==(const ThisClass& source) const
	{
		if(this == &source) return true;
		if(SuperClass::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	bool  NEIndexedArrayTemplate<Type*, true, type>::operator!=(const ThisClass& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const NEIndexedArrayTemplate<Type*, true, type>& NEIndexedArrayTemplate<Type*, true, type>::operator+=(const ThisClass& source)
	{
		if(this == &source)
		{
			ThisClass copyied(*this);
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
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	Type& NEIndexedArrayTemplate<Type*, true, type>::getElement(type_index index) 
	{
		//	pre:
		//		�� ���۷���:
		Type* null_pointer = NE_NULL;
		//		�Ķ���� �˻�:
		if(index < 0) return *null_pointer;
		if(index > getSizeLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;
		if( ! _occupiedset[index]) return *null_pointer;



		//	post:
		return *(_data[index]);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const Type& NEIndexedArrayTemplate<Type*, true, type>::getElement(type_index index) const 
	{			
		//	pre:
		//		�� ���۷���:
		Type* null_pointer = NE_NULL;
		//		�Ķ���Ͱ˻�:
		if(index < 0) return *null_pointer;
		if(index > getSizeLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;
		if( ! _occupiedset[index]) return *null_pointer;



		//	post:
		return *(_data[index]);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::setElement(type_index index, const Type* const source)  
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if( !(&source) ) return NE_INDEX_ERROR;
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return NE_INDEX_ERROR;
		if( ! _occupiedset[index]) return RESULT_TYPE_ERROR;



		//	main:
		//		������ ����:
		//			�޸� ����:	�����Ͱ��� �ٲٱ����� ���������� �޸𸮸� ���� ���־� �Ѵ�.
		_returnHeapMemory(index);
		//			������ ����:
		_data[index] = static_cast<Type*>( &(source->clone()) ); // clone (= virtual ��������� )���� ���� �����Ѵ�
		_occupiedset[index] = true;

		return NE_INDEX_ERROR;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �����Ϳ� ��ġ�ϴ� ������Ʈ�� �����ϴ��� �˻���, �ε����� ����
	//				�Ѵ�.
	//	��������:	���ø� ���� Type�� operator==�� ���ǵǾ� �־�� �Ѵ�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::find(const Type* const source) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if( !(&source) ) return RESULT_TYPE_ERROR | RESULT_WRONG_POINTER_ADDRESSING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(_length < 0) return NE_INDEX_ERROR;



		//	main:
		//		�˻�:
		for(type_index index=0; index < _length ;index++)
			if( _data[index] == source)
				return index;



		//	post:
		//		������ȯ:	��ã�Ҵ�
		return NE_INDEX_ERROR; // -1
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		ThisClass& copied_in_heap = static_cast<ThisClass&>(clone());
		create(size); // size�� ���ο��� ���õ�.
		for(int n=0; n < copied_in_heap.getSize() ;n++)
			insert(n, copied_in_heap[n]);



		//	post:
		delete &copied_in_heap;
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::push(const ThisClass& source)
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

			push(source.getElement(index));
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::pushFront(const ThisClass& source)
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

			pushFront(source.getElement(index));
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�����͸� ������ �ε����� �����Ѵ�.	
	//	��������:	���̰� ũ�⺸�� �۾ƾ� �Ѵ�. �������� ������ �����ؾ� �Ѵ�.
	//	�޸�	:	���� 5�� �����͸� �����ϸ�, 5�� �ִ� �����ʹ� 6���� �з�����.
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	�߰��� ���� ����. ���� == ũ�� �ϰ��, ��ü ũ�Ⱑ + 1 �����Ѵ�.
	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::insert(type_index index, const Type* const source) 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if( !(&source) ) return NE_INDEX_ERROR;
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR;
		//		���� ������� �˻�:
		//			NEArray�� �ٸ���, �����ִٸ� ������ ��ҿ� setElement�� �õ��Ѵ�.
		if(_length > _size) return NE_INDEX_ERROR;
		//		������ �ڸ��� �̹� ���� �ִٸ�:
		//			������ �����Ѵ�.
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;
		if(_occupiedset[index]) 
		{
			if(NEResult::hasError(setElement(index, source)))
				return NE_INDEX_ERROR;
			return index;			
		}




		//	main:
		//		������ ȹ��:
		//			�� �������� ���� ȹ���ϴ°�:
		//				source->clone�� ���ؼ� ������ �����͵� �ȿ� �� �ٽ� NEIndexTemplate�� ���� 
		//				��찡 ���� �� �ִ�. �̶� ������� �ν��Ͻ��� �ڽ��̶� �ϰ�, ������ ȣ���ڸ�
		//				�θ�� ����.
		//				�θ��� ������ ���̺��� set ���� �ʰ� clone���� �ڽ��� �����ع�����, �ڽĿ� 
		//				���忡�� ���� NEIndexeTemplate�� ������ ����ִ� �ε����� ã��, ���������
		//				�θ��� �ڸ��� �������� �ȴ�.
		_occupiedset[index] = true;
		//		�����ͻ���:
		//			������ ��ȯ:
		_data[index] = static_cast<Type*>( &(source->clone()) );
		
		//		���� ����:
		_length++;



		//	post:
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::remove(type_index index) 
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR;
		//		������ �ڸ��� �ƹ��͵� ���ٸ�:
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;
		if( ! _occupiedset[index]) return RESULT_SUCCESS | RESULT_ABORT_ACTION;



		//	main:
		//		������(= ������ ) ����:
		//			�޸� ����:	�����͸� �����ϱ� ���� �������� ������ �޸𸮸� ���� �����ؾ� �Ѵ�
		_returnHeapMemory(index);


		//	post:
		//		���� ���� ����:
		_length--;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �迭�� ���� �迭���� Ȯ���Ѵ�.
	//	��������:	���� �迭�� �����Ǳ� ���ؼ��� 2���� ������ �ʿ��ϴ�.
	//					���� 1.	���� ũ��� ���̸� ������ �־�� ��.
	//					���� 2. ���Ұ� ���ƾ� ��.
	//					���� 3. ���ø� ���� Type�� operator==�� �������� ��.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�				
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	bool  NEIndexedArrayTemplate<Type*, true, type>::isEqualSizeAndElement(const ThisClass& source) const
	{
		//	pre:
		if(_size != source._size) return false;
		if(getLength() != source.getLength()) return false;
		if(isEqualElement(source) == false) return false;
		


		//	post:
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	ũ��� ���̸� ������� �ʰ�, ���� ���Ҹ� ������ �ִ��� ���� ������.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	bool  NEIndexedArrayTemplate<Type*, true, type>::isEqualElement(const ThisClass& source) const
	{
		//	pre:
		if(getLengthLastIndex() != source.getLengthLastIndex()) return false;
		if(_occupiedset != source._occupiedset) return false;

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
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::isValid() const  
	{
		//	pre:
		//		���� Ŭ������ VALID üũ:
		type_result result = SuperClass::isValid();
		if(NEResult::hasError(result) == true) return result;		
		//		�������:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;

		//	post:
		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	void  NEIndexedArrayTemplate<Type*, true, type>::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/
		return SuperClass::release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NEType::Type NEIndexedArrayTemplate<Type*, true, type>::getType() const
	{
		if(type == NEType::NEARRAY_TEMPLATE)
			return NEType::NEPOINTER_ARRAY_WITH_POLYMORPHISM_TEMPLATE;



		return type;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ũ�⸸ŭ�� �޸� ������ �Ҵ��Ѵ�.
	//				������ �����ϴ� �����ʹ� ��� �ʱ�ȭ�Ǿ �޸𸮿��� ���� �ȴ�.
	//	��������:
	//	�޸�	:	���� �����͸� ������ ä ũ�⸦ �����ϰ� �ʹٸ� resize�� ����϶�.
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::create(type_count size)
	{
		release();

		if(size > 0)
		{
			//	����, Type�� primitive�ϰ�쿡, 0���� ������ �ؾ��Ѵ�. �̰� �迭�� ǥ���ϸ�
			//		new Type[size] = {0, }; 
			//	�� �ȴٴ� �� �˰��ִ�.
			//	�׷��� Type�� Ŭ�����ϰ�쿡��, ���� �ڵ忡�� int �����ڰ� ȣ��ȴ�. ��, �������� int�����ڸ� ������ ������
			//	Ŭ������ �� ���ø�Ŭ������ ���� �� ����.
			//	�׷���, new int(); �� 0���� �ʱ�ȭ�� �ȴٴ� ��ǿ��� ����,
			//	���� �ڵ��, Type�� Ŭ�����̰�, primitive�̰� ���� �����ڸ� ȣ���ϰ� ������ִ� �� �ϳ��� �ڵ��.			
			_data = new Type*[size](); //����: Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.

			_occupiedset.create(size);
			for(int n=0; n < _occupiedset.getSize() ;n++)
				_occupiedset.push(false);

			_size = size;
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	
	
	
	
	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const NEIndexedArrayTemplate<Type*, true, type>& NEIndexedArrayTemplate<Type*, true, type>::_assign(const ThisClass& source)
	{	//	pre:
		if(this == &source) return *this;

		//	main:
		//		���� ����:
		create(source._size);
		//		����:
		//			���� �˰���:
		/*
						�������� ���� �����ϰ�, source�� �������� Ž���Ѵ�.
						Ž���ϸ鼭 source�� �������� �����ϸ�, �ش� ���Ҹ� �����ϰ�,
						�������� set �Ѵ�.
					push(ThisClass)�� ���� �ʴ� ����:
						push�� "���� ���� ���ڸ�"�� �ִ� �Լ��̹Ƿ�, �̷��� �ϸ�
						source�� �ε����� �޶����� �ȴ�.
		*/
		const NEBooleanSet& source_occupiedset = source._occupiedset;
		for(int n=0; n < source_occupiedset.getLength() ;n++)
			if(source_occupiedset[n])
				insert(n, source.getElement(n));

		//	pre:
		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	void  NEIndexedArrayTemplate<Type*, true, type>::_release() 
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
		_occupiedset.release();
	}		



	//	---------------------------------------------------------------------------------
	//	����	:	�޸��Ҵ��� �����Ѵ�.
	//	��������:
	//	�޸�	:	setElement, push ��, �����͸� �Ҵ��ϱ� ���� ȣ��ȴ�.
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	type_result NEIndexedArrayTemplate<Type*, true, type>::_returnHeapMemory(type_index index)
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(index < 0) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(_length <= 0) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY | RESULT_ABORT_ACTION;



		//	main:
		//		�޸�����:	����, �������� Ȯ���Ѵ�.
		if(_occupiedset[index])
			delete _data[index];
		//		������ �Ҹ�:
		//			��ü�� ������ ������� �� insert���� ���������.
		//			�޸𸮰� ��ȯ�Ǵ� ��, NEIndexedArrayTemplate ��ü�� ��ȯ�ɶ���.
		_occupiedset[index] = false;

		return RESULT_SUCCESS;
	}



	template <typename Type, NEType::Type type>
	NEBinaryFileLoader& NEIndexedArrayTemplate<Type*, true, type>::serialize(NEBinaryFileLoader& loader)
	{
		/*
			Serialize�� ����:
				�������� �ݷ��ʹ� serialize�� ������ �� ����. �ִ��� ������ ��
				(���⼭�� �ݷ����� ũ�⸦ ���ϴ� ��)�� �����Ѵ�.
		*/
		//	pre:
		SuperClass::serialize(loader);



		//	main:
		//		�������� ����:
		type_index length = getLength();
		//		���� �ʱ�ȭ:
		create(getSize());


	
		//	post:
		//		���� ���� ����:
		_length = length;
		return loader >> _occupiedset;
	}



	template <typename Type, NEType::Type type>
	NEBinaryFileSaver& NEIndexedArrayTemplate<Type*, true, type>::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);

		return saver << _occupiedset;
	}



	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::_searchIndex(bool by_decreasing, bool target_is_occupied) const
	{
		if( ! by_decreasing)
		{
			for(int n=0; n < _occupiedset.getLength() ;n++)
				if(_occupiedset[n] == target_is_occupied)
					return n;
		}
		else
		{
			for(int n=_occupiedset.getLengthLastIndex(); n >= 0 ;n--)
				if(_occupiedset[n] == target_is_occupied)
					return n;
		}


		return NE_INDEX_ERROR;
	}



	template <typename Type, NEType::Type type>
	NEObject& NEIndexedArrayTemplate<Type*, true, type>::clone() const
	{
		return *(new ThisClass(*this));
	}



	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::push(const Type* const source)
	{
		type_index highest_vacant_index = _searchIndex(true, false);
		if(highest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return insert(highest_vacant_index, source);
	}

	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::insert(type_index index, const Type& source)
	{
		return insert(index, &source);
	}

	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::pop()
	{
		type_index highest_occupied_index = _searchIndex(true, true);
		if(highest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(highest_occupied_index);
	}

	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::pushFront(const Type& source)
	{
		return pushFront(&source);
	}

	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::pushFront(const Type* const source)
	{
		type_index lowest_vacant_index = _searchIndex(false, false);
		if(lowest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return insert(lowest_vacant_index, source);
	}

	template <typename Type, NEType::Type type>
	type_index NEIndexedArrayTemplate<Type*, true, type>::popFront()
	{
		type_index lowest_occupied_index = _searchIndex(false, true);
		if(lowest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(lowest_occupied_index);
	}



	template <typename Type, NEType::Type type>
	const NEOccupiedSet& NEIndexedArrayTemplate<Type*, true, type>::getOccupiedSet() const
	{
		return _occupiedset;
	}
}