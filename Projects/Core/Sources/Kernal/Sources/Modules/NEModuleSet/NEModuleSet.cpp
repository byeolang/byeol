#include "NEModuleSet.hpp"

namespace NE
{
	NEModuleSet::NEModuleSet()
		: SuperClass() 
	{
	}
	NEModuleSet::NEModuleSet(type_count size)
		: SuperClass(size)
	{

	}
	NEModuleSet::NEModuleSet(const NEModuleSet& rhs)
		: SuperClass(rhs)
	{

	}
	NEModuleSet::~NEModuleSet()
	{

	}
	type_index NEModuleSet::insert(type_index index, const NEModule* const source)
	{
		type_index result_index = SuperClass::insert(index, source);
		if(result_index == NE_INDEX_ERROR)
			return result_index;

		_updateModule(result_index);

		return result_index;
	}
	type_result NEModuleSet::setElement(type_index index, const NEModule* const source)
	{
		type_result result = SuperClass::setElement(index, source);
		if( ! NEResult::hasError(result))
			_updateModule(index);

		return result;
	}

	void NEModuleSet::_updateModule(type_index index)
	{
		//		����:
		NEModule& pushed = getElement(index);
		pushed._scriptcode.create(1);
		pushed._scriptcode.push(NECode(index, NECodeType::SCRIPT));

		pushed._onFetchModule();
	}
}
