#include "NEArgumentSet.hpp"
#include "../../Commons/Interfaces/NEArgumentInterfaceBase/NEArgumentInterfaceBase.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	NEArgumentSet::NEArgumentSet(NEModule* owner, type_count size)
		: SuperClass(size), _owner(owner)
	{

	}

	NEArgumentSet::NEArgumentSet(NEModule* owner, const ThisClass& rhs)
		: SuperClass(rhs), _owner(owner)
	{

	}

	NEArgumentSet::NEArgumentSet(NEModule* owner)
		: SuperClass(), _owner(owner)
	{

	}

	type_index NEArgumentSet::insert(type_index index, NEArgumentInterfaceBase& base)
	{
		return base._onInsertedInArguments(index, *this);
	}
	type_index NEArgumentSet::push(NEArgumentInterfaceBase& base)
	{
		return base._onInsertedInArguments(getLength(), *this);
	}
	type_index NEArgumentSet::pushFront(NEArgumentInterfaceBase& base)
	{
		return base._onInsertedInArguments(0, *this);
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

	NEModule& NEArgumentSet::getOwner()
	{
		return *_owner;
	}

	const NEModule& NEArgumentSet::getOwner() const
	{
		return *_owner;
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
			if( ! getElement(n).isBinded()) return false;

		return true;
	}

}