namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NETIndexedArray<Type*, false, type>::NETIndexedArray()
		: SuperClass(), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NETIndexedArray<Type*, false, type>::NETIndexedArray(type_count size)
		: SuperClass(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NETIndexedArray<Type*, false, type>::NETIndexedArray(const ThisClass& source)
		: SuperClass(), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NETIndexedArray<Type*, false, type>::~NETIndexedArray()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const NETIndexedArray<Type*, false, type>&
		NETIndexedArray<Type*, false, type>::operator=
		(
		const ThisClass& source
		)
	{
		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	NETIndexedArray<Type*, false, type>  
		NETIndexedArray<Type*, false, type>::operator+
		(
		const ThisClass& source
		) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return ThisClass(*this);



		//	main:
		ThisClass buffer(source._length + _length);
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
	template <typename Type, NEType::Type type>
	bool  NETIndexedArray<Type*, false, type>::operator==(const NETIndexedArray<Type*, false, type>& source) const
	{
		if(this == &source) return true;
		if(SuperClass::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	bool  NETIndexedArray<Type*, false, type>::operator!=(const NETIndexedArray<Type*, false, type>& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	const NETIndexedArray<Type*, false, type>&
		NETIndexedArray<Type*, false, type>::operator+=
		(
		const ThisClass& source
		)
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
	Type& NETIndexedArray<Type*, false, type>::getElement(type_index index) 
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
	const Type& NETIndexedArray<Type*, false, type>::getElement(type_index index) const 
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
	type_result NETIndexedArray<Type*, false, type>::setElement(type_index index, const Type* const source)
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
	template <typename Type, NEType::Type type>
	type_result NETIndexedArray<Type*, false, type>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		ThisClass& copied_in_heap = static_cast<ThisClass&>(clone()); // use operator=
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
	template <typename Type, NEType::Type type>
	type_result NETIndexedArray<Type*, false, type>::push(const ThisClass& source)
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
	template <typename Type, NEType::Type type>
	type_result NETIndexedArray<Type*, false, type>::pushFront(const ThisClass& source)
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

	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::insert(type_index index, const Type* const source) 
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
	template <typename Type, NEType::Type type>
	type_result NETIndexedArray<Type*, false, type>::remove(type_index index) 
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
	template <typename Type, NEType::Type type>
	bool  NETIndexedArray<Type*, false, type>::isEqualSizeAndElement(const ThisClass& source) const
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
	template <typename Type, NEType::Type type>
	bool  NETIndexedArray<Type*, false, type>::isEqualElement(const ThisClass& source) const
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
	template <typename Type, NEType::Type type> 
	type_result NETIndexedArray<Type*, false, type>::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = SuperClass::isValid();		
		//if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, NEType::Type type>
	void  NETIndexedArray<Type*, false, type>::release()  
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
	NEType::Type NETIndexedArray<Type*, false, type>::getType() const
	{
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
	type_result NETIndexedArray<Type*, false, type>::create(type_count size)
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
	template <typename Type, NEType::Type type>
	const NETIndexedArray<Type*, false, type>&
		NETIndexedArray<Type*, false, type>::_assign
		(
		const ThisClass& source
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
					push(ThisClass)�� ���� �ʴ� ����:
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
	template <typename Type, NEType::Type type>
	void  NETIndexedArray<Type*, false, type>::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;
		_occupiedset.release();
	}



	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::push(const Type* const source)
	{
		type_index highest_vacant_index = _searchIndex(true, false);
		if(highest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;
			
		return insert(highest_vacant_index, source);
	}



	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::pop()
	{
		type_index highest_occupied_index = _searchIndex(true, true);
		if(highest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(highest_occupied_index);
	}


	
	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::pushFront(const Type* const source)
	{
		type_index lowest_vacant_index = _searchIndex(false, false);
		if(lowest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return insert(lowest_vacant_index, source);
	}



	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::popFront()
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
	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::find(const Type* const source) const
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



	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::_searchIndex(bool by_descreasing, bool target_is_occupied) const
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



	template <typename Type, NEType::Type type>
	NEBinaryFileSaver& NETIndexedArray<Type*, false, type>::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);

		return saver << _occupiedset;
	}


	template <typename Type, NEType::Type type>
	NEBinaryFileLoader& NETIndexedArray<Type*, false, type>::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		SuperClass::serialize(loader);



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



	template <typename Type, NEType::Type type>
	NEObject& NETIndexedArray<Type*, false, type>::clone() const
	{
		return *(new ThisClass(*this));
	}



	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::insert(type_index index, const Type& source)
	{
		return insert(index, &source);
	}

	template <typename Type, NEType::Type type>
	type_index NETIndexedArray<Type*, false, type>::pushFront(const Type& source)
	{
		return pushFront(index, &source);
	}



	template <typename Type, NEType::Type type>
	const NEOccupiedSet& NETIndexedArray<Type*, false, type>::getOccupiedSet() const
	{
		return _occupiedset;
	}
}