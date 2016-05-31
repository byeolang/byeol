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
//				�⺻������ NECollectorTemplate�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(InsideType*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(InsideType*)	:	�������� ���·� ������ �����ϴ�.
//					3. push(NEListTemplate&)	:	����Ʈ ���θ� ��°�� �����Ѵ�.
//					4. pushFront(NEListTemplate&)
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NEReservedCollectorTemplate/NEReservedCollectorTemplate.hpp"
#include "../../Interfaces/NESpecifiedInsertable/NESpecifiedInsertable.hpp"

namespace NE
{
	template <typename InsideType, NEType::Type type>
	class NEArrayTemplate<InsideType, false, InsideType, type> : public NEReservedCollectorTemplate<InsideType>, public NESpecifiedInsertable<InsideType>
	{	
		//	������:
	public:
		NEArrayTemplate();
		NEArrayTemplate(type_count size);
		NEArrayTemplate(const NEArrayTemplate<InsideType, false, InsideType, type>& source);

		//	�Ҹ���:
	public:
		virtual ~NEArrayTemplate();

		//	������ ��ø:
	public:
		const NEArrayTemplate<InsideType, false, InsideType, type>& operator=(const NEArrayTemplate<InsideType, false, InsideType, type>& source); 
		NEArrayTemplate<InsideType, false, InsideType, type> operator+(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const;
		bool operator==(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const;
		bool operator!=(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const;
		const NEArrayTemplate<InsideType, false, InsideType, type>& operator+=(const NEArrayTemplate<InsideType, false, InsideType, type>& source);

		//	�������̽�:
		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_index insert(type_index index, const InsideType& source);
		virtual type_index remove(type_index index);
		//			NEReservedCollectorTemplate:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NECollectorTemplate:
	public:
		virtual const InsideType& getElement(type_index index) const; 
		virtual InsideType& getElement(type_index index);
		virtual type_result setElement(type_index index, const InsideType& source);  
		virtual type_index push(const InsideType& source);
		using NECollectorTemplate::push;
		virtual type_index pop();
		virtual type_index find(const InsideType& source) const;
		//			NEObject:
	public:
		virtual NEObject& clone() const;
		virtual type_result isValid() const; 
		virtual void release(); 
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const; 
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEType::Type getType() const;

		//		���� �������̽�:
	public:
		type_index pushFront(const InsideType& source);
		type_index popFront();
		type_result push(const NEArrayTemplate<InsideType, false, InsideType, type>& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const NEArrayTemplate<InsideType, false, InsideType, type>& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		bool isEqualSizeAndElement(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const;
		bool isEqualElement(const NEArrayTemplate<InsideType, false, InsideType, type>& source) const;		

		//	�����Լ�:
	private:
		const NEArrayTemplate<InsideType, false, InsideType, type>& _assign(const NEArrayTemplate<InsideType, false, InsideType, type>& source);
		void _release();

		//	�������:
	protected:
		InsideType*	_data;
	};
}

#include "NEArrayWithSameTypedTemplate.inl"
#include "NEPointerArrayWithSameTypedTemplate.hpp"
#include "NEPointerArrayUsingPolyMorphismWithSameTypedTemplate.hpp"