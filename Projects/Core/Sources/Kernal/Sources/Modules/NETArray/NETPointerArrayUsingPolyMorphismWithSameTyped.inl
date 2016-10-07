//	---------------------------------------------------------------------------------
//	Ŭ������:	NETVector<InsideType>
//	����	:	���ø� ���ڷ� �����͸� ����ϰ�, �� �����Ͱ� ���� ������ �ν��Ͻ���
//				����Ű��, Array�� ���Ҷ� �����Ǵ� Ŭ����.
//				��, ���ø� ���ڰ� �����Ͷ�� ������ �ܼ��� �����͸� ������ �ִ�
//				Array��� NETVector<InsideType>�� ���� ���̴�.
//				�̴�, 2��° ���ø�������
//	����	:	���Ŭ����.			NETReservedCollector
//				�Ϲ� ���ø� Ŭ����.	Array<InsideType>
//	Ư��	:	2��° ���ø� ������ type_bool pointerUseNewInstance�� true��?
//					:	�����͸� deepcopy�Ѵٴ� ���� ���Ѵ�.
//						�������, NENode a; �� �ּ��� &a�� ���� pointerUseNewInstance
//						�� true�϶��� false�϶��� ������ ���̸� ���캸��.
//							1. NETVector<InsideType*, false> �� �� 
//								: �����Ͱ� ����Ǿ� ������Ʈ�� ����. ���� �� ���̴�.
//							2. This �� ��
//								: �����Ͱ� ����Ű�� �ν��Ͻ����� ����Ǿ� ����.
//								�����ͷκ��� ����� ������Ʈ�� �����Ϳʹ� ������ �ν�
//								�Ͻ��� ������.
//						This�� �̿� ���� ������ InsideType* �������� �����ϰ��� �Ҷ�
//						����� ���ȴ�. 
//	�˰���:	NETVector<InsideType>�� �����ϴ�. �ش� ��������� ������ ��.
//	�����:	NETVector<InsideType>�� �����ϴ�. �ش� ��������� ������ ��.
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

namespace NE
{
	template <typename InsideType>
	class NETVector<InsideType*, true, InsideType*> : public NETReservedCollector<InsideType*>, public NESpecifiedInsertable<InsideType*> // InsideType = NEKey
	{
		typedef NETVector<InsideType*, true, InsideType*> _This;

		NE_DECLARE_CLASS_ONLY(_This, NETReservedCollector<InsideType*>)
	public:
		using Super::push;
		using Super::match;
		using Super::setElement;
		using Super::serialize;

		//	������:
	public:
		NETVector();
		NETVector(type_count size);
		NETVector(const This& source);

		//	�Ҹ���:
	public:
		virtual ~NETVector();

		//	������ ��ø:
	public:
		const This& operator=(const This& source);
		type_bool operator==(const This& source) const;
		type_bool operator!=(const This& source) const;
		const This& operator+=(const This& source);
		This operator+(const This& source) const;

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
		virtual type_index match(const InsideType* const source) const;
		virtual type_index push(const InsideType* const source);
		virtual type_index pop();
		//			NEObject:
	public:
		virtual type_result isValid() const; 
		virtual void release(); 
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//		���� �������̽�:
	public:
		type_index match(const InsideType& source) const;
		type_index popFront();
		type_index pushFront(const InsideType* const source);
		type_index pushFront(const InsideType& source);
		type_index insert(type_index index, const InsideType& source);
		type_result push(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_result pushFront(const This& source); // pushArrayFront�� �ʿ䰡 ����. source�ʿ��� push�� ȣ���ϸ� �Ǵϱ�
		type_bool isEqualSizeAndElement(const This& source) const;
		type_bool isEqualElement(const This& source) const;		

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		void _release();
		type_result _returnHeapMemory(type_index index);

		//	�������:
	protected:
		InsideType**	_data;
	};
}