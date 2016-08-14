namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	NETIndexedArray<Type*, false>::NETIndexedArray()
		: Super(), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	NETIndexedArray<Type*, false>::NETIndexedArray(type_count size)
		: Super(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	NETIndexedArray<Type*, false>::NETIndexedArray(const This& source)
		: Super(), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	NETIndexedArray<Type*, false>::~NETIndexedArray()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	const NETIndexedArray<Type*, false>&
		NETIndexedArray<Type*, false>::operator=
		(
		const This& source
		)
	{
		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	NETIndexedArray<Type*, false>  
		NETIndexedArray<Type*, false>::operator+
		(
		const This& source
		) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return This(*this);



		//	main:
		This buffer(source._length + _length);
		buffer.push(*this);
		buffer.push(source);



		// post:
		return buffer;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���Ҹ� ������ true�ΰ���, ��������� ���ƾ� �ϴ����� ���� ��ȣ���� 
	//				�ֱ⶧���� �״��� ��õ���� �ʴ´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------	
	template <typename Type>
	type_bool  NETIndexedArray<Type*, false>::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(Super::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_bool  NETIndexedArray<Type*, false>::operator!=(const This& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	const NETIndexedArray<Type*, false>&
		NETIndexedArray<Type*, false>::operator+=
		(
		const This& source
		)
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
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	Type& NETIndexedArray<Type*, false>::getElement(type_index index) 
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
	template <typename Type>
	const Type& NETIndexedArray<Type*, false>::getElement(type_index index) const 
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
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::setElement(type_index index, const Type* const source)
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return RESULT_TYPE_WARNING;
		if(index > getSizeLastIndex()) return RESULT_TYPE_WARNING; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return RESULT_TYPE_WARNING;
		if( ! _occupiedset[index]) return RESULT_TYPE_ERROR;


		
		Type* const_casted_source = const_cast<Type*>(source);
		*(_data + index) = const_casted_source;

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���� �����͸� �ִ��� ������ ä �迭�� ũ�⸦ �����Ѵ�.
	//				�翬��, ũ�Ⱑ �پ��� ���� ������ �Ϻκ��� ������� �ȴ�.
	//	��������:
	//	�޸�	:	
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		This& copied_in_heap = static_cast<This&>(clone()); // use operator=
		create(size); // size�� ���ο��� ���õ�.
		for(int n=0; n < copied_in_heap._occupiedset.getSize() ;n++)
			insert(n, copied_in_heap[n]);	//	source.getElement(n)�� �ùٸ��� ������ *null�� ������, �̴� insert���� ������ �� �ִ�.



		//	post:
		delete &copied_in_heap;
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	 �迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::push(const This& source)
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if( ! source.getLength())	return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if( ! source.getSize()) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
		for(type_index index=0; index < source.getLength() ;index++) // length�� index���� 1ũ�Ƿ� <= ��� <�� ����Ѵ�
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
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::pushFront(const This& source)
	{
		//	pre:
		//		�Ķ���� �˻�:
		if( ! source.getLength() )	return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if( ! source.getSize() ) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
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

	template <typename Type>
	type_index NETIndexedArray<Type*, false>::insert(type_index index, const Type* const source) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if( ! &source) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR;
		//		���ø������ �˻�:
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
		Type* const_casted_source = const_cast<Type*>(source);
		_data[index] = const_casted_source;
		//		���� ����:
		_length++;

		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::remove(type_index index) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR;
		//		������ �ڸ��� �ƹ��͵� ���ٸ�:
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;
		if( ! _occupiedset[index]) return RESULT_SUCCESS | RESULT_ABORT_ACTION;



		//	main:
		//		������ �Ҹ�:
		/*
					��ü�� ������ ������� �� insert���� ���������.
					�޸𸮰� ��ȯ�Ǵ� ��, NETIndexedArray ��ü�� ��ȯ�ɶ���.
		*/
		_occupiedset[index] = false;



		//	post:
		//		���� ����:
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
	template <typename Type>
	type_bool  NETIndexedArray<Type*, false>::isEqualSizeAndElement(const This& source) const
	{
		//	pre:
		if(_size != source._size) return false;
		if(_length != source._length) return false;
		if(!isEqualElement(source)) return false;		

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	ũ��� ���̸� ������� �ʰ�, ���� ���Ҹ� ������ �ִ��� ���� ������.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_bool  NETIndexedArray<Type*, false>::isEqualElement(const This& source) const
	{
		if(getLengthLastIndex() != source.getLengthLastIndex()) return false;
		if(_occupiedset != source._occupiedset) return false;

		for(type_index index=0; index < _length ;index++)
		{				
			if(getElement(index) != source.getElement(index))
				return false;
		}

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type> 
	type_result NETIndexedArray<Type*, false>::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = Super::isValid();		
		//if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	void  NETIndexedArray<Type*, false>::release()  
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
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	type_result NETIndexedArray<Type*, false>::create(type_count size)
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
			_data = new Type*[size](); // **����**	Type�� Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.		
			
			_occupiedset.create(size);
			for(int n=0; n < size ;n++)
				_occupiedset.push(false);

			_size = size;
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	const NETIndexedArray<Type*, false>&
		NETIndexedArray<Type*, false>::_assign
		(
		const This& source
		)
	{	
		//	pre:
		if(this == &source) return *this;



		//	main:
		//		���� ����:
		create(source._size);	//	���ο��� occupiedtable�� �ʱ�ȭ �ȴ�.
		//		����:
		//			���� �˰���:
		/*
						�������� ���� �����ϰ�, source�� �������� Ž���Ѵ�.
						Ž���ϸ鼭 source�� �������� �����ϸ�, �ش� ���Ҹ� �����ϰ�,
						�������� set �Ѵ�.
					push(This)�� ���� �ʴ� ����:
						push�� "���� ���� ���ڸ�"�� �ִ� �Լ��̹Ƿ�, �̷��� �ϸ�
						source�� �ε����� �޶����� �ȴ�.
		*/
		const NEBooleanSet& source_occupiedset = source._occupiedset;
		for(int n=0; n < source_occupiedset.getLength() ;n++)
			if(source_occupiedset[n])
				insert(n, source.getElement(n));

		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type>
	void  NETIndexedArray<Type*, false>::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;
		_occupiedset.release();
	}



	template <typename Type>
	type_index NETIndexedArray<Type*, false>::push(const Type* const source)
	{
		type_index highest_vacant_index = _searchIndex(true, false);
		if(highest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;
			
		return insert(highest_vacant_index, source);
	}



	template <typename Type>
	type_index NETIndexedArray<Type*, false>::pop()
	{
		type_index highest_occupied_index = _searchIndex(true, true);
		if(highest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(highest_occupied_index);
	}


	
	template <typename Type>
	type_index NETIndexedArray<Type*, false>::pushFront(const Type* const source)
	{
		type_index lowest_vacant_index = _searchIndex(false, false);
		if(lowest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return insert(lowest_vacant_index, source);
	}



	template <typename Type>
	type_index NETIndexedArray<Type*, false>::popFront()
	{
		type_index lowest_occupied_index = _searchIndex(false, true);
		if(lowest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(lowest_occupied_index);
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �����Ϳ� ��ġ�ϴ� ������Ʈ�� �����ϴ��� �˻���, �ε����� ����
	//				�Ѵ�.
	//	��������:	���ø� ���� Type�� operator==�� ���ǵǾ� �־�� �Ѵ�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//     	---------------------------------------------------------------------------------
	template <typename Type>
	type_index NETIndexedArray<Type*, false>::find(const Type* const source) const
	{
		//	pre:
		//		���ø������ �˻�:
		if(_length <= 0) return NE_INDEX_ERROR;



		//	main:
		//		�˻�:
		for(type_index index=0; index < _length ;index++)
			if(_data[index] == source) // ������ ��ü�� ���Ѵ�
				return index;



		//	post:
		//		������ȯ:	ã�� ������
		return NE_INDEX_ERROR; // -1
	}



	template <typename Type>
	type_index NETIndexedArray<Type*, false>::_searchIndex(type_bool by_descreasing, type_bool target_is_occupied) const
	{
		if( ! by_descreasing)
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



	template <typename Type>
	NEBinaryFileSaver& NETIndexedArray<Type*, false>::serialize(NEBinaryFileSaver& saver) const
	{
		Super::serialize(saver);

		return saver << _occupiedset;
	}


	template <typename Type>
	NEBinaryFileLoader& NETIndexedArray<Type*, false>::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		Super::serialize(loader);



		//	main:
		//		���� ���� ����:
		type_index length = getLength();
		//		���� ������:
		create(getSize());



		//	post:
		//		���� ���� ����:
		_length = length;
		return loader >> _occupiedset;
	}



	template <typename Type>
	type_index NETIndexedArray<Type*, false>::insert(type_index index, const Type& source)
	{
		return insert(index, &source);
	}

	template <typename Type>
	type_index NETIndexedArray<Type*, false>::pushFront(const Type& source)
	{
		return pushFront(index, &source);
	}



	template <typename Type>
	const NEOccupiedSet& NETIndexedArray<Type*, false>::getOccupiedSet() const
	{
		return _occupiedset;
	}
}