namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NETIndexedArray<Type, pointerUseNewInstance, type>::NETIndexedArray()
		: NETReservedCollector<Type>(), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NETIndexedArray<Type, pointerUseNewInstance, type>::NETIndexedArray(type_count size)
		: NETReservedCollector<Type>(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NETIndexedArray<Type, pointerUseNewInstance, type>::NETIndexedArray(const ThisClass& source)
		: NETReservedCollector<Type>(), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NETIndexedArray<Type, pointerUseNewInstance, type>::~NETIndexedArray()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	const NETIndexedArray<Type, pointerUseNewInstance, type>&
		NETIndexedArray<Type, pointerUseNewInstance, type>::operator=
		(
		const ThisClass& source
		)
	{
		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NETIndexedArray<Type, pointerUseNewInstance, type>  
		NETIndexedArray<Type, pointerUseNewInstance, type>::operator+
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	bool  NETIndexedArray<Type, pointerUseNewInstance, type>::operator==(const NETIndexedArray<Type, pointerUseNewInstance, type>& source) const
	{
		if(this == &source) return true;
		if(NETReservedCollector<Type>::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	bool  NETIndexedArray<Type, pointerUseNewInstance, type>::operator!=(const NETIndexedArray<Type, pointerUseNewInstance, type>& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	const NETIndexedArray<Type, pointerUseNewInstance, type>&
		NETIndexedArray<Type, pointerUseNewInstance, type>::operator+=
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	Type& NETIndexedArray<Type, pointerUseNewInstance, type>::getElement(type_index index) 
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
		return _data[index];
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	const Type& NETIndexedArray<Type, pointerUseNewInstance, type>::getElement(type_index index) const 
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
		return _data[index];
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::setElement(type_index index, const Type& source)
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return RESULT_TYPE_WARNING;
		if(index > getSizeLastIndex()) return RESULT_TYPE_WARNING; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return RESULT_TYPE_WARNING;
		if( ! _occupiedset[index]) return RESULT_TYPE_ERROR;



		*(_data + index) = source;

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���� �����͸� �ִ��� ������ ä �迭�� ũ�⸦ �����Ѵ�.
	//				�翬��, ũ�Ⱑ �پ��� ���� ������ �Ϻκ��� ������� �ȴ�.
	//	��������:
	//	�޸�	:	
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		ThisClass copied = *this; // use operator=
		create(size); // size�� ���ο��� ���õ�.
		for(int n=0; n < copied._occupiedset.getSize() ;n++)
			insert(n, copied[n]);	//	source.getElement(n)�� �ùٸ��� ������ *null�� ������, �̴� insert���� ������ �� �ִ�.



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	 �迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::push(const ThisClass& source)
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::pushFront(const ThisClass& source)
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

	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::insert(type_index index, const Type& source) 
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
		if(_occupiedset.getSize() != getSize()) return NE_INDEX_ERROR;
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
		//			������ ��ȯ:	�Ļ�Ŭ�������� ������ _convert�� ����Ѵ�
		_data[index] = source;
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::remove(type_index index) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getSizeLastIndex()) return NE_INDEX_ERROR;
		//		������ �ڸ��� �ƹ��͵� ���ٸ�:
		if(_occupiedset.getSize() != getSize()) return NE_INDEX_ERROR;
		if( ! _occupiedset[index]) return NE_INDEX_ERROR;



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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	bool  NETIndexedArray<Type, pointerUseNewInstance, type>::isEqualSizeAndElement(const ThisClass& source) const
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	bool  NETIndexedArray<Type, pointerUseNewInstance, type>::isEqualElement(const ThisClass& source) const
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type> 
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = NETReservedCollector<Type>::isValid();		
		if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;
		if(_occupiedset.getSize() != getSize()) return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	void  NETIndexedArray<Type, pointerUseNewInstance, type>::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/

		return NETReservedCollector<Type>::release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NEBinaryFileSaver  &NETIndexedArray<Type, pointerUseNewInstance, type>::serialize(NEBinaryFileSaver& saver) const 
	{			
		//	pre:
		NETReservedCollector::serialize(saver);		



		//	main:
		//		���� ������ ����:
		for(type_index n=0; n < getSize() ;n++)
		{
			saver << _occupiedset[n];

			if(_occupiedset[n])
				saver << _data[n];
		}



		//	post:
		return saver;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NEBinaryFileLoader& NETIndexedArray<Type, pointerUseNewInstance, type>::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		ThisClass::release();
		NETReservedCollector::serialize(loader);		



		//	main:
		//		�޸� ���۰��� ����:
		create(_size);		
		//		������ �ε�:
		for(type_index n=0; n < getSize() ;n++)
		{
			loader >> _occupiedset[n];

			if(_occupiedset[n])
			{
				loader >> _data[n];
				_length++;
			}
		}



		//	post:
		return loader;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NEType::Type NETIndexedArray<Type, pointerUseNewInstance, type>::getType() const
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_result NETIndexedArray<Type, pointerUseNewInstance, type>::create(type_count size)
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
			_data = new Type[size](); // **����**	Type�� Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.		
			
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	const NETIndexedArray<Type, pointerUseNewInstance, type>&
		NETIndexedArray<Type, pointerUseNewInstance, type>::_assign
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	void  NETIndexedArray<Type, pointerUseNewInstance, type>::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;
		_occupiedset.release();
	}



	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::push(const Type& source)
	{
		type_index highest_vacant_index = _searchIndex(true, false);
		if(highest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;
			
		return insert(highest_vacant_index, source);
	}



	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::pop()
	{
		type_index highest_occupied_index = _searchIndex(true, true);
		if(highest_occupied_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return remove(highest_occupied_index);
	}


	
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::pushFront(const Type& source)
	{
		type_index lowest_vacant_index = _searchIndex(false, false);
		if(lowest_vacant_index == NE_INDEX_ERROR) return NE_INDEX_ERROR;

		return insert(lowest_vacant_index, source);
	}



	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::popFront()
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
	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::find(const Type& source) const
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



	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	type_index NETIndexedArray<Type, pointerUseNewInstance, type>::_searchIndex(bool by_descreasing, bool target_is_occupied) const
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



	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	NEObject& NETIndexedArray<Type, pointerUseNewInstance, type>::clone() const
	{
		return static_cast<NEObject&>( *(new ThisClass(*this)) );
	}


	template <typename Type, bool pointerUseNewInstance, NEType::Type type>
	const NEOccupiedSet& NETIndexedArray<Type, pointerUseNewInstance, type>::getOccupiedSet() const
	{
		return _occupiedset;
	}
}