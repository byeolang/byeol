//	---------------------------------------------------------------------------------
//	Ŭ������:	NETArray
//	����	:	Array�� ���·� �����͸� �����ϴ� Ŭ����.
//	����	:	���Ŭ����.			NETReservedCollector
//	Ư��	:	�迭�� Ư���� �״�� �����Ѵ�. �� ������Ʈ�� �߰�/����/������
//				���ϰ� ū �ݸ�, "�˻�"�� ���ϴ�.//				
//	�˰���:	���� �����͸� �������� ���, �ش� �����ͷκ��� ���󺹻�������� clone
//				�� ȣ���ؼ� DeepCopy �Ѵ�.
//	�����:	���� ���
//					:	���ø� ���� InsideType�� ����
//							���� 1.	NEObject�� ����ؾ��Ѵ�. 
//									NEObject::clone()�� ����ϱ� �����̴�.
//							���� 2. 
//				bool pointerUseNewInstance��?
//					:	false�϶� InsideType�� �������� ���, �����Ͱ� ���ο� �ν��Ͻ��� ����Ű����
//						�����ʰ�, shallowcopy���� ����ϰڴٴ� �ǹ̴�.
//						true�϶��� NEPointerList.hpp�� �����϶�.
//				�⺻������ NETReservedCollector�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(InsideType*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(InsideType*)	:	�������� ���·� ������ �����ϴ�.
//					3. push(NETList&)	:	����Ʈ ���θ� ��°�� �����Ѵ�.
//					4. pushFront(NETList&)
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

namespace NE
{
	template <typename InsideType>
	class NETArray<InsideType*, false, InsideType*> : public NETReservedCollector<InsideType*>, public NESpecifiedInsertable<InsideType*>
	{
		typedef NETArray<InsideType*, false, InsideType*> _This;

		NE_DECLARE_CLASS(_This, NETReservedCollector<InsideType*>)
	public:
		using Super::push;
		using Super::find;
		using Super::setElement;

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
		bool operator==(const This& source) const;
		bool operator!=(const This& source) const;
		This operator+(const This& source) const;
		const This& operator+=(const This& source);

		//	�������̽�:
		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_index insert(type_index index, const InsideType* const source);
		virtual type_index remove(type_index index);
		//			NETReservedCollector:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NETCollector:
	public:
		virtual const InsideType& getElement(type_index index) const; 
		virtual InsideType& getElement(type_index index);
		virtual type_result setElement(type_index index, const InsideType* const source);		
		virtual type_index push(const InsideType* const source);
		using NETCollector::push;
		virtual type_index pop();
		virtual type_index find(const InsideType* const source) const;
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual void release(); 
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//		���� �������̽�:
	public:
		type_index insert(type_index index, const InsideType& source);
		type_index popFront();
		type_index pushFront(const InsideType* const source);
		type_index pushFront(const InsideType& source);
		type_result push(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		bool isEqualSizeAndElement(const This& source) const;
		bool isEqualElement(const This& source) const;		

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		void _release();

		//	�������:
	protected:
		InsideType**	_data;
	};
}

#include "NETPointerArrayWithSameTyped.inl"
