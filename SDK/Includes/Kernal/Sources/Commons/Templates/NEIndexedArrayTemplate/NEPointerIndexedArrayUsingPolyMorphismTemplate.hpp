//	---------------------------------------------------------------------------------
//	Ŭ������:	NEIndexedArrayTemplate<Type>
//	����	:	���ø� ���ڷ� �����͸� ����ϰ�, �� �����Ͱ� ���� ������ �ν��Ͻ���
//				����Ű��, Array�� ���Ҷ� �����Ǵ� Ŭ����.
//				��, ���ø� ���ڰ� �����Ͷ�� ������ �ܼ��� �����͸� ������ �ִ�
//				Array��� NEIndexedArrayTemplate<Type>�� ���� ���̴�.
//				�̴�, 2��° ���ø�������
//	����	:	���Ŭ����.			NEReservedCollectorTemplate
//				�Ϲ� ���ø� Ŭ����.	Array<Type>
//	Ư��	:	2��° ���ø� ������ bool pointerUseNewInstance�� true��?
//					:	�����͸� deepcopy�Ѵٴ� ���� ���Ѵ�.
//						�������, NENode a; �� �ּ��� &a�� ���� pointerUseNewInstance
//						�� true�϶��� false�϶��� ������ ���̸� ���캸��.
//							1. NEIndexedArrayTemplate<Type*, false> �� �� 
//								: �����Ͱ� ����Ǿ� ������Ʈ�� ����. ���� �� ���̴�.
//							2. ThisClass �� ��
//								: �����Ͱ� ����Ű�� �ν��Ͻ����� ����Ǿ� ����.
//								�����ͷκ��� ����� ������Ʈ�� �����Ϳʹ� ������ �ν�
//								�Ͻ��� ������.
//						ThisClass�� �̿� ���� ������ Type* �������� �����ϰ��� �Ҷ�
//						����� ���ȴ�. 
//	�˰���:	NEIndexedArrayTemplate<Type>�� �����ϴ�. �ش� ��������� ������ ��.
//	�����:	NEIndexedArrayTemplate<Type>�� �����ϴ�. �ش� ��������� ������ ��.
//	�޸�	:	
//	�����丮:	2013-01-10	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

namespace NE
{
	template <typename Type, NEType::Type type>
	class NEIndexedArrayTemplate<Type*, true, type> : public NEReservedCollectorTemplate<Type*>, public NESpecifiedInsertable<Type*> // Type = NEKey
	{
		typedef NEReservedCollectorTemplate<Type*> SuperClass;
		typedef NEIndexedArrayTemplate<Type*, true, type> ThisClass;

	public:
		using SuperClass::push;
		using SuperClass::find;
		using SuperClass::setElement;
		//	������:
	public:
		NEIndexedArrayTemplate();
		NEIndexedArrayTemplate(type_count size);
		NEIndexedArrayTemplate(const ThisClass& source);

		//	�Ҹ���:
	public:
		virtual ~NEIndexedArrayTemplate();

		//	������ ��ø:
	public:
		const ThisClass& operator=(const ThisClass& source); 
		ThisClass operator+(const ThisClass& source) const;
		bool operator==(const ThisClass& source) const;
		bool operator!=(const ThisClass& source) const;
		const ThisClass& operator+=(const ThisClass& source);

		//	�������̽�:
		//		������:
	public:
		const NEOccupiedSet& getOccupiedSet() const;

		//		���:
		//			NESpecifiedInsertable:
	public:
		virtual type_result insert(type_index index, const Type* const source);
		virtual type_index remove(type_index index);
		//			NEReservedCollectorTemplate:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NECollectorTemplate:
	public:
		virtual const Type& getElement(type_index index) const; 
		virtual Type& getElement(type_index index); 
		virtual type_result setElement(type_index index, const Type* const source); 
		virtual type_index find(const Type* const source) const;
		virtual type_index push(const Type* const source);
		virtual type_index pop();
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual void release(); 
		virtual NEType::Type getType() const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEObject& clone() const;

		//		���� �������̽�:
	public:
		type_index popFront();
		type_index pushFront(const Type* const source);
		type_index pushFront(const Type& source);
		type_index insert(type_index index, const Type& source);
		type_result push(const ThisClass& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const ThisClass& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		bool isEqualSizeAndElement(const ThisClass& source) const;
		bool isEqualElement(const ThisClass& source) const;		

		//	�����Լ�:
	private:
		const ThisClass& _assign(const ThisClass& source);
		void _release();
		type_result _returnHeapMemory(type_index index);
		type_index _searchIndex(bool by_descreasing, bool target_is_occupied) const;

		//	�������:
	protected:
		Type**	_data;
		NEOccupiedSet _occupiedset;
	};
}

#include "NEPointerIndexedArrayUsingPolyMorphismTemplate.inl"