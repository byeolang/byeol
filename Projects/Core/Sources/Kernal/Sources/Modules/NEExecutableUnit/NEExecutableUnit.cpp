#include "NEExecutableUnit.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEExecutableUnit::NEExecutableUnit()
		: SuperClass()
	{
		
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEExecutableUnit::NEExecutableUnit(const NEExecutableUnit& source)
		: SuperClass(source)
	{
		
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEExecutableUnit::~NEExecutableUnit()
	{

	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEExecutableUnit NE_DLL &NEExecutableUnit::operator=(const NEExecutableUnit& source) 
	{
		if(this == &source) return *this;

		SuperClass::operator=(source);

		return *this;	
	}



	NEType::Type NE_DLL NEExecutableUnit::getType() const
	{
		return NEType::NEEXCUTABLE_UNIT;
	}
}