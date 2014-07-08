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
		_real_index = NE_INDEX_ERROR;
		_comparing_id = NE_INDEX_ERROR;

		return RESULT_SUCCESS;
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

}