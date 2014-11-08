#include "../../../Modules/Kernal/Kernal.hpp"

namespace NE
{
	type_result NEKeyNameBinder::bind()
	{
		using namespace NEType;
		NENodeManager& noder = Kernal::getInstance().getNodeManager();				
		const NEKeyCodeSet& ks = noder.getLocalStack().getRecentNode().getKeySet();	//	��¼�� bind�Լ��� ȣ��Ǵ� ������ NodeManager.execute()�� �Ǵ� �������̱� ������ _manager�� noder��� �����Ѵ�.


		//	main:		
		for(int n=0; n < ks.getLength() ;n++)
			if(ks[n].getName() == _keyname)
				return SuperClass::bind(ks[n], NEType::NENODE_MANAGER);


		//	post:
		KERNAL_WARNING("KeyName ���ε��� �����߽��ϴ�. �־��� �̸� %s��, �� ��忡 ������ ���� �� �����ϴ�.\n���ε��� ���������Ƿ� �⺻���� �Ҵ�� ���Դϴ�.", _keyname.toCharPointer());
		return RESULT_TYPE_WARNING;
	}

	type_result NEKeyNameBinder::setKeyName(const NETString& new_keyname)
	{
		_keyname = new_keyname;
		
		//	Release Binding ����:
		return unbind();
	}

	const NETString& NEKeyNameBinder::getKeyName() const
	{	
		return _keyname;
	}

	NEKeyNameBinder::NEKeyNameBinder()
	{

	}

	NEKeyNameBinder::NEKeyNameBinder(const NETString& new_keyname)
		: _keyname(new_keyname)
	{

	}

	NEKeyNameBinder::NEKeyNameBinder(const NEKeyNameBinder& rhs)
		: SuperClass(rhs)
	{
		_assign(rhs);
	}

	NEKeyNameBinder& NEKeyNameBinder::operator=(const NEKeyNameBinder& rhs)
	{
		if(this == &rhs) return *this;

		SuperClass::operator=(rhs);

		return _assign(rhs);
	}

	bool NEKeyNameBinder::operator==(const NEKeyNameBinder& rhs) const
	{
		return SuperClass::operator==(rhs) && _keyname == rhs._keyname;
	}

	bool NEKeyNameBinder::operator!=(const NEKeyNameBinder& rhs) const
	{
		return ! operator==(rhs);
	}

	NEKeyNameBinder& NEKeyNameBinder::_assign(const ThisClass& rhs)
	{
		SuperClass::release();
		//	�� release()�� �����ϴ°�:
		//		NEKeyNameBinder�� Module��ü �ȿ����� scope�� ��ȿ�� Ŭ������.
		//		���� �ٸ� Module ��ü������ �ٸ� ���ε��� ����Ǿ�� �Ѵ�.
		//		(���� KeyName�� �����ϰ� �ִٰ� �ϴ��� NEKeyNameBinder��
		//		�����ϰ� �ִ� ����� �ٸ� ��ü��, �ٸ� Ű�� ���ε� �Ǿ�� �Ѵ�
		//		��� ����)
		//		���⼭ �־��� rhs�� �ܺ� ����� NEKeyNameBinder�� �� �ִ�.
		//		���� ���ε� ������ �Ҵ����� �ʰ�, keyname�� �Ҵ��Ѵ�.
		//		����, "rhs�� ���ε��ϴ� Ű�� ��"�� �Ҵ��ϰ� ���� �Ŷ��,
		//		this->getBinded() = rhs.getBinded(); �� �����ϸ� �ȴ�.

		_keyname = rhs._keyname;

		return *this;
	}

	void NEKeyNameBinder::release()
	{
		_keyname.release();
		SuperClass::release();
	}

	type_result NEKeyNameBinder::isValid() const
	{
		type_result result = 0;
		if(result = _keyname.isValid())	return result;

		return SuperClass::isValid();
	}

	NEBinaryFileLoader& NEKeyNameBinder::serialize(NEBinaryFileLoader& loader)
	{
		//	����Ŭ������ ���ε���� ������ ����(���� �� �ε�)�� �� ����.

		return loader >> _keyname;
	}

	NEBinaryFileSaver& NEKeyNameBinder::serialize(NEBinaryFileSaver& saver) const
	{
		//	����Ŭ������ ���ε���� ������ ����(���� �� �ε�)�� �� ����.

		return saver << _keyname;
	}

	NEObject& NEKeyNameBinder::clone() const
	{
		return *(new ThisClass(*this));
	}

	NEType::Type NEKeyNameBinder::getType() const
	{
		return NEType::NEKEY_NAME_BINDER;
	}
}