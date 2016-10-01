#pragma once

#include "../NETCollector/NETCollector.hpp"
#include "NETReservedCollector.inl"

namespace NE
{
	NE_DEFINE_INTERFACE_ONLY(NETReservedCollector<OutsideType>, template <typename OutsideType>)

	template<typename OutsideType>
	NETReservedCollector<OutsideType>::NETReservedCollector()
		: Super() // Ŭ������ ��쿡�� int�� ����� �����ڸ� �����ؾ� ������ �𸥴�S
	{
		_release();
	}
	
	
	
	
	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	NETReservedCollector<OutsideType>::NETReservedCollector(type_index size)
		: Super()
	{
		_release();
		_size = size;
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	NETReservedCollector<OutsideType>::~NETReservedCollector() 
	{
		_release();
	}		



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	NETReservedCollector<OutsideType>::NETReservedCollector(const NETReservedCollector<OutsideType>& source)
		: Super(source)
	{
		_assign(source);
	}

	
	
	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	const NETReservedCollector<OutsideType>  &NETReservedCollector<OutsideType>::operator=(const NETReservedCollector& source) 
	{	
		Super::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	type_bool NETReservedCollector<OutsideType>::operator==(const NETReservedCollector<OutsideType>& source) const 
	{
		if(this == &source) return true;
		if(Super::operator!=(source)) return false;
		if(_size != source._size) return false;

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	type_bool NETReservedCollector<OutsideType>::operator!=(const NETReservedCollector<OutsideType>& source) const
	{
		return !(operator==(source));
	}


	
	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	������ �����Ѵٸ� ��������ڸ� �����Ѵ�. ������ ��������ڴ� �Ļ�Ŭ�������� ȣ���
	template<typename OutsideType>	
	type_index	 NETReservedCollector<OutsideType>::getSize() const 
	{
		return _size;
	}	



	
	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>	
	type_index	 NETReservedCollector<OutsideType>::getSizeLastIndex() const 
	{
		return _size-1;
	}
	


	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>	
	void  NETReservedCollector<OutsideType>::release()   
	{
		Super::release();

		return _release();
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>	
	type_result  NETReservedCollector<OutsideType>::isValid() const 
	{		
		//	���� üũ:	���ø�Ŭ�������� Kernal����� ���� �� �����Ƿ� ��ũ�θ�
		//				����� �� ����. (����� �������� �𸥴�)
		type_result result = Super::isValid();
		if(NEResult::hasError(result)) return result;
		if(_size < 0) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;
		if(_size < getLength()) return RESULT_TYPE_ERROR | RESULT_WRONG_BOUNDARY;

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------	
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	NEBinaryFileSaver &NETReservedCollector<OutsideType>::serialize(NEBinaryFileSaver& saver) const
	{
		NETCollector<OutsideType>::serialize(saver);

		return saver << _size;
	}



	//	---------------------------------------------------------------------------------	
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	NEBinaryFileLoader &NETReservedCollector<OutsideType>::serialize(NEBinaryFileLoader& loader)
	{
		NETCollector<OutsideType>::serialize(loader);

		return loader >> _size;
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	assign�� ���� �ܰ迡���� ��������� �Ҵ縸 ����Ѵ�.
	//	�Ž����ö󰡸鼭 �Լ��� ȣ���ϴ� �� ��������ڿ� operator=���� ����Ѵ�.
	//	��������� �ִ� -> assign ����
	//	�ν��Ͻ� �������� -> ���������, operator=���� assign ȣ��
	template<typename OutsideType>
	const NETReservedCollector<OutsideType>& NETReservedCollector<OutsideType>::_assign(const NETReservedCollector<OutsideType>& source) // �̰� ���� ���� �Լ�. ���̻� ���Ŭ�����Լ��� ȣ������ �ʴ´�
	{			
		if(this == &source) return *this;

		_size = source._size;
		
		return *this; 
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	template<typename OutsideType>
	void NETReservedCollector<OutsideType>::_release()
	{
		_size = 0;
	}
}