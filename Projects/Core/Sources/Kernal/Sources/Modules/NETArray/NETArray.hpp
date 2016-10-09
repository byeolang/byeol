#pragma once

#include "../NETReservedCollector/NETReservedCollector.hpp"
#include "NETArray.inl"

namespace NE
{
#define _TEMPLATE	template <typename T>
#define _OWNERNAME	NETArray<T>
#define _NAME		_OWNERNAME##::Iterator

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	_TEMPLATE
	_NAME::Iterator(_This& owner, type_index new_index)
		: Super(), _owner(owner), _index(new_index)
	{

	}			

	_TEMPLATE
	type_bool _NAME::operator==(const This& rhs) const
	{
		//	When these binders are designating same object each other,
		//	we'll return true.
		return	_owner.get() == rhs._owner.get()	&&
				_index == rhs._index				;

	}

	_TEMPLATE
	type_bool _NAME::operator<(const This& rhs) const
	{
		return	_index < rhs._index;
	}

	_TEMPLATE
	type_bool _NAME::operator>(const This& rhs) const
	{
		return	_index > rhs._index;
	}

	_TEMPLATE
	const T& _NAME::get() const
	{
		const T* nullpointer = NE_NULL;
		if( ! _owner.isBinded())
			return *nullpointer;		

		return _owner->getElement(_index);
	}

	_TEMPLATE
	T& _NAME::get()
	{
		const T* nullpointer = NE_NULL;
		if( ! _owner.isBinded())
			return *nullpointer;		

		return _owner->getElement(_index);
	}

	_TEMPLATE
	type_bool _NAME::isEnd() const
	{
		if( ! _owner.isBinded())
			//	Oops. Now, we got trouble.
			return true;

		return	_index >= _owner->getLength()	||
				_index < 0						;
	}

	_TEMPLATE
	type_result _NAME::next(type_index step_for_next)
	{
		if(NEResult::hasError(isValid()))	goto FAILED;

		Node* nodes = _owner->_data;

		for(; step_for_next > 0 ;step_for_next--)
		{
			do
			{
				_index++;
				//	Left Nodes:	Node isn't element.
				type_count left_nodes = getSize() - _index;
				if(	step_for_next > left_nodes	||
					isEnd()						)
				{
					_index = getLength();
					return RESULT_ABORT_ACTION | RESULT_TYPE_WARNING;
				}
			} while( ! nodes[_index].is_occupied);
		}
		
		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_result _NAME::back(type_index step_for_back)
	{
		if(NEResult::hasError(isValid()))	goto FAILED;

		Node* nodes = _owner->_data;

		for(; step_for_next > 0 ;step_for_next--)
		{
			do
			{
				_index--;
				//	Left Nodes:	Node isn't element.
				type_count left_nodes = _index+1;
				if(	step_for_next > left_nodes	||
					isEnd()						)
				{
					_index = NE_INDEX_ERROR;	//	this means "StartIterater".
					return RESULT_ABORT_ACTION | RESULT_TYPE_WARNING;
				}
			} while( ! nodes[_index].is_occupied);
		}

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_index _NAME::getLength() const
	{
		return _owner.isBinded() ? _owner->getLength() : 0;
	}

	_TEMPLATE
	void _NAME::release()
	{
		_owner.release();
		_index = NE_INDEX_ERROR;
	}

	_TEMPLATE
	NEBinaryFileSaver& _NAME::serialize(NEBinaryFileSaver& saver) const
	{
		return saver << _owner << _index;
	}

	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _owner >> _index;
	}

	_TEMPLATE
	type_result _NAME::isValid() const
	{
		if( ! _owner.isBinded()) return RESULT_TYPE_ERROR;
		if( isEnd()) return RESULT_TYPE_WARNING;		

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	_OWNERNAME& _NAME::getOwner() const
	{
		return _owner;
	}

	_TEMPLATE
	const _OWNERNAME& _NAME::getOwner() const
	{
		return _owner;
	}

	_TEMPLATE
	type_result _NAME::setIndex(type_index new_index) const
	{
		_node = new_index;

		return isValid();
	}

	_TEMPLATE
	type_index _NAME::getIndex() const
	{
		return _index;
	}

#undef _NAME	
#define _NAME	_OWNERNAME##::CIterator

	NE_DEFINE_CLASS_ONLY_2(NE_MACRO_FUNNEL(_NAME), NE_MACRO_FUNNEL(_TEMPLATE))

	_TEMPLATE
	_NAME::CIterator(const _This& owner, type_index new_index)
		: Super(), _owner(owner), _index(new_index)
	{

	}			

	_TEMPLATE
	type_bool _NAME::operator==(const This& rhs) const
	{
		//	When these binders are designating same object each other,
		//	we'll return true.
		return	_owner.get() == rhs._owner.get()	&&
				_index == rhs._index				;

	}

	_TEMPLATE
	type_bool _NAME::operator<(const This& rhs) const
	{
		return	_index < rhs._index;
	}

	_TEMPLATE
	type_bool _NAME::operator>(const This& rhs) const
	{
		return	_index > rhs._index;
	}

	_TEMPLATE
	const T& _NAME::get() const
	{
		const T* nullpointer = NE_NULL;
		if( ! _owner.isBinded())
			return *nullpointer;		

		return _owner->getElement(_index);
	}

	_TEMPLATE
	type_bool _NAME::isEnd() const
	{
		if( ! _owner.isBinded())
			//	Oops. Now, we got trouble.
			return true;

		return	_index >= _owner->getLength()	||
			_index < 0						;
	}

	_TEMPLATE
	type_result _NAME::next(type_index step_for_next)
	{
		if(NEResult::hasError(isValid()))	goto FAILED;

		Node* nodes = _owner->_data;

		for(; step_for_next > 0 ;step_for_next--)
		{
			do
			{
				_index++;
				//	Left Nodes:	Node isn't element.
				type_count left_nodes = getSize() - _index;
				if(	step_for_next > left_nodes	||
					isEnd()						)
				{
					_index = getLength();
					return RESULT_ABORT_ACTION | RESULT_TYPE_WARNING;
				}
			} while( ! nodes[_index].is_occupied);
		}

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_result _NAME::back(type_index step_for_back)
	{
		if(NEResult::hasError(isValid()))	goto FAILED;

		Node* nodes = _owner->_data;

		for(; step_for_next > 0 ;step_for_next--)
		{
			do
			{
				_index--;
				//	Left Nodes:	Node isn't element.
				type_count left_nodes = _index+1;
				if(	step_for_next > left_nodes	||
					isEnd()						)
				{
					_index = NE_INDEX_ERROR;	//	this means "StartIterater".
					return RESULT_ABORT_ACTION | RESULT_TYPE_WARNING;
				}
			} while( ! nodes[_index].is_occupied);
		}

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	type_index _NAME::getLength() const
	{
		return _owner.isBinded() ? _owner->getLength() : 0;
	}

	_TEMPLATE
	void _NAME::release()
	{
		_owner.release();
		_index = NE_INDEX_ERROR;
	}

	_TEMPLATE
	NEBinaryFileSaver& _NAME::serialize(NEBinaryFileSaver& saver) const
	{
		return saver << _owner << _index;
	}

	_TEMPLATE
	NEBinaryFileLoader& _NAME::serialize(NEBinaryFileLoader& loader)
	{
		return loader >> _owner >> _index;
	}

	_TEMPLATE
	type_result _NAME::isValid() const
	{
		if( ! _owner.isBinded()) return RESULT_TYPE_ERROR;
		if( isEnd()) return RESULT_TYPE_WARNING;		

		return RESULT_SUCCESS;
	}

	_TEMPLATE
	const _OWNERNAME& _NAME::getOwner() const
	{
		return _owner;
	}

	_TEMPLATE
	type_result _NAME::setIndex(type_index new_index) const
	{
		_node = new_index;

		return isValid();
	}

	_TEMPLATE
	type_index _NAME::getIndex() const
	{
		return _index;
	}

#undef _NAME

	NE_DEFINE_INTERFACE_ONLY_2(_OWNERNAME, _TEMPLATE)

	_TEMPLATE
	_OWNERNAME::NETArray()
		: Super(), _data(NE_NULL)
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_OWNERNAME::NETArray(type_count size)
		: Super(size), _data(NE_NULL)
	{
		create(size);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_OWNERNAME::NETArray(const This& source)
		: Super(), _data(NE_NULL)
	{
		_assign(source);
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	_OWNERNAME::~NETArray()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _OWNERNAME& _OWNERNAME::operator=(const This& source)
	{
		return _assign(source);
	}

	//	---------------------------------------------------------------------------------
	//	����	:	���Ҹ� ������ true�ΰ���, ��������� ���ƾ� �ϴ����� ���� ��ȣ���� 
	//				�ֱ⶧���� �״��� ��õ���� �ʴ´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------	
	_TEMPLATE
	type_bool _OWNERNAME::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(Super::operator==(source) == false) return false;

		return isEqualElement(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_bool _OWNERNAME::operator!=(const This& source) const
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//				2011-11-20	������	���� ����	source�� *this�� �� �߻��ϰԵ� ���� ��ó �߰�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _OWNERNAME& _OWNERNAME::operator+=(const This& source)
	{
		if(this == &source)
		{
			This copyied(*this);
			push(copyied);
		}
		else
			push(source);

		return *this;
	}

	_TEMPLATE
	_OWNERNAME _OWNERNAME::operator+(const This& source) const
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
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	T& _OWNERNAME::getElement(type_index index) 
	{	
		//	pre:
		//		�� ���۷���:
		T* null_pointer = NE_NULL;
		//		�Ķ���� �˻�:
		if(index < 0) return *null_pointer;
		if(index > getSizeLastIndex()) return *null_pointer;

		//	post:
		Node& node = _data[index];
		return ! node.is_occupied ? *null_pointer : node.datum;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const T& _OWNERNAME::getElement(type_index index) const 
	{	
		//	pre:
		//		�� ���۷���:
		const T* null_pointer = NE_NULL;
		//		�Ķ���Ͱ˻�: 
		if(index < 0) return *null_pointer;
		if(index > getSizeLastIndex()) return *null_pointer;

		//	post:
		const Node& node = _data[index];
		return ! node.is_occupied ? *null_pointer : node.datum;
	}

	_TEMPLATE
	NETBinder<NETIterator<T>> _OWNERNAME::insert(const NETCIterator<T>& where, const T& source)
	{
		//	pre:
		NETBinder<NETIterator<T>> to_return;
		CIterator& wh = cast<CIterator>(where);
		//		parameter checking:
		if( ! _isValid(wh)	||
			! &source		) 
			return to_return;
		//		current state checking:
		if(_length >= _size) return to_return;		
		

		//	main:
		//		packaging return:
		type_index index = wh.getIndex();
		Iterator& e = *(new Iterator(*this, index));
		to_return.bind(e);
		//		replacement checking:
		//			if there is element placed already, we'll find another vacant.
		//			in this precedure, we may reach the end of inner array, _data.
		//			Then, we keep searching once again from the front of it.
		if(*e)
		{	
			type_bool once_arounded = false;
			while((++e).isEnd())
			{
				if(once_arounded++)
					return to_return;
				
				e.setIndex(0);
			}
			//		Founded:	iterator e has been updated.
		}
		//		proclaim occupying:
		//			Why do we do this first?:
		//				Sometimes it happen to call NETArray::insert() again by source->clone()
		//				while on inserting this.
		//				So, if we don't occupy place first, eventually the later cloned instance
		//				tries to be inserted at same index like the former.
		//				And for sure, this cracks the job.
		Node& node = *e;
		node.is_occupied = true;		
		node.datum = source;
		//		���� ����:
		_length++;

		
		//	post:
		return to_return;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ �ε����� ��ġ�� �����͸� �ϳ� �����Ѵ�.
	//	��������:	������ �ε����� �ִ� �ε������� �۰�, 0 �̻��̴�.
	//	�޸�	:
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_index _OWNERNAME::remove(type_index index) 
	{
		//	pre:
		NETBinder<NETIterator<T>> to_return;
		CIterator& wh = cast<CIterator>(where);
		//		parameter checking:
		if( ! _isValid(wh)	||
			! &source		) 
			return to_return;
		//		current state checking:
		if(_length >= _size) return to_return;		
		type_index index = wh.getIndex();
		if( ! _data[index].is_occupied)
			return to_return;

		
		//	main:
		//		return occupied:
		//			We don't need to set 0 what we'll return.
		_data[index].is_occupied = false;


		//	post:
		_length--;
		to_return.bind( *(new Iterator(*this, index)) );
		return to_return;
	}

	_TEMPLATE 
	type_result _OWNERNAME::isValid() const  
	{
		//	���� Ŭ������ valid üũ:
		type_result result = Super::isValid();		
		if(NEResult::hasError(result) == true) return result;
		//	��������� valid üũ:
		if(	_size > 0 && ! _data) return RESULT_TYPE_WARNING | RESULT_OUT_OF_MEMORY;
		if( _size < _length) return RESULT_TYPE_WARNING;
		//	checks realdata and length:
		type_count n=0;
		each([&n](const T&){ 
			n++;
			return RESULT_SUCCESS;
		});
		if(n != _length) return RESULT_TYPE_WARNING;

		return RESULT_SUCCESS;
	}			



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	void  _OWNERNAME::release()  
	{		
		_release();
		/*
			�ǵ����� ���� ó��:
				���⿡���� _release�� ����Ŭ������ release�� ȣ���ϴ� ������ �ڹٲ��� �Ѵ�.
		*/

		return Super::release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	NEBinaryFileSaver  &_OWNERNAME::serialize(NEBinaryFileSaver& saver) const 
	{			
		//	pre:
		NETReservedCollector::serialize(saver);		

		for(type_index n=0; n < getSize() ;n++)
		{
			Node& node = _data[n];
			saver << node.is_occupied;
			
			if(node.is_occupied)
				saver << node.datum;
		}

		return saver;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	NEBinaryFileLoader& _OWNERNAME::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		This::release();
		NETReservedCollector::serialize(loader);		

		//	main:
		create(_size);

		for(type_index n=0; n < getSize() ;n++)
		{
			Node& node = _data[n];
			loader >> node.is_occupied;

			if(node.is_occupied)
				loader >> node.datum;
		}

		return loader;
	}

	_TEMPLATE
	type_result _OWNERNAME::resize(type_count size) // �������ִ� �����ʹ� ������ä ũ�⸸ �����Ѵ�.
	{
		//	pre:
		if(size < 0) return RESULT_TYPE_INFORMATION | RESULT_ABORT_ACTION;



		//	main:
		This copied(*this); // use operator=
		create(size); // size�� ���ο��� ���õ�.
		push(copied);

		//	post:
		return RESULT_SUCCESS;
	}


	//	---------------------------------------------------------------------------------
	//	����	:	������ ũ�⸸ŭ�� �޸� ������ �Ҵ��Ѵ�.
	//				������ �����ϴ� �����ʹ� ��� �ʱ�ȭ�Ǿ �޸𸮿��� ���� �ȴ�.
	//	��������:
	//	�޸�	:	���� �����͸� ������ ä ũ�⸦ �����ϰ� �ʹٸ� resize�� ����϶�.
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	type_result _OWNERNAME::create(type_count size)
	{
		release();

		if(size > 0)
		{
			//	����, T�� primitive�ϰ�쿡, 0���� ������ �ؾ��Ѵ�. �̰� �迭�� ǥ���ϸ�
			//		new T[size] = {0, }; 
			//	�� �ȴٴ� �� �˰��ִ�.
			//	�׷��� T�� Ŭ�����ϰ�쿡��, ���� �ڵ忡�� int �����ڰ� ȣ��ȴ�. ��, �������� int�����ڸ� ������ ������
			//	Ŭ������ �� ���ø�Ŭ������ ���� �� ����.
			//	�׷���, new int(); �� 0���� �ʱ�ȭ�� �ȴٴ� ��ǿ��� ����,
			//	���� �ڵ��, T�� Ŭ�����̰�, primitive�̰� ���� �����ڸ� ȣ���ϰ� ������ִ� �� �ϳ��� �ڵ��.			
			_data = new T[_size = size](); // **����**	T�� Ŭ������ ���, �⺻������()�� �ʿ��ϴ�.
			_length = 0;
		}

		return RESULT_SUCCESS;
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	const _OWNERNAME& _OWNERNAME::_assign(const This& source)
	{	
		//	pre:
		if(this == &source) return *this;

		create(source._size);
		push(source);

		return *this;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2013-01-10	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	_TEMPLATE
	void  _OWNERNAME::_release() 
	{	
		if(_data)
			delete [] _data;
		_data = NE_NULL;
	}
}