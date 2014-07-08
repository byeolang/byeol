#include "CameraManipulator.hpp"
#include "../Camera/Camera.hpp"

namespace DX9Graphics
{
	const NEExportable::ModuleHeader& DX9Graphics::CameraManipulator::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("CameraManipulator");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. CameraSelector\n1��. �̵� Ÿ��(0:yaw, 1:pitch, 2:roll, 3:forward, 4:step aside, 5:go up)\n2��. ����");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	type_result CameraManipulator::execute()
	{
		type_byte type = static_cast<NEByteKey&>(getKeySet()[1]).getValue();
		type_float degree = static_cast<NEFloatKey&>(getKeySet()[2]).getValue();


		//	main:
		NEModuleSelector& sel = getCameraSelector();
		Camera* itr = 0;
		while( itr = static_cast<Camera*>(&sel.getModule()))
		{
			switch(type)
			{
			case 0:
				return itr->yaw(degree);
			case 1:
				return itr->pitch(degree);
			case 2:
				return itr->roll(degree);
			case 3:
				return itr->forward(degree);
			case 4:
				return itr->stepAside(degree);
			case 5:
				return itr->goUp(degree);

			default:
				ALERT_ERROR(" : �ùٸ��� ���� Ÿ��.");
				sel.initializeReferingPoint();
				return RESULT_TYPE_ERROR;
			}
		}

		return RESULT_SUCCESS;
	}
}