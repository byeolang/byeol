//	---------------------------------------------------------------------------------
//	Ŭ������:	NEArrayTemplate
//	����	:	Array�� ���·� �����͸� �����ϴ� Ŭ����.
//	����	:	���Ŭ����.			NEReservedCollectorTemplate
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
//				�⺻������ NEReservedCollectorTemplate�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(InsideType*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(InsideType*)	:	�������� ���·� ������ �����ϴ�.
//					3. push(NEListTemplate&)	:	����Ʈ ���θ� ��°�� �����Ѵ�.
//					4. pushFront(NEListTemplate&)
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

namespace NE
{
	template <typename InsideType, NEType::Type type>
	class NEArrayTemplate<InsideType*, false, InsideType*, type> : public NEReservedCollectorTemplate<InsideType*>, public NESpecifiedInsertable<InsideType*>
	{
		typedef NEReservedCollectorTemplate<InsideType*> SuperClass;
		typedef NEArrayTemplate<InsideType*, false, InsideType*, type> ThisClass;

	public:
		using SuperClass::push;
		using SuperClass::find;
		using SuperClass::setElement;

		//	������:
	public:
		NEArrayTemplate();
		NEArrayTemplate(type_count size);
		NEArrayTemplate(const ThisClass& source);

		//	�Ҹ���:
	public:
		virtual ~NEArrayTemplate();

		//	������ ��ø:
	public:
		const ThisClass& operator=(const ThisClass& source);
		bool operator==(const ThisClass& source) const;
		bool operator!=(const ThisClass& source) const;
		ThisClass operator+(const ThisClass& source) const;
		const ThisClass& operator+=(const ThisClass& source);

		//	�������̽�:
		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_index insert(type_index index, const InsideType* const source);
		virtual type_index remove(type_index index);
		//			NEReservedCollectorTemplate:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NECollectorTemplate:
	public:
		virtual const InsideType& getElement(type_index index) const; 
		virtual InsideType& getElement(type_index index);
		virtual type_result setElement(type_index index, const InsideType* const source);		
		virtual type_index push(const InsideType* const source);
		using NECollectorTemplate::push;
		virtual type_index pop();
		virtual type_index find(const InsideType* const source) const;
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual void release(); 
		virtual NEType::Type getType() const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEObject& clone() const;

		//		���� �������̽�:
	public:
		type_index insert(type_index index, const InsideType& source);
		type_index popFront();
		type_index pushFront(const InsideType* const source);
		type_index pushFront(const InsideType& source);
		type_result push(const ThisClass& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const ThisClass& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		bool isEqualSizeAndElement(const ThisClass& source) const;
		bool isEqualElement(const ThisClass& source) const;		

		//	�����Լ�:
	private:
		const ThisClass& _assign(const ThisClass& source);
		void _release();

		//	�������:
	protected:
		InsideType**	_data;
	};
}

#include "NEPointerArrayWithSameTypedTemplate.inl"
