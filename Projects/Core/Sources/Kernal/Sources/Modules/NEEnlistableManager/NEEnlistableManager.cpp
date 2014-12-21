#include "NEEnlistableManager.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
/*
	C4355��� ����:
		��� ������ ����:
			�Ϲ�������, this �����͸� �ѱ��, �ѱ��ʿ��� �����ڰ� �ʱ�ȭ����
			�ʾҴµ���, �Ѱܹ��� �ʿ��� ����� ������ �ֱ� �����̴�.
		�����ϴ� ����:
			�Ʒ��� ���� ���, ����� "�����ڿ��� ȣ�� ���� ����"�Ƿ� ����Ѵ�.
*/
#pragma warning(push)	
#pragma warning(disable:4355)
	NEEnlistableManager::NEEnlistableManager()
		: SuperClass(), _nodeset(*this), _moduleset(*this), _keyset(*this)
	{
		_release();
	}

	NEEnlistableManager::NEEnlistableManager(const NEEnlistableManager& source)
		: SuperClass(source), _nodeset(*this), _moduleset(*this), _keyset(*this)
	{
		_assign(source);
	}
#pragma warning(pop)

	NEType::Type NEEnlistableManager::getType() const
	{
		return NEType::NEENLISTABLE_MANAGER;
	}

	type_result NEEnlistableManager::_enlist(NENode& target, type_index index/*= NE_INDEX_ERROR*/)
	{
		if(index <= NE_INDEX_ERROR) 
			index = _searchRealNodeIndex(target);



		type_result result = _getShortCutSet(NECodeType::SCRIPT)._enlist(target, index);
		result |= _getShortCutSet(NECodeType::NAME)._enlist(target, index);
		result |= _getShortCutSet(NECodeType::GROUP)._enlist(target, index);
		return result |= _getShortCutSet(NECodeType::PRIORITY)._enlist(target, index);
	}

	type_result NEEnlistableManager::_unlist(NENode& target, type_index index/*= NE_INDEX_ERROR*/)
	{
		if(index <= NE_INDEX_ERROR) 
			index = _searchRealNodeIndex(target);



		type_result result = _getShortCutSet(NECodeType::SCRIPT)._unlist(target, index);
		result |= _getShortCutSet(NECodeType::NAME)._unlist(target, index);
		result |= _getShortCutSet(NECodeType::GROUP)._unlist(target, index);
		result |= _getShortCutSet(NECodeType::PRIORITY)._unlist(target, index);

		target._manager = NE_NULL;

		return result;
	}

	NEIndexedKeySet& NEEnlistableManager::_getKeySet()
	{
		return _keyset;
	}

	const NEIndexedKeySet& NEEnlistableManager::getKeySet() const
	{
		return _keyset;
	}

	NEIndexedModuleSet& NEEnlistableManager::_getModuleSet()
	{
		return _moduleset;
	}

	const NEIndexedModuleSet& NEEnlistableManager::getModuleSet() const
	{
		return _moduleset;
	}

	NEIndexedNodeSet& NEEnlistableManager::_getNodeSet()
	{
		return _nodeset;
	}

	const NEIndexedNodeSet& NEEnlistableManager::getNodeSet() const
	{
		return _nodeset;
	}

	void NEEnlistableManager::release()
	{
		SuperClass::release();
	
		_release();
	}

	type_result NEEnlistableManager::isValid() const
	{
		type_result result = SuperClass::isValid();
		result |= _priority_shortcutset.isValid();
		result |= _group_shortcutset.isValid();
		result |= _script_shortcutset.isValid();
		result |= _name_shortcutset.isValid();
		result |= _nodeset.isValid();
		result |= _moduleset.isValid();
		result |= _keyset.isValid();

		return result;
	}

	NEBinaryFileLoader& NEEnlistableManager::serialize(NEBinaryFileLoader& loader)
	{
		SuperClass::serialize(loader);
		
		loader	>> _priority_shortcutset >> _script_shortcutset >> _group_shortcutset >> _name_shortcutset >> _nodeset >> _moduleset >> _keyset;
		
		return loader;
	}

	NEBinaryFileSaver& NEEnlistableManager::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);

		return saver	<< _priority_shortcutset << _script_shortcutset << _group_shortcutset << _name_shortcutset << _nodeset << _moduleset << _keyset;
	}

	void NEEnlistableManager::_setScriptCode(NENode& target, type_code new_scriptcode) const
	{
		target._scriptcode = new_scriptcode;
	}

	void NEEnlistableManager::_setNameCode(NENode& target, type_code new_namecode) const
	{
		target._namecode = new_namecode;
	}

	void NEEnlistableManager::_setGroupCode(NENode& target, const NECodeSet& new_groupcode) const
	{
		target._groupcodeset = new_groupcode;
	}

	void NEEnlistableManager::_setPriorityCode(NENode& target, type_code new_priority) const
	{
		target._priority = new_priority;
	}

	type_index NEEnlistableManager::_searchRealNodeIndex(NENode& target) const
	{
		const NEIndexedNodeSet& nodeset = getNodeSet();

		for(int n=0; n < nodeset.getSize() ;n++)
		{
			const NENode& node = nodeset.getElement(n);
			if(&node == &target)
				return n;
		}

		return NE_INDEX_ERROR;
	}

	type_result NEEnlistableManager::_onChangeNameCode(NENode& target, type_code new_namecode)
	{
		if(target.getNameCode() == new_namecode) return RESULT_SUCCESS | RESULT_ABORT_ACTION;



		type_index real_index = _searchRealNodeIndex(target);
		NEShortCutSet& shortcutset = _getNameShortCutSet();
		type_result result = shortcutset._unlist(target, real_index);
		if(NEResult::hasError(result))
		{
			ALERT_ERROR(" : unlist���� ������ �߻��߽��ϴ�.\n\t�����ڵ� : %d", result);
			return result;
		}

		_setNameCode(target, new_namecode);

		return shortcutset._enlist(target, real_index);
	}

	type_result NEEnlistableManager::_onChangePriorityCode(NENode& target, type_code new_prioritycode)
	{
		if(target.getPriority() == new_prioritycode) return RESULT_SUCCESS | RESULT_ABORT_ACTION;



		type_index real_index = _searchRealNodeIndex(target);
		NEShortCutSet& shortcutset = _getPriorityShortCutSet();
		type_result result = shortcutset._unlist(target, real_index);
		if(NEResult::hasError(result))
		{
			ALERT_ERROR(" : ");
			return result;
		}

		_setPriorityCode(target, new_prioritycode);

		return shortcutset._enlist(target, real_index);
	}

	type_result NEEnlistableManager::_onChangeGroupCode(NENode& target, const NECodeSet& new_groupcodeset)
	{
		if(target.getGroupCode() == new_groupcodeset) return RESULT_SUCCESS | RESULT_ABORT_ACTION;



		type_index real_index = _searchRealNodeIndex(target);
		NEShortCutSet& shortcutset = _getGroupShortCutSet();
		type_result result = shortcutset._unlist(target, real_index);
		if(NEResult::hasError(result))
		{
			ALERT_ERROR(" : ");
			return result;
		}

		_setGroupCode(target, new_groupcodeset);

		return shortcutset._enlist(target, real_index);
	}

	type_result NEEnlistableManager::_createShortCutSetAndFillWithShortCuts(NEShortCutSet& shortcutset, type_index size)
	{
		if( ! &shortcutset)
		{
			KERNAL_ERROR(" : ���Ƽ��� �����ϴ�");
			return RESULT_SUCCESS;
		}



		//	main:
		type_result result = RESULT_SUCCESS;
		result |= shortcutset.create(size);
		for(int n=0; n < shortcutset.getSize() ;n++)
			result |= shortcutset.push(NEShortCut());

		return result;
	}

	void NEEnlistableManager::_release()
	{
		_nodeset.release();
		_moduleset.release();		
		_keyset.release();
		_priority_shortcutset.release();
		_script_shortcutset.release();
		_group_shortcutset.release();
		_name_shortcutset.release();		
	}

	const NEEnlistableManager& NEEnlistableManager::operator=(const ThisClass& source)
	{
		SuperClass::operator=(source);

		return _assign(source);
	}

	const NEEnlistableManager& NEEnlistableManager::_assign(const ThisClass& source)
	{
		if(this == &source) return *this;

		_priority_shortcutset = source._priority_shortcutset;
		_script_shortcutset = source._script_shortcutset;
		_group_shortcutset = source._group_shortcutset;
		_name_shortcutset = source._name_shortcutset;
		_nodeset = source._nodeset;
		_moduleset = source._moduleset;
		_keyset = source._keyset;

		return *this;
	}

	type_result NEEnlistableManager::initialize()
	{
		return RESULT_SUCCESS;
	}

	NEEnlistableManager::~NEEnlistableManager()
	{

	}

	bool NEEnlistableManager::operator==(const ThisClass& source) const
	{
		if(SuperClass::operator!=(source)) return false;
		if(	_keyset != source._keyset											||
			_priority_shortcutset != source._priority_shortcutset				||
			_script_shortcutset != source._script_shortcutset					||
			_group_shortcutset != source._group_shortcutset						||
			_name_shortcutset != source._name_shortcutset						||
			_nodeset != source._nodeset											||
			_moduleset != source._moduleset										)
			return false;
		
		return true;
	}

	bool NEEnlistableManager::operator!=(const ThisClass& source) const
	{
		return ! operator==(source);
	}

	NEShortCutSet& NEEnlistableManager::_getShortCutSet(const NECodeType& type)
	{
		NEShortCutSet* nullpointer = 0x00;
		if( ! &type) 
			return *nullpointer;


		//	main:
		switch(type)
		{
		case NECodeType::SCRIPT:	return _script_shortcutset;
		case NECodeType::NAME:		return _name_shortcutset;
		case NECodeType::GROUP:		return _group_shortcutset;
		}

		return *nullpointer;
	}

	const NEShortCutSet& NEEnlistableManager::getShortCutSet(const NECodeType& type) const
	{
		ThisClass* unconsted = const_cast<ThisClass*>(this);

		return unconsted->_getShortCutSet(type);
	}

	void NEEnlistableManager::_setCode(NENode& target, const NECodeSet& new_codes) const
	{
		target._setCode(new_codes)
	}
}