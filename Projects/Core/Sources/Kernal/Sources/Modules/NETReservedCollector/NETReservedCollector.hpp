//	---------------------------------------------------------------------------------
//	Ŭ������:	NETReservedCollector
//	����	:	�����͸� �����ϴ� �ڷᱸ���� ��Ÿ����.
//	����	:	���Ŭ����.	NETCollector
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
#include "../NETCollector/NETCollector.hpp"

namespace NE
{
	template<typename OutsideType>
	class NETReservedCollector : public NETCollector<OutsideType>
	{
		//	������:
	public:
		NETReservedCollector();
		NETReservedCollector(type_index size);
		NETReservedCollector(const NETReservedCollector& source);

		//	�Ҹ���:
	public:
		virtual ~NETReservedCollector();	

		//	������ ��ø:
	public:
		const NETReservedCollector<OutsideType>& operator=(const NETReservedCollector<OutsideType>& source); 
		bool operator==(const NETReservedCollector<OutsideType>& source) const;
		bool operator!=(const NETReservedCollector<OutsideType>& source) const;

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
		const NETReservedCollector<OutsideType>& _assign(const NETReservedCollector<OutsideType>& source);
		void _release();

		//	�������:
	protected:
		type_index _size;
	}; 
}

#include "NETReservedCollector.inl"
#include "NETPointerReservedCollector.hpp"
