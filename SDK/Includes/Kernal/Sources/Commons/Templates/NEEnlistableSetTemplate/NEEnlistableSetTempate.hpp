#pragma once

#include "../../Interfaces/NEOnEnlisted/NEOnEnlisted.hpp" 
#include "../../Units/NEGlobalManagerOffer/NEGlobalManagerOffer.hpp"

namespace NE
{
	class NEEnlistableManager;

	template <typename T>
	class NE_DLL NEEnlistableSetTemplate : public T, public NEOnEnlisted
	{
	public:
		typedef NEEnlistableSetTemplate<T> ThisClass;
		typedef T SuperClass;

	public:
		NEEnlistableSetTemplate()
			: T(), _manager(&NEGlobalManagerOffer::getGlobalManager()), _is_enlisted(false)
		{
			//	getGlobalManagerOnCopyConstructor�� ���Ͽ�:
			//		NEIndexedModuleSet.cpp�� ���� �ּ��� �����϶�
		}
		NEEnlistableSetTemplate(NEEnlistableManager& manager)
			: T(), _manager(&manager), _is_enlisted(false)
		{

		}
		NEEnlistableSetTemplate(type_index size)
			: T(size), _manager(&NEGlobalManagerOffer::getGlobalManager()), _is_enlisted(false)
		{

		}

	public:		
		NEEnlistableSetTemplate(const ThisClass& source)
			: T(source), _manager(&NEGlobalManagerOffer::getGlobalManager()), _is_enlisted(false)
		{
			//	getGlobalManagerOnCopyConstructor�� ���Ͽ�:
			//		NEIndexedModuleSet.cpp�� ���� �ּ��� �����϶�
		}

	public:
		const ThisClass& operator=(const ThisClass& source)
		{
			/*
				_manager�� �������� �ʵ��� �ϱ� ���ؼ� operator=�� �����Ѵ�.
			*/
			T::operator=(source);

			return *this;
		}

	public:
		NEEnlistableManager& getManager()
		{
			return *_manager;
		}
		const NEEnlistableManager& getManager() const
		{
			return *_manager;
		}
		bool isManaged() const
		{
			return _manager != NE_NULL;
		}
		bool isEnlisted() const
		{
			return _is_enlisted;
		}

	public:
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
		{
			SuperClass::serialize(saver);

			return saver <<_is_enlisted;
		}
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
		{
			SuperClass::serialize(loader);

			return loader >> _is_enlisted;
		}

	protected:
		type_result _setManager(NEEnlistableManager& manager)
		{
			_manager = &manager;

			return RESULT_SUCCESS;
		}
		type_result _setEnlisted(bool enlisted)
		{
			_is_enlisted = enlisted;

			return RESULT_SUCCESS;
		}

	protected:
		NEEnlistableManager* _manager;
		bool _is_enlisted;
	};	
}