//	---------------------------------------------------------------------------------
//	Ŭ������:	NETArray
//	����	:	Array�� ���·� �����͸� �����ϴ� Ŭ����.
//	����	:	���Ŭ����.			NETReservedCollector
//	Ư��	:	�迭�� Ư���� �״�� �����Ѵ�. �� ������Ʈ�� �߰�/����/������
//				���ϰ� ū �ݸ�, "�˻�"�� ���ϴ�.//				
//	�˰���:	���� �����͸� �������� ���, �ش� �����ͷκ��� ���󺹻�������� clone
//				�� ȣ���ؼ� DeepCopy �Ѵ�.
//	�����:	���� ���
//					:	���ø� ���� Type�� ����
//							���� 1.	NEObject�� ����ؾ��Ѵ�. 
//									NEObject::clone()�� ����ϱ� �����̴�.
//							���� 2. 
//				type_bool pointerUseNewInstance��?
//					:	false�϶� Type�� �������� ���, �����Ͱ� ���ο� �ν��Ͻ��� ����Ű����
//						�����ʰ�, shallowcopy���� ����ϰڴٴ� �ǹ̴�.
//						true�϶��� NEPointerList.hpp�� �����϶�.
//				�⺻������ NETCollector�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(Type*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(Type*)	:	�������� ���·� ������ �����ϴ�.
//					3. push(NETList&)	:	����Ʈ ���θ� ��°�� �����Ѵ�.
//					4. pushFront(NETList&)
/*
				����(Outside)�� ����(Inside) Ÿ���� ���� �ٸ� Collector ���� ���:
					1. Ÿ���� �ٸ� ���, NETArray�� �߻�Ŭ�����μ� ����ȴ�.
					2. ������ �Լ����� �Ļ�Ŭ�������� ä�����Ѵ�.
						1. ���������
						2. virtual release
						3. virtual insert
						3. virtual remove
						4. virtual getElement(index) const
						5. virtual getElement(index)
						6. virtual setElement
						7. virtual clone
						8. virtual find
*/
//	�޸�	:	
//	�����丮:	2013-01-10	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NEOccupiedSet/NEOccupiedSet.hpp"
#include "../NETReservedCollector/NETReservedCollector.inl"

namespace NE
{
	template <typename Type, type_bool pointerUseNewInstance = false>
	class NETArray : public NETReservedCollector<Type>
	{	
		typedef NETArray<Type, pointerUseNewInstance> _This;

		NE_DECLARE_CLASS(_This, NETReservedCollector<Type>)

		//	������:
	public:
		NETArray();
		NETArray(type_count size);
		NETArray(const This& source);

		//	�Ҹ���:
	public:
		virtual ~NETArray();

		//	������ ��ø:
	public:
		const This& operator=(const This& source); 
		This operator+(const This& source) const;
		type_bool operator==(const This& source) const;
		type_bool operator!=(const This& source) const;
		const This& operator+=(const This& source);

		//	�������̽�:
		//		������:
	public:
		const NEOccupiedSet& getOccupiedSet() const;

		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_index insert(type_index index, const Type& source);
		virtual type_index remove(type_index index);
		//			NETReservedCollector:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NETCollector:
	public:		
		virtual Type& getElement(type_index index);
		virtual const Type& getElement(type_index index) const;
		virtual type_result setElement(type_index index, const Type& source);
		virtual type_index push(const Type& source);
		virtual type_index pop();		
		virtual type_index match(const Type& source) const;
		//			NEObject:
	public:
		virtual type_result isValid() const;
		virtual void release();
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		
		//		���� �������̽�:
	public:
		type_index pushFront(const Type& source);
		type_index popFront();
		type_result push(const This& source);
		type_result pushFront(const This& source);
		type_bool isEqualSizeAndElement(const This& source) const;
		type_bool isEqualElement(const This& source) const;

	protected:
		type_index _searchIndex(type_bool by_decreasing, type_bool target_is_occupied) const;

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		void _release();

		//	�������:
	protected:
		Type*	_data;
		NEOccupiedSet _occupiedset;
	};
}

#include "NETPointerIndexedArray.inl"
#include "NETPointerIndexedArrayUsingPolyMorphism.inl"