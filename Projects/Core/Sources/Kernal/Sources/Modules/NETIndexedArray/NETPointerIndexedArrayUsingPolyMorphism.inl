//	---------------------------------------------------------------------------------
//	Ŭ������:	NETIndexedArray<Type>
//	����	:	���ø� ���ڷ� �����͸� ����ϰ�, �� �����Ͱ� ���� ������ �ν��Ͻ���
//				����Ű��, Array�� ���Ҷ� �����Ǵ� Ŭ����.
//				��, ���ø� ���ڰ� �����Ͷ�� ������ �ܼ��� �����͸� ������ �ִ�
//				Array��� NETIndexedArray<Type>�� ���� ���̴�.
//				�̴�, 2��° ���ø�������
//	����	:	���Ŭ����.			NETReservedCollector
//				�Ϲ� ���ø� Ŭ����.	Array<Type>
//	Ư��	:	2��° ���ø� ������ type_bool pointerUseNewInstance�� true��?
//					:	�����͸� deepcopy�Ѵٴ� ���� ���Ѵ�.
//						�������, NENode a; �� �ּ��� &a�� ���� pointerUseNewInstance
//						�� true�϶��� false�϶��� ������ ���̸� ���캸��.
//							1. NETIndexedArray<Type*, false> �� �� 
//								: �����Ͱ� ����Ǿ� ������Ʈ�� ����. ���� �� ���̴�.
//							2. This �� ��
//								: �����Ͱ� ����Ű�� �ν��Ͻ����� ����Ǿ� ����.
//								�����ͷκ��� ����� ������Ʈ�� �����Ϳʹ� ������ �ν�
//								�Ͻ��� ������.
//						This�� �̿� ���� ������ Type* �������� �����ϰ��� �Ҷ�
//						����� ���ȴ�. 
//	�˰���:	NETIndexedArray<Type>�� �����ϴ�. �ش� ��������� ������ ��.
//	�����:	NETIndexedArray<Type>�� �����ϴ�. �ش� ��������� ������ ��.
//	�޸�	:	
//	�����丮:	2013-01-10	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

namespace NE
{
	template <typename Type>
	class NETIndexedArray<Type*, true> : public NETReservedCollector<Type*>, public NESpecifiedInsertable<Type*> // Type = NEKey
	{
		typedef NETIndexedArray<Type*, true> _This;

		NE_DECLARE_CLASS(_This, NETReservedCollector<Type*>)

	public:
		using Super::push;
		using Super::match;
		using Super::setElement;
		//	������:
	public:
		NETIndexedArray();
		NETIndexedArray(type_count size);
		NETIndexedArray(const This& source);

		//	�Ҹ���:
	public:
		virtual ~NETIndexedArray();

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
		virtual type_result insert(type_index index, const Type* const source);
		virtual type_index remove(type_index index);
		//			NETReservedCollector:
	public:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);
		//			NETCollector:
	public:
		virtual const Type& getElement(type_index index) const; 
		virtual Type& getElement(type_index index); 
		virtual type_result setElement(type_index index, const Type* const source); 
		virtual type_index match(const Type* const source) const;
		virtual type_index push(const Type* const source);
		virtual type_index pop();
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual void release();
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;

		//		���� �������̽�:
	public:
		type_index popFront();
		type_index pushFront(const Type* const source);
		type_index pushFront(const Type& source);
		type_index insert(type_index index, const Type& source);
		type_result push(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_bool isEqualSizeAndElement(const This& source) const;
		type_bool isEqualElement(const This& source) const;		

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		void _release();
		type_result _returnHeapMemory(type_index index);
		type_index _searchIndex(type_bool by_descreasing, type_bool target_is_occupied) const;

		//	�������:
	protected:
		Type**	_data;
		NEOccupiedSet _occupiedset;
	};
}