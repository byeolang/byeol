#include "NETEnlistableSet.hpp"

namespace NE
{
	typedef NENamedUnit T;
	typedef NETEnlistableSet<NENamedUnit> This;

	NETEnlistableSet<NENamedUnit>::NETEnlistableSet(const NECodeType& script_type, const NECodeType& name_type)
		: T(script_type, name_type), _manager(&NEGlobalManagerOffer::getGlobalManager()), _is_enlisted(false)
	{
		//	getGlobalManagerOnCopyConstructor�� ���Ͽ�:
		//		NEModuleArray.cpp�� ���� �ּ��� �����϶�
	}
	NETEnlistableSet<NENamedUnit>::NETEnlistableSet(const This& source)
		: T(source), _manager(&NEGlobalManagerOffer::getGlobalManager()), _is_enlisted(false)
	{
		//	getGlobalManagerOnCopyConstructor�� ���Ͽ�:
		//		NEModuleArray.cpp�� ���� �ּ��� �����϶�
	}

	const This& NETEnlistableSet<NENamedUnit>::operator=(const This& source)
	{
		/*
		_manager�� �������� �ʵ��� �ϱ� ���ؼ� operator=�� �����Ѵ�.
		*/
		T::operator=(source);

		return *this;
	}

	NEEnlistableManager& NETEnlistableSet<NENamedUnit>::getManager()
	{
		return *_manager;
	}
	const NEEnlistableManager& NETEnlistableSet<NENamedUnit>::getManager() const
	{
		return *_manager;
	}
	type_bool NETEnlistableSet<NENamedUnit>::isManaged() const
	{
		return _manager != NE_NULL;
	}
	type_bool NETEnlistableSet<NENamedUnit>::isEnlisted() const
	{
		return _is_enlisted;
	}
	NEBinaryFileSaver& NETEnlistableSet<NENamedUnit>::serialize(NEBinaryFileSaver& saver) const
	{
		Super::serialize(saver);

		return saver << _is_enlisted;
	}
	NEBinaryFileLoader& NETEnlistableSet<NENamedUnit>::serialize(NEBinaryFileLoader& loader)
	{
		Super::serialize(loader);

		return loader >> _is_enlisted;
	}
	type_result NETEnlistableSet<NENamedUnit>::_setManager(NEEnlistableManager& manager)
	{
		_manager = &manager;

		return RESULT_SUCCESS;
	}
	type_result NETEnlistableSet<NENamedUnit>::_setEnlisted(type_bool enlisted)
	{
		_is_enlisted = enlisted;

		return RESULT_SUCCESS;
	}
}