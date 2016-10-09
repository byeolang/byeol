///	@brief	A kind of @see NETCollector that manages element splitly.
///	@detail	This seems that @see NETVector, but this usually uses when you don't
///			need to concern order among elements.
///			for example, when you call, 
///
///				yourList += yourList
///
///			Next then, yourList's order of the elements are unpredictable.
///			new elements from the rigth hand "yourList" will be inserted to left handed
///			"yourList" from index 0	to "length-1" if it's vacant.
///
///			And, Of cource, this is like a traditional C array, it's far faster and got low
///			performance cost when you don't need not to make it space between elements.
//	---------------------------------------------------------------------------------
//	Ŭ������:	NETArray
//	����	:	Array�� ���·� �����͸� �����ϴ� Ŭ����.
//	����	:	���Ŭ����.			NETReservedCollector
//	Ư��	:	�迭�� Ư���� �״�� �����Ѵ�. �� ������Ʈ�� �߰�/����/������
//				���ϰ� ū �ݸ�, "�˻�"�� ���ϴ�.//				
//	�˰���:	���� �����͸� �������� ���, �ش� �����ͷκ��� ���󺹻�������� clone
//				�� ȣ���ؼ� DeepCopy �Ѵ�.
//	�����:	���� ���
//					:	���ø� ���� T�� ����
//							���� 1.	NEObject�� ����ؾ��Ѵ�. 
//									NEObject::clone()�� ����ϱ� �����̴�.
//							���� 2. 
//				type_bool pointerUseNewInstance��?
//					:	false�϶� T�� �������� ���, �����Ͱ� ���ο� �ν��Ͻ��� ����Ű����
//						�����ʰ�, shallowcopy���� ����ϰڴٴ� �ǹ̴�.
//						true�϶��� NEPointerList.hpp�� �����϶�.
//				�⺻������ NETCollector�� �Լ��� ��� ��밡���ϴ�. (���� �� ��)
//				�⺻ �������̽�
//					1. push(T*)			:	�������� ���·� ������ �����ϴ�.
//					2. pushFront(T*)	:	�������� ���·� ������ �����ϴ�.
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
#include "../NETClass/NETClass.inl"
#include "../NETReservedCollector/NETReservedCollector.inl"
#include "../NETIterator/NETIterator.inl"

namespace NE
{
	template <typename T>
	class NETArray : public NETReservedCollector<T>
	{	
		typedef NETArray<T> _This;

		NE_DECLARE_CLASS(_This, NETReservedCollector<T>)

	protected:
		struct Node
		{
			type_bool is_occupied;
			T datum;
		};
		class Iterator : public NETIterator<T>
		{
			NE_DECLARE_CLASS_ONLY(Iterator, NETIterator<T>)

		public:
			Iterator(_This& owner, type_index new_index);

		public:
			type_bool operator==(const This& rhs) const;
			virtual type_bool operator<(const This& rhs) const;
			virtual type_bool operator>(const This& rhs) const;

		public:
			virtual const T& get() const;
			virtual T& get();
			virtual type_bool isEnd() const;
			virtual type_result next(type_index step_for_next);
			virtual type_result back(type_index step_for_back);
			//		NELengthedObject:
			virtual type_index getLength() const;
			//		NEObject:
			virtual void release();
			virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
			virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
			virtual type_result isValid() const;

		public:
			_This& getOwner();
			const _This& getOwner() const;
			type_result setIndex(type_index new_index);
			type_index getIndex() const;

		private:
			NETWeakBinder<_This> _owner;	//	we'll keep track of owner-instance for checking its validness.
			type_index _index;
		};
		class CIterator : public NETCIterator<T>
		{
			NE_DECLARE_CLASS_ONLY(CIterator, NETCIterator<T>)

		public:
			CIterator(const _This& owner, type_index new_index);

		public:
			type_bool operator==(const This& rhs) const;		
			virtual type_bool operator<(const This& rhs) const;
			virtual type_bool operator>(const This& rhs) const;	

		public:
			virtual const T& get() const;
			virtual type_bool isEnd() const;
			virtual type_result next(type_index step_for_next);
			virtual type_result back(type_index step_for_back);
			//		NELengthedObject:
			virtual type_index getLength() const;
			//		NEObject:
			virtual void release();
			virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
			virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
			virtual type_result isValid() const;

		public:
			const _This& getOwner() const;
			type_result setIndex(type_index new_index);
			type_index getIndex() const;

		private:
			NETWeakBinder<const _This> _owner;	//	we'll keep track of owner-instance for checking its validness.
			type_index _index;
		};

	public:
		friend class Iterator;
		friend class CIterator;

		//	������:
	public:
		NETArray();
		NETArray(type_count size);
		NETArray(const This& source);
		virtual ~NETArray();

		//	������ ��ø:
	public:
		const This& operator=(const This& source); 
		type_bool operator==(const This& source) const;
		type_bool operator!=(const This& source) const;
		const This& operator+=(const This& source);
		This operator+(const This& source) const;

		//	Interfaces:
		//		Virtuals:
	public:
		//		NETReservedCollector:
		virtual type_result resize(type_index new_size);
		virtual type_result create(type_count new_size);		
		//			NETCollector
		virtual NETBinder<NETCIterator<T>> getIterator(type_index index) const;
		virtual NETBinder<NETIterator<T>> getIterator(type_index index);
		///	@brief	insert Element to designated index.
		///	@return	Iterator pointing inserted one.
		///			if there is already element to where you want to insert, this calls
		///			replace the element to pramaeter "source".
		virtual NETBinder<NETIterator<T>> insert(const NETCIterator<T>& where, const T& source);
		virtual NETBinder<NETIterator<T>> insert(const NETCIterator<T>& where, const NETCIterator<T>& from, const NETCIterator<T>& end);
		virtual NETBinder<NETIterator<T>> remove(const NETCIterator<T>& from, const NETCIterator<T>& end);
		//			We're overriding below ones because of optimization: 
		virtual T& getElement(type_index index);
		virtual const T& getElement(type_index index) const;
		//			NEObject:
		virtual type_result isValid() const;
		virtual void release();
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		type_bool _isValid(const CIterator& iterator) const;
		void _release();

		//	�������:
	protected:
		Node* _data;
	};
}