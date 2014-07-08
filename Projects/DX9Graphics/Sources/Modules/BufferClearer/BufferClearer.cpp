#include "BufferClearer.hpp"
#include "define.hpp"
#include "../DX9/DX9.hpp"

namespace DX9Graphics
{
	NEExportable::ModuleHeader& BufferClearer::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T(_NAME);
			_header.getDeveloper() = _T(_DEVELOPER);
			_header.setRevision(_REVISION);
			_header.getComment() = _T(_COMMENT);
			_header.getVersion()  = _T(_VERSION);
			_header.getReleaseDate() = _T(_DATE);
			_header.getModuleDependencies().create(_DEPENDENCIES_COUNT);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	type_result BufferClearer::initialize()
	{
		SuperClass::initialize();

		NEKeyCodeSet& keyset = getKeySet();
		keyset.resize(keyset.getLength() + _KEY_COUNT);		
		keyset.push(NEUIntKey(_DEFAULT_COLOR));
		keyset.push(NEUIntKey(_DEFAULT_FLAG));

		return RESULT_SUCCESS;
	}

	type_result BufferClearer::execute()
	{
		//	main:
		DX9& dx9 = getBinded();
		if( ! &dx9)
		{
			ALERT_ERROR(" : ����̽��� �ʱ�ȭ ���� �ʾ����Ƿ� �۾��� ����մϴ�.");
			return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();		
		//		Ÿ����:
		NEUIntKey& color = static_cast<NEUIntKey&>(getKeySet()[1]);
		NEUIntKey& flag = static_cast<NEUIntKey&>(getKeySet()[2]);
		if(FAILED(device->Clear(0, NULL, flag.getValue(), color.getValue(), 1.0f, 0)))
		{
			ALERT_ERROR(" : ���� Ŭ���� ����");

			return RESULT_TYPE_ERROR;
		}
		
		

		//	post:
		return RESULT_SUCCESS;
	}

	NEObject& BufferClearer::clone() const
	{
		return *(new BufferClearer(*this));
	}
}