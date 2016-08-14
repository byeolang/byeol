#include "NEKeyBinder.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	type_bool NEKeyBinder::isBinded() const
	{
		if(isBindedLocalKey()) return true;
		const NEIndexedKeySet& keyset = _getKeySet();
		if( ! &keyset)	return false;
		const NEKey& key = keyset[getRealIndex()];
		if( ! &key) return false;

		return	key.getId() == getComparingId();
	}

	NEKey& NEKeyBinder::getBinded()
	{
		NEKey* nullpointer = NE_NULL;
		if( ! isBinded())
			return *nullpointer;

		return _getKeySet()[getRealIndex()];
	}

	const NEKey& NEKeyBinder::getBinded() const
	{
		const NEKey* nullpointer = NE_NULL;
		if( ! isBinded())
			return *nullpointer;

		return _getKeySet()[getRealIndex()];
	}

	NEIndexedKeySet& NEKeyBinder::_getKeySet()
	{
		if(NEType::isValidHierachy(NEType::LOCALSTACK, _manager_type))
			return Kernal::getInstance().getNodeManager().getLocalStack()._getLocalKeySet();
		NEIndexedKeySet* nullpointer = NE_NULL;
		NEEnlistableManager& manager = getManager();

		if( ! &manager)
			return *nullpointer;

		return manager._getKeySet();
	}

	const NEIndexedKeySet& NEKeyBinder::_getKeySet() const
	{
		if(NEType::isValidHierachy(NEType::LOCALSTACK, _manager_type))
			return Kernal::getInstance().getNodeManager().getLocalStack().getLocalKeySet();
		const NEIndexedKeySet* nullpointer = NE_NULL;		
		const NEEnlistableManager& manager = getManager();

		if (!&manager)			
			return *nullpointer;


		return manager.getKeySet();
	}

	type_result NEKeyBinder::_onEnlisted()
	{
		return RESULT_SUCCESS;
	}

	type_result NEKeyBinder::bind(const NEKey& key, NEType::Type manager_type)
	{
		using namespace NEType;
		//	pre:
// 		if (!isValidHierachy(NEENLISTABLE_MANAGER, manager_type) && ! isValidHierachy(NEType::LOCALSTACK, manager_type))
// 		{
// 			KERNAL_ERROR("�־��� Manager�� null �̰ų� NEEnlistableManager Ȥ�� LocalStack�� �ƴմϴ�.");
// 			goto ON_ERROR;
// 		}
		//_manager_type = manager_type;

		const NEIndexedKeySet& cont = _getKeySet();
		if( ! &cont)
		{
			KERNAL_ERROR("NEIndexedKeySet�� ������ �� �������ϴ�.");
			goto ON_ERROR;
		}
		const NEOccupiedSet& ocp_tbl = cont.getOccupiedSet();
		type_id src_id = key.getId();


		//	main:
		for(int n=0; n < cont.getSize() ;n++)
		{
			const NEKey& itr = cont[n];
			if( ! ocp_tbl[n]			||	//	�������� ���ų�
				! &itr					||	//	Key�� ���ų�
				itr.getId() != src_id	)	//	�־��� Id�� ��ġ���� ������
				continue;

			_real_index = n;
			_comparing_id = src_id;
			return RESULT_SUCCESS;
		}

ON_ERROR:
		KERNAL_ERROR("�־��� Key&, NEEnlistableManager& �� ���ε� �����߽��ϴ�.");
		return RESULT_TYPE_ERROR;
	}

	type_result NEKeyBinder::_markToBindLocalKey()
	{
		_real_index = NE_INDEX_ERROR;
		_comparing_id = NE_NULL;

		return RESULT_SUCCESS;
	}

	type_bool NEKeyBinder::isBindedLocalKey() const
	{
		return	_real_index == NE_INDEX_ERROR	&&
				_comparing_id == NE_NULL		;
	}
}