#include "NEExecutableUnit.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEExecutableUnit::NEExecutableUnit()
		: NEUnit()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEExecutableUnit::NEExecutableUnit(const NEExecutableUnit& source)
		: NEUnit(source)
	{
		_assign(source);
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

		NEUnit::operator=(source);

		return _assign(source);		
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEExecutableUnit::operator==(const NEExecutableUnit& source) const 
	{
		if(this == &source) return true;
		if(_enable != source._enable) return false;

		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEExecutableUnit::operator!=(const NEExecutableUnit& source) const 
	{
		return !(operator==(source));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEExecutableUnit::isEnable() const
	{
		return _enable;
	}



	void NE_DLL NEExecutableUnit::setEnable(bool enable)
	{
		_enable = enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NE_DLL NEExecutableUnit::release() 
	{
		NEUnit::release();

		return _release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver NE_DLL &NEExecutableUnit::serialize(NEBinaryFileSaver& saver) const 
	{
		NEUnit::serialize(saver);

		return saver << _enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader NE_DLL &NEExecutableUnit::serialize(NEBinaryFileLoader& loader)
	{
		NEUnit::serialize(loader);

		return loader >> _enable;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEExecutableUnit::_release()
	{
		_enable = true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEExecutableUnit& NEExecutableUnit::_assign(const NEExecutableUnit& source)
	{
		if(this == &source) return *this;

		_enable = source._enable;

		return *this;
	}

	NEType::Type NE_DLL NEExecutableUnit::getType() const
	{
		return NEType::NEEXCUTABLE_UNIT;
	}
}