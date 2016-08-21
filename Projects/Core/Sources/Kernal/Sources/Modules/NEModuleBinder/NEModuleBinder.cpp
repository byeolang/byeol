#include "NEModuleBinder.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	type_bool NEModuleBinder::isBinded() const
	{
		const NEIndexedModuleSet& moduleset = _getModuleSet();
		if( ! &moduleset)	return false;
		const NEModule& node = moduleset[getRealIndex()];
		if( ! &node) return false;

		return node.getId() == getComparingId();
	}

	NEModule& NEModuleBinder::getBinded()
	{
		NEModule* nullpointer = NE_NULL;
		if( ! isBinded())
			return *nullpointer;

		return _getModuleSet()[getRealIndex()];
	}

	const NEModule& NEModuleBinder::getBinded() const
	{
		const NEModule* nullpointer = NE_NULL;
		if( ! isBinded())
			return *nullpointer;

		return _getModuleSet()[getRealIndex()];
	}

	NEIndexedModuleSet& NEModuleBinder::_getModuleSet()
	{
		NEIndexedModuleSet* nullpointer = NE_NULL;
		NEEnlistableManager& manager = getManager();

		if( ! &manager)
			return *nullpointer;

		return manager._getModuleSet();
	}

	const NEIndexedModuleSet& NEModuleBinder::_getModuleSet() const
	{
		const NEIndexedModuleSet* nullpointer = NE_NULL;
		const NEEnlistableManager& manager = getManager();

		if( ! &manager)
			return *nullpointer;

		return manager.getModuleSet();
	}

	type_result NEModuleBinder::bind(const NEModule& module/*, NEType::Type manager_type*/)
	{
		/*using namespace NEType;*/
		//	pre:
		/*if( ! isValidHierachy(NEENLISTABLE_MANAGER, manager_type))
		{

			KERNAL_ERROR("�־��� Manager�� null �̰ų� NEEnlistableManager�� �ƴմϴ�.");
			goto ON_ERROR;
		}
		_manager_type = manager_type;*/

		NEIndexedModuleSet& cont = _getModuleSet();
		if( ! &cont)
		{
			KERNAL_ERROR("NEIndexedModuleSet�� ������ �� �������ϴ�.");
			goto ON_ERROR;
		}
		const NEOccupiedSet& ocp_tbl = cont.getOccupiedSet();
		type_id src_id = module.getId();


		//	main:
		for(int n=0; n < cont.getSize() ;n++)
		{
			NEModule& itr = cont[n];
			if( ! ocp_tbl[n]			||	//	�������� ���ų�
				! &itr					||	//	Node�� ���ų�
				itr.getId() != src_id	)	//	�־��� Id�� ��ġ���� ������
				continue;

			_real_index = n;
			_comparing_id = src_id;
			return RESULT_SUCCESS;
		}

ON_ERROR:
		KERNAL_ERROR("�־��� Module&, NEEnlistableManager& �� ���ε� �����߽��ϴ�.");
		return RESULT_TYPE_ERROR;
	}

	NEModuleBinder::NEModuleBinder()
		: SuperClass()
	{

	}

	NEModuleBinder::NEModuleBinder(const NEModule& module/*, NEType::Type manager_type*/)
		: SuperClass()
	{
		bind(module/*, manager_type*/);
	}

}