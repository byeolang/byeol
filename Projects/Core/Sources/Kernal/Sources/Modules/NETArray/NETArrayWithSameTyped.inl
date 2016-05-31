namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType, false, InsideType, type>::NEArrayTemplate()
		: NEReservedCollectorTemplate<InsideType>(), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType, false, InsideType, type>::NEArrayTemplate(type_count size)
		: NEReservedCollectorTemplate<InsideType>(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType, false, InsideType, type>::NEArrayTemplate(const NEArrayTemplate<InsideType, false, InsideType, type>& source)
		: NEReservedCollectorTemplate<InsideType>(), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType, false, InsideType, type>::~NEArrayTemplate()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType, false, InsideType, type>&
		NEArrayTemplate<InsideType, false, InsideType, type>::operator=
		(
		const NEArrayTemplate<InsideType, false, InsideType, type>& source
		)
	{
		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEArrayTemplate<InsideType, false, InsideType, type>  
		NEArrayTemplate<InsideType, false, InsideType, type>::operator+
		(
		const NEArrayTemplate<InsideType, false, InsideType, type>& source
		) const
	{
		//	pre:
		//		�Ķ���� �˻�:
		if(source._length <= 0)	return NEArrayTemplate<InsideType, false, InsideType, type>(*this);



		//	main:
		NEArrayTemplate<InsideType, false, InsideType, type> buffer(source._length + _length);
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
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------	
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType, false, InsideType, type>::operator==(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const
	{
		if(this == &source) return true;
		if(NEReservedCollectorTemplate<InsideType>::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	bool  NEArrayTemplate<InsideType, false, InsideType, type>::operator!=(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType, false, InsideType, type>&
		NEArrayTemplate<InsideType, false, InsideType, type>::operator+=
		(
		const NEArrayTemplate<InsideType, false, InsideType, type>& source
		)
	{
		if(this == &source)
		{
			NEArrayTemplate<InsideType, false, InsideType, type> copyied(*this);
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
	template <typename InsideType, NEType::Type type>
	InsideType  &NEArrayTemplate<InsideType, false, InsideType, type>::getElement(type_index index) 
	{	
		InsideType* null_pointer = NE_NULL;
		//	pre:
		//		�Ķ���� �˻�:
		if(index < 0) return *null_pointer;
		if(index > getLengthLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;



		//	post:
		return *(_data + index);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const InsideType  &NEArrayTemplate<InsideType, false, InsideType, type>::getElement(type_index index) const 
	{	
		InsideType* null_pointer = NE_NULL;
		//	pre:
		//		�Ķ���Ͱ˻�: 
		if(index < 0) return *null_pointer;
		if(index > getLengthLastIndex()) return *null_pointer;
		//		���ø������ �˻�:
		if(_size <= 0) return *null_pointer;

 

		//	post:
		return *(_data + index);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType, false, InsideType, type>::setElement(type_index index, const InsideType& source)  
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return RESULT_TYPE_WARNING;
		if(index > getLengthLastIndex()) return RESULT_TYPE_WARNING; // setElement�� �߰��� �������� ������ �����ϴ�. ����, ������ �߰��� ���Ѵٸ� push�� ����϶�.
		//		���ø������ �˻�:
		if(_size <= 0) return RESULT_TYPE_WARNING; 

		*(_data + index) = source;
		
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
	type_result NEArrayTemplate<InsideType, false, InsideType, type>::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		NEArrayTemplate<InsideType, false, InsideType, type> temp = *this; // use operator=
		create(size); // size�� ���ο��� ���õ�.
		push(temp); // temp �ڵ� �Ҹ�



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	 �迭 ��ü�� push�Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result  NEArrayTemplate<InsideType, false, InsideType, type>::push(const NEArrayTemplate<InsideType, false, InsideType, type>& source)
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
			insert(_length, source.getElement(index));	
		}



		//	post:
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	type_result NEArrayTemplate<InsideType, false, InsideType, type>::pushFront(const NEArrayTemplate<InsideType, false, InsideType, type>& source)
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

			insert(index, source.getElement(index));
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
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::insert(type_index index, const InsideType& source) 
	{
		//	pre:
		//		�Ķ���Ͱ˻�:
		if(index < 0) return NE_INDEX_ERROR;
		if( ! &source) return NE_INDEX_ERROR;
		if(index > _length) return NE_INDEX_ERROR;
		//		���ø������ �˻�:
		if(_length >= _size) resize(_size * 2 + 1);



		//	main:
		//		���Ե� ��������:	�����͸� ��ĭ�� ������ �ű��
		for(type_index n = getLengthLastIndex(); n >= index ;n--)
			_data[n + 1] = _data[n];



		//	post:
		//		�����ͻ���:
		_data[index] = source;
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
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::remove(type_index index) 
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
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::find(const InsideType& source) const
	{
		//	pre:
		//		���ø������ �˻�:
		if(_length <= 0) return NE_INDEX_ERROR;



		//	main:
		//		�˻�:
		for(type_index index=0; index < _length ;index++)
			if(_data[index] == source) 
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
	bool  NEArrayTemplate<InsideType, false, InsideType, type>::isEqualSizeAndElement(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const
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
	bool  NEArrayTemplate<InsideType, false, InsideType, type>::isEqualElement(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const
	{
		if(getLengthLastIndex() != source.getLengthLastIndex()) return false;

		for(type_index index=0; index < _length ;index++)
		{				
			if(getElement(index) != source.getElement(index))
				return false;
		}

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------	
	template <typename InsideType, NEType::Type type>
	NEObject& NEArrayTemplate<InsideType, false, InsideType, type>::clone() const 
	{			
		return *(new NEArrayTemplate<InsideType, false, InsideType, type>(*this));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type> 
	type_result NEArrayTemplate<InsideType, false, InsideType, type>::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = NEReservedCollectorTemplate<InsideType>::isValid();
		//if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	void  NEArrayTemplate<InsideType, false, InsideType, type>::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/

		return NEReservedCollectorTemplate<InsideType>::release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEBinaryFileSaver  &NEArrayTemplate<InsideType, false, InsideType, type>::serialize(NEBinaryFileSaver& saver) const 
	{			
		//	pre:
		NEReservedCollectorTemplate::serialize(saver);		



		//	main:
		//		���� ������ ����:
		for(type_index n=0; n < _length ;n++)
			saver << _data[n];



		//	post:
		return saver;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEBinaryFileLoader& NEArrayTemplate<InsideType, false, InsideType, type>::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		NEArrayTemplate<InsideType, false, InsideType, type>::release();
		NEReservedCollectorTemplate::serialize(loader);		



		//	main:
		//		�޸� ���۰��� ����:
		int length = _length;
		create(_size);
		_length = 0;
		//		������ �ε�:
		for(type_index n=0; n < length ;n++)
		{
			InsideType buffer;
			loader >> buffer;
			push(buffer);
		}



		//	post:
		return loader;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	NEType::Type NEArrayTemplate<InsideType, false, InsideType, type>::getType() const
	{
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
	type_result NEArrayTemplate<InsideType, false, InsideType, type>::create(type_count size)
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
			_data = new InsideType[size](); // **����**	InsideType�� Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.		

			_size = size;
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template <typename InsideType, NEType::Type type>
	const NEArrayTemplate<InsideType, false, InsideType, type>&
		NEArrayTemplate<InsideType, false, InsideType, type>::_assign
		(
		const NEArrayTemplate<InsideType, false, InsideType, type>& source
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
	void  NEArrayTemplate<InsideType, false, InsideType, type>::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;		
	}



	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::push(const InsideType& source)
	{
		return insert(getLength(), source);
	}



	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::pop()
	{
		return remove(getLengthLastIndex());
	}


	
	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::pushFront(const InsideType& source)
	{
		return insert(0, source);
	}



	template <typename InsideType, NEType::Type type>
	type_index NEArrayTemplate<InsideType, false, InsideType, type>::popFront()
	{
		return remove(0);
	}
}