namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType*, false, InsideType*, type>::NEArrayTemplate()
		: SuperClass(0), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType*, false, InsideType*, type>::NEArrayTemplate(type_count size)
		: SuperClass(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType*, false, InsideType*, type>::NEArrayTemplate(typename const ThisClass& source)
		: SuperClass(source), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType*, false, InsideType*, type>::~NEArrayTemplate()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType*, false, InsideType*, type>&
		NEArrayTemplate<InsideType*, false, InsideType*, type>::operator=
		(
		const ThisClass& source
		)
	{
		NEReservedCollectorTemplate<InsideType*>::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���Ҹ� ������ true�ΰ���, ��������� ���ƾ� �ϴ����� ���� ��ȣ���� 
	//				�ֱ⶧���� �״��� ��õ���� �ʴ´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------	
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType*, false, InsideType*, type>::operator==(const ThisClass& source) const
	{
		if(this == &source) return true;
		if(NEReservedCollectorTemplate<InsideType*>::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType*, false, InsideType*, type>::operator!=(const ThisClass& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType*, false, InsideType*, type>
		NEArrayTemplate<InsideType*, false, InsideType*, type>::operator+(const ThisClass& source) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return ThisClass(*this);



		//	main:
		ThisClass buffer(source._length + _length);
		buffer.push(*this);
		buffer.push(source);



		//	post:
		return buffer;
	}



	template <typename InsideType, NEType::Type type>
	NEObject& NEArrayTemplate<InsideType*, false, InsideType*, type>::clone() const
	{
		return *(new ThisClass(*this));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType*, false, InsideType*, type>&
		NEArrayTemplate<InsideType*, false, InsideType*, type>::operator+=
		(
		const ThisClass& source
		)
	{
		if(this == &source)
		{
			ThisClass& copyied_in_heap = static_cast<ThisClass&>(clone());
			resize(_length + copyied_in_heap._length);
			push(copyied_in_heap);
			delete &copyied_in_heap;
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
	template <typename InsideType, NEType::Type type>
	InsideType& NEArrayTemplate<InsideType*, false, InsideType*, type>::getElement(type_index index) 
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
	template <typename InsideType, NEType::Type type>
	const InsideType& NEArrayTemplate<InsideType*, false, InsideType*, type>::getElement(type_index index) const 
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
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType*, false, InsideType*, type>::setElement(type_index index, const InsideType* const source)  
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return RESULT_TYPE_WARNING;
		if(index > getLengthLastIndex()) return RESULT_TYPE_WARNING; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return RESULT_TYPE_WARNING; 


		InsideType* const_casted_source = const_cast<InsideType*>(source);
		*(_data + index) = const_casted_source;
		
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���� �����͸� �ִ��� ������ ä �迭�� ũ�⸦ �����Ѵ�.
	//				�翬��, ũ�Ⱑ �پ��� ���� ������ �Ϻκ��� ������� �ȴ�.
	//	��������:
	//	�޸�	:	
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType*, false, InsideType*, type>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		ThisClass& copied_in_heap = static_cast<ThisClass&>(clone());
		create(size); // size�� ���ο��� ���õ�.
		push(copied_in_heap); // temp �ڵ� �Ҹ�



		//	post:
		delete &copied_in_heap;
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	 �迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result  NEArrayTemplate<InsideType*, false, InsideType*, type>::push(const ThisClass& source)
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(!source._length)	return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(!source._size) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
		for(type_index index=0; index < source._length ;index++) // length�� index���� 1ũ�Ƿ� <= ��� <�� ����Ѵ�
		{
			if(_length == _size) break;
			insert(_length, source.getElement(index));	
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType*, false, InsideType*, type>::pushFront(const ThisClass& source)
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(!source._length)	return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(!source._size) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if(&source == this) return RESULT_TYPE_WARNING | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION; // ������������, assign�迭���� �ڱ��ڽ��� ����ó���ϴ°� ��Ģ�̹Ƿ� ������ ���ؼ� ����ó���� �Ѵ�.



		//	main:
		for(type_index index=0; index < source._length ;index++) // length�� index���� 1ũ�Ƿ� <= ��� <�� ����Ѵ�
		{
			if(_length == _size) break;

			insert(index, &(source.getElement(index)));
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�����͸� ������ �ε����� �����Ѵ�.	
	//	��������:	���̰� ũ�⺸�� �۾ƾ� �Ѵ�. �������� ������ �����ؾ� �Ѵ�.
	//	�޸�	:	���� 5�� �����͸� �����ϸ�, 5�� �ִ� �����ʹ� 6���� �з�����.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::insert(type_index index, const InsideType* const source) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > _length) return NE_INDEX_ERROR;
		//		���ø������ �˻�:
		if(_length >= _size) return NE_INDEX_ERROR;



		//	main:
		//		���Ե� ��������:	�����͸� ��ĭ�� ������ �ű��
		for(type_index n = getLengthLastIndex(); n >= index ;n--)
			_data[n + 1] = _data[n];



		//	post:
		//		�����ͻ���:
		InsideType* const_casted_source = const_cast<InsideType*>(source);
		_data[index] = const_casted_source;
		//		���� ����:
		_length++;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::remove(type_index index) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if(index > getLengthLastIndex()) return NE_INDEX_ERROR;



		//	main:
		//		������ �̵�:	��ĭ�� ������ �ű��. �� �������� ������ ���ÿ� �Ͼ��
		for(type_index n = index; n < getLengthLastIndex() ;n++)
			_data[n] = _data[n + 1];



		//	post:
		//		���� ����:
		_length--;
		return index;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �����Ϳ� ��ġ�ϴ� ������Ʈ�� �����ϴ��� �˻���, �ε����� ����
	//				�Ѵ�.
	//	��������:	���ø� ���� InsideType�� operator==�� ���ǵǾ� �־�� �Ѵ�.
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::find(const InsideType* const source) const
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



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� �迭�� ���� �迭���� Ȯ���Ѵ�.
	//	��������:	���� �迭�� �����Ǳ� ���ؼ��� 2���� ������ �ʿ��ϴ�.
	//					���� 1.	���� ũ��� ���̸� ������ �־�� ��.
	//					���� 2. ���Ұ� ���ƾ� ��.
	//					���� 3. ���ø� ���� InsideType�� operator==�� �������� ��.
	//	�޸�	:	
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType*, false, InsideType*, type>::isEqualSizeAndElement(const ThisClass& source) const
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
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType*, false, InsideType*, type>::isEqualElement(const ThisClass& source) const
	{
		if(getLengthLastIndex() != source.getLengthLastIndex()) return false;

		for(type_index index=0; index < _length ;index++)
			if(&getElement(index) != &source.getElement(index))
				return false;

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType*, false, InsideType*, type>::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = SuperClass::isValid();
		if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	void  NEArrayTemplate<InsideType*, false, InsideType*, type>::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/
		return SuperClass::release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEType::Type NEArrayTemplate<InsideType*, false, InsideType*, type>::getType() const
	{
		if(type == NEType::NEARRAY_TEMPLATE)
			return NEType::NEPOINTER_ARRAY_TEMPLATE;
		
		
		
		return type;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ũ�⸸ŭ�� �޸� ������ �Ҵ��Ѵ�.
	//				������ �����ϴ� �����ʹ� ��� �ʱ�ȭ�Ǿ �޸𸮿��� ���� �ȴ�.
	//	��������:
	//	�޸�	:	���� �����͸� ������ ä ũ�⸦ �����ϰ� �ʹٸ� resize�� ����϶�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType*, false, InsideType*, type>::create(type_count size)
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
			_data = new InsideType*[size](); // **����**	InsideType�� Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.		

			_size = size;
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	
	
	
	
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType*, false, InsideType*, type>&
		NEArrayTemplate<InsideType*, false, InsideType*, type>::_assign
		(
		const ThisClass& source
		)
	{	
		if(this == &source) return *this;

		create(source._size);
		push(source);
		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	void  NEArrayTemplate<InsideType*, false, InsideType*, type>::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;		
	}



	template <typename InsideType, NEType::Type type>
	NEBinaryFileLoader& NEArrayTemplate<InsideType*, false, InsideType*, type>::serialize(NEBinaryFileLoader& loader)
	{	/*
			Serialize�� ����:
				�������� �ݷ��ʹ� serialize�� ������ �� ����. �ִ��� ������ ��
				(���⼭�� �ݷ����� ũ�⸦ ���ϴ� ��)�� �����Ѵ�.
		*/
		//	pre:
		SuperClass::serialize(loader);



		//	main:
		//		���� ���� ����:
		type_index length = getLength();
		//		���� �ʱ�ȭ:
		create(getSize());



		//	post:
		//		���� ���� ����:
		_length = length;
		return loader;
	}



	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::push(const InsideType* const source)
	{
		return insert(getLength(), source);
	}

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::insert(type_index index, const InsideType& source)
	{
		return insert(index, &source);
	}

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::pop()
	{
		return remove(getLengthLastIndex());
	}

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::pushFront(const InsideType& source)
	{
		return insert(0, &source);
	}

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::pushFront(const InsideType* const source)
	{
		return insert(0, source);
	}

	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType*, false, InsideType*, type>::popFront()
	{
		return remove(0);
	}
}
