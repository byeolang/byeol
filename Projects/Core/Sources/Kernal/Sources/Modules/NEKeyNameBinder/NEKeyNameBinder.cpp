#include "../Kernal/Kernal.hpp"

namespace NE
{
	type_result NEKeyNameBinder::bind()
	{
		using namespace NEType;
		NENodeManager& noder = Kernal::getInstance().getNodeManager();				
		NENodeManager::LocalStack& stack = noder.getLocalStack();
		const NEKeyCodeSet& ks = stack.getRecentNode().getKeySet();	//	��¼�� bind�Լ��� ȣ��Ǵ� ������ NodeManager.execute()�� �Ǵ� �������̱� ������ _manager�� noder��� �����Ѵ�.
		if(_keyname.getLength() <= 0) return RESULT_TYPE_WARNING | RESULT_ABORT_ACTION | RESULT_INVALID_DATA;

		//	main:
		if(_keyname[0] == TCHAR('$'))
			return _bindLocalKey();
		//		�����ִ� Node.KeySet���� Ž��:
		for(int n=0; n < ks.getLength() ;n++)
			if(ks[n].getName() == _keyname)
				return Super::bind(ks[n]/*, NEType::NENODE_MANAGER*/);


		//	post:
		KERNAL_WARNING("KeyName ���ε��� �����߽��ϴ�. �־��� �̸� %s��, �� ��忡 ������ ���� �� �����ϴ�.\n���ε��� ���������Ƿ� �⺻���� �Ҵ�� ���Դϴ�.", _keyname.toCharPointer());
		return RESULT_TYPE_WARNING;
	}

	type_result NEKeyNameBinder::_bindLocalKey()
	{
		//		LocalKeys���� Ž��:
		const NENodeManager::LocalStack::PersistentKeySet& local_keys = Kernal::getInstance().getNodeManager().getLocalStack().getLocalKeySet();
		for(int n = 0; n < local_keys.getLength(); n++)
		{
			const NEKey& key = local_keys[n];
			if (&key && key.getName().toLowerCase() == _keyname.toLowerCase())
			{
				type_result result = Super::bind(key/*, NEType::LOCALSTACK*/);

				if( ! NEResult::hasError(result))
					_manager_type = NEType::LOCALSTACK;

				return result;
			}
		}

		/*_manager_type = NEType::NENODE_MANAGER;*/
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
		: Super()	//	��������ڸ� ȣ������ �ʴ´�. �ڼ��� ������ _assign ����
	{
		_assign(rhs);
	}

	NEKeyNameBinder& NEKeyNameBinder::operator=(const NEKeyNameBinder& rhs)
	{
		if(this == &rhs) return *this;

		//	Super::operator=(rhs);	����Ŭ������ �޼ҵ� ü�̴� ���� �ʴ´�. �ڼ��� ������ _assign ����.

		return _assign(rhs);
	}

	type_bool NEKeyNameBinder::operator==(const NEKeyNameBinder& rhs) const
	{
		return Super::operator==(rhs) && _keyname == rhs._keyname;
	}

	type_bool NEKeyNameBinder::operator!=(const NEKeyNameBinder& rhs) const
	{
		return ! operator==(rhs);
	}

	NEKeyNameBinder& NEKeyNameBinder::_assign(const This& rhs)
	{
		Super::release();
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

		//	���� Ŭ������ ���ε� ������ ������ �ʴ� Enable�� ���⼭ ������ �Ҵ��� �ش�
		setEnable(rhs.isEnable());

		_keyname = rhs._keyname;

		return *this;
	}

	void NEKeyNameBinder::release()
	{
		_keyname.release();
		Super::release();
	}

	type_result NEKeyNameBinder::isValid() const
	{
		type_result result = 0;
		if(result = _keyname.isValid())	return result;

		return Super::isValid();
	}

	NEBinaryFileLoader& NEKeyNameBinder::serialize(NEBinaryFileLoader& loader)
	{
		NESwitchableUnit::serialize(loader);
		//	�߰� Ŭ����(NEBinderBase���� NEKeyBinder)�� ���ε���� ������ ����(���� �� �ε�)�� �� ����.		

		return loader >> _keyname;
	}

	NEBinaryFileSaver& NEKeyNameBinder::serialize(NEBinaryFileSaver& saver) const
	{
		NESwitchableUnit::serialize(saver);
		//	�߰� Ŭ����(NEBinderBase���� NEKeyBinder)�� ���ε���� ������ ����(���� �� �ε�)�� �� ����.		

		return saver << _keyname;
	}
}