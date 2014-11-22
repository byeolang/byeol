#include "NEArgumentSet.hpp"
#include "../Kernal/Kernal.hpp"
#include "../../Commons/Units/NEArgumentInterfaceBase/NEArgumentInterfaceBase.hpp"

namespace NE
{
	NEArgumentSet::NEArgumentSet(type_count size)
		: SuperClass(size)
	{

	}

	NEArgumentSet::NEArgumentSet(const ThisClass& rhs)
		: SuperClass(rhs)
	{

	}

	NEArgumentSet::NEArgumentSet()
		: SuperClass()
	{

	}

	NEBinaryFileSaver& NEArgumentSet::serialize(NEBinaryFileSaver& saver) const
	{
		//	@�ӽ�:
		//	NEArgument�� Serialize ��å:
		//		�̷� �������� �̿��� �迭�� ���������� �̿��ؼ� �������κ��� �ν��Ͻ��� �����ϴ�
		//		������ serialize�� ���ԵǾ�� �Ѵ�. �׷���, Argument�� �ν��Ͻ��� ����ִٰ� �����ϰ�
		//		serialize�� �����Ѵ�.
		//		�ν��Ͻ��� ä������ ���ؼ� ���������� Manager ������ �����ϴ� ��ü�� ����⿡�� ������
		//		�۱� �����̴�.
		//		��¥�� �ӽ� �ڵ� �̹Ƿ� NEModule�� _onFetchArguement�� �Լ����� �ν��Ͻ��� �������� ����
		//		�� ������ ���̸�, �̸��� NEArgumentSet::serialize�� �ݵ�� �ܺο��� ȣ��Ǵ� ���� �ƴ϶�
		//		NEModule�������� ȣ���� �Ǿ�� �Ѵٴ� �ǹ̰� �ȴ�.
		saver << getLength();

		for(int n=0; n < getLength() ;n++)
			saver << getElement(n);

		return saver;
	}

	NEBinaryFileLoader& NEArgumentSet::serialize(NEBinaryFileLoader& loader)
	{
		/*
			NEArgument�� �ν��Ͻ��� �������� ����ϹǷ� ��Ģ������ load�� �Ұ����ϴ�.
			�׷��� ���� NEModule������ ȣ��Ǿ�� �Ѵ�.
			�ܺο��� ȣ���� �� ������ private�� ���������ڰ� �ɷ��ִ�.
		*/
		//	main:
		//		������ �ν��Ͻ��� �������� load ������ ����:
		type_count length = 0;
		loader >> length;
		if(length != getLength())
			KERNAL_ERROR("���̰� ���� �ʴ´�. ��𼱰� �����Ͱ� ������");

		for(int n=0; n < getLength() ;n++)
			loader >> getElement(n);

		return loader;
	}

	NEArgumentSet& NEArgumentSet::operator=(const ThisClass& rhs)
	{
		if(this == &rhs) return *this;

		SuperClass::operator=(rhs);

		return *this;
	}

	bool NEArgumentSet::isBinded() const
	{
		for(int n=0; n < getLength() ;n++)
			if( ! getElement(n).isBinded()) 
				return false;

		return true;
	}

	type_result NEArgumentSet::bind()
	{
		type_result to_return = RESULT_SUCCESS;

		for(int n=0; n < getLength() ;n++)
		{
			NEArgumentBase& arg = getElement(n);

			if( arg.isNeedingBinding() && ! arg.isBinded())
				to_return |= arg.bind();
		}

		return to_return;
	}

	type_index NEArgumentSet::insert(type_index index, NEArgumentInterfaceBase& basis)
	{
		type_result r = basis._onInsertedInArguments(index, *this);
		r |= basis._onBindInstance(getElement(index));
		if(NEResult::hasError(r))
			return NE_INDEX_ERROR;

		return index;
	}

	type_index NEArgumentSet::push(NEArgumentInterfaceList& basises)
	{
		typedef NEArgumentInterfaceList::Iterator Iterator;

		for(Iterator* itr=basises.getIterator(0); itr ;itr=itr->getNext())
			push(itr->getValue());

		return RESULT_SUCCESS;
	}	

	NE::type_index NEArgumentSet::push(NEArgumentInterfaceBase& basis)
	{
		return insert(getLength(), basis);
	}

	NE::type_index NEArgumentSet::pushFront(NEArgumentInterfaceBase& basis)
	{
		return insert(0, basis);
	}

	type_result NEArgumentSet::bind(NEArgumentInterfaceList& basis)
	{
		int n=0;
		for(int n=0; n < getLength() ;n++, basis.popFront())
		{
			NEArgumentBase& e = getElement(n);
			NEArgumentInterfaceBase& base = basis[0];
			
			if(e.getTypeToBeBinded() != basis[0].getKeyType())
				base._onInsertedInArguments(n, *this);
			base._onBindInstance(e);
		}


		//	post:
		push(basis);	//		�����ִ� �͵��� ��� �߰��Ѵ�.
		return RESULT_SUCCESS;
	}

	type_result NEArgumentSet::update()
	{
		type_result result = RESULT_SUCCESS;

		for(int n=0; n < getLength() ;n++)
			result |= getElement(n).update();

		return result;
	}
}