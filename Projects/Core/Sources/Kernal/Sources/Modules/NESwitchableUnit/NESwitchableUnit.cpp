#include "NESwitchableUnit.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NESwitchableUnit::NESwitchableUnit()
		: NEUnit()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NESwitchableUnit::NESwitchableUnit(const NESwitchableUnit& source)
		: NEUnit(source)
	{
		_assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NESwitchableUnit::~NESwitchableUnit()
	{

	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NESwitchableUnit NE_DLL &NESwitchableUnit::operator=(const NESwitchableUnit& source) 
	{
		if(this == &source) return *this;

		NEUnit::operator=(source);

		return _assign(source);		
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NESwitchableUnit::operator==(const NESwitchableUnit& source) const 
	{
		if(this == &source) return true;
		if(_enable != source._enable) return false;

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NESwitchableUnit::operator!=(const NESwitchableUnit& source) const 
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NESwitchableUnit::isEnable() const
	{
		return _enable;
	}



	void NE_DLL NESwitchableUnit::setEnable(bool enable)
	{
		_enable = enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NE_DLL NESwitchableUnit::release() 
	{
		NEUnit::release();

		return _release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver NE_DLL &NESwitchableUnit::serialize(NEBinaryFileSaver& saver) const 
	{
		NEUnit::serialize(saver);

		return saver << _enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader NE_DLL &NESwitchableUnit::serialize(NEBinaryFileLoader& loader)
	{
		NEUnit::serialize(loader);

		return loader >> _enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NESwitchableUnit::_release()
	{
		_enable = true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NESwitchableUnit& NESwitchableUnit::_assign(const NESwitchableUnit& source)
	{
		if(this == &source) return *this;

		_enable = source._enable;

		return *this;
	}

	NEType::Type NE_DLL NESwitchableUnit::getType() const
	{
		return NEType::NESWITCHABLE_UNIT;
	}
}