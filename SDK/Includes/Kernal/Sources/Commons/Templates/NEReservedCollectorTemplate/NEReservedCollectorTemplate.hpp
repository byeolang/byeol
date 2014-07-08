//	---------------------------------------------------------------------------------
//	Ŭ������:	NEReservedCollectorTemplate
//	����	:	�����͸� �����ϴ� �ڷᱸ���� ��Ÿ����.
//	����	:	���Ŭ����.	NECollectorTemplate
//	Ư��	:	�����͸� �����ϴ� �ٽ����� �Լ��� �����Լ��� �Ǿ��ֱ� ������ ���� 
//				�������� �����ϴ�.
//	�˰���:	
//	�����:	�⺻ �������̽�
//					1.	getSize()	:	�������� ���� ũ��
//					2.	getSize()	:	�������� ���� ����
//					3.	push()		:	�����͸� ���� �ڿ� �߰�
//					4.	pushFront()	:	�����͸� ���� �տ� �߰�
//					5.	pop()		:	���� ���� �����͸� �ϳ� ����
//					6.	popFront()	:	�� ���� �����͸� �ϳ� ����
//
//				���� �Լ�
//					1.	insert()	:	�����͸� ������ �ε����� ��ġ�� ����
//					2.	remove()	:	������ �ε����� ��ġ�ϴ� �����͸� ����
//					3.	find()		:	�־��� ���ҿ� ��ġ�ϴ� ���Ҹ� Collector����
//										�˻�. (���������� operator==�� �ǽ��Ѵ�)
//					4.	getElement():	�־��� �ε����� �ش��ϴ� ���ҿ� �����Ѵ�.
//					5.	setElement():	�־��� �ε����� ������Ʈ�� �����͸� ��ü�Ѵ�.
//										�߰��� �ƴϴ�.
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NECollectorTemplate/NECollectorTemplate.hpp"

namespace NE
{
	template<typename OutsideType>
	class NEReservedCollectorTemplate : public NECollectorTemplate<OutsideType>
	{
		//	������:
	public:
		NEReservedCollectorTemplate();
		NEReservedCollectorTemplate(type_index size);
		NEReservedCollectorTemplate(const NEReservedCollectorTemplate& source);

		//	�Ҹ���:
	public:
		virtual ~NEReservedCollectorTemplate();	

		//	������ ��ø:
	public:
		const NEReservedCollectorTemplate<OutsideType>& operator=(const NEReservedCollectorTemplate<OutsideType>& source); 
		bool operator==(const NEReservedCollectorTemplate<OutsideType>& source) const;
		bool operator!=(const NEReservedCollectorTemplate<OutsideType>& source) const;

		//	������:	������ �����Ѵٸ� ��������ڸ� �����Ѵ�. ������ ��������ڴ� �Ļ�Ŭ�������� ȣ���
	public:
		type_index getSize() const;
		type_index getSizeLastIndex() const;		

		//	�������̽�:
		//		���:
	public:
		virtual type_result create(type_index new_size) = 0;
		virtual type_result resize(type_index new_size) = 0;

	public:
		virtual void release(); 
		virtual type_result isValid() const; 
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		
		//	�����Լ�:
	private: 
		const NEReservedCollectorTemplate<OutsideType>& _assign(const NEReservedCollectorTemplate<OutsideType>& source);
		void _release();

		//	�������:
	protected:
		type_index _size;
	}; 
}

#include "NEReservedCollectorTemplate.inl"
#include "NEPointerReservedCollectorTemplate.hpp"
