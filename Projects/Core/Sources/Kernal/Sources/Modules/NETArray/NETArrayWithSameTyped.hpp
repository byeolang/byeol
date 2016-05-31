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
//				�⺻������ NETCollector�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(InsideType*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(InsideType*)	:	�������� ���·� ������ �����ϴ�.
//					3. push(NETList&)	:	����Ʈ ���θ� ��°�� �����Ѵ�.
//					4. pushFront(NETList&)
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NETReservedCollector/NETReservedCollector.hpp"
#include "../../Commons/Interfaces/NESpecifiedInsertable/NESpecifiedInsertable.hpp"

namespace NE
{
	template <typename InsideType, NEType::Type type>
	class NETArray<InsideType, false, InsideType, type> : public NETReservedCollector<InsideType>, public NESpecifiedInsertable<InsideType>
	{	
		//	������:
	public:
		NETArray();
		NETArray(type_count size);
		NETArray(const NETArray<InsideType, false, InsideType, type>& source);

		//	�Ҹ���:
	public:
		virtual ~NETArray();

		//	������ ��ø:
	public:
		const NETArray<InsideType, false, InsideType, type>& operator=(const NETArray<InsideType, false, InsideType, type>& source); 
		NETArray<InsideType, false, InsideType, type> operator+(const NETArray<InsideType, false, InsideType, type>& source) const;
		bool operator==(const NETArray<InsideType, false, InsideType, type>& source) const;
		bool operator!=(const NETArray<InsideType, false, InsideType, type>& source) const;
		const NETArray<InsideType, false, InsideType, type>& operator+=(const NETArray<InsideType, false, InsideType, type>& source);

		//	�������̽�:
		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_index insert(type_index index, const InsideType& source);
		virtual type_index remove(type_index index);
		//			NETReservedCollector:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NETCollector:
	public:
		virtual const InsideType& getElement(type_index index) const; 
		virtual InsideType& getElement(type_index index);
		virtual type_result setElement(type_index index, const InsideType& source);  
		virtual type_index push(const InsideType& source);
		using NETCollector::push;
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
		type_result push(const NETArray<InsideType, false, InsideType, type>& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const NETArray<InsideType, false, InsideType, type>& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		bool isEqualSizeAndElement(const NETArray<InsideType, false, InsideType, type>& source) const;
		bool isEqualElement(const NETArray<InsideType, false, InsideType, type>& source) const;		

		//	�����Լ�:
	private:
		const NETArray<InsideType, false, InsideType, type>& _assign(const NETArray<InsideType, false, InsideType, type>& source);
		void _release();

		//	�������:
	protected:
		InsideType*	_data;
	};
}

#include "NETArrayWithSameTyped.inl"
#include "NETPointerArrayWithSameTyped.hpp"
#include "NETPointerArrayUsingPolyMorphismWithSameTyped.hpp"