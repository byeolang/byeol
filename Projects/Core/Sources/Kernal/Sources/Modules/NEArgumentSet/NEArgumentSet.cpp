#include "NEArgumentSet.hpp"
#include "../Kernal/Kernal.hpp"

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
			if( ! getElement(n).isBinded()) return false;

		return true;
	}

	type_result NEArgumentSet::bind()
	{
		type_result to_return = RESULT_SUCCESS;

		for(int n=0; n < getLength() ;n++)
		{
			NEArgumentBase& arg = getElement(n);
			if( ! arg.isBinded())
				to_return |= arg.bind();
		}

		return to_return;
	}

}