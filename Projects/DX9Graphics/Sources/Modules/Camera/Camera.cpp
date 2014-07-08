#include "Camera.hpp"
#include "../DX9/DX9.hpp"
#include "../PerspectiveCamera/PerspectiveCamera.hpp"
#include <math.h>

namespace DX9Graphics
{
	type_result Camera::_applyViewMatrix()
	{
		const D3DXMATRIX& view_matrix = getViewMatrix();
		DX9& dx9 = _getBinded();
		if( ! &dx9)
		{
			ALERT_ERROR(" : DX9�� ���ε� �Ǿ����� �����Ƿ� �۾��� ����մϴ�.");
			return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();
		


		//	post:		
		device->SetTransform(D3DTS_VIEW, &view_matrix);
		return RESULT_SUCCESS;
	}

	void Camera::_setModuleCodeSet(NEModuleSelector& selector)
	{	
		selector.setModuleCodeSet(getCameraCodeSet());
		selector.setModuleType(NECodeType::SCRIPT);
	}

	type_result Camera::_setViewPort()
	{
		DX9& dx9 = _getBinded();
		if( ! &dx9)
		{
			ALERT_ERROR(" : DX9�� ���ε� �Ǿ����� �����Ƿ� �۾��� ����մϴ�.");
			return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();



		//	main:
		if(isViewportMaximized())
			device->SetViewport(&createMaximizedViewPort());
		else
			device->SetViewport(&createViewPort());
		return RESULT_SUCCESS;
	}

	const NECodeSet& Camera::getCameraCodeSet()
	{
		static NECodeSet codeset;
		if(codeset.getLength() <= 0)
		{
			NEModuleManager& moduler = Kernal::getInstance().getModuleManager();
			codeset.create(1);
			codeset.setCodeType(NECodeType::SCRIPT);
			codeset.push(PerspectiveCamera().getHeader());
		}

		return codeset;
	}

	DX9& Camera::_getBinded()
	{
		NEModuleSelector& sel = getDX9Selector();

		if( ! sel.getBinder().isBinded())
		{			
			static type_code dx9_code = Kernal::getInstance().getModuleManager()
				.getModule(DX9().getHeader()).getScriptCode();

			while(NEModule* itr = &sel.getModule())
				if(itr->getScriptCode() == dx9_code)
				{
					sel.bind();
					sel.initializeReferingPoint();
					goto POST;
				}

			ALERT_WARNING("DX9 ���ε� ����");
		}

POST:
		return (DX9&) sel.getBinder().getBinded();
	}

	const DX9& Camera::_getBinded() const
	{
		return (const DX9&) getDX9Selector().getBinder().getBinded();
	}

	void Camera::EulerAngles::fromVector(const D3DXVECTOR3& look)
	{
		//	main:
		//		��ǥ ���� �˰���:
		//			Euler->XYZ Coordinates ������ arcsin�� ����ؼ� �����Ѵ�.
		//			�ﰢ�Լ��� �񰡿��̹Ƿ�, asin�� ������ ���ǿ��� ������ ����.
		//			���⼭�� 0 ~ 2PI�� ����(�����δ� -PI/2 ~ 3/2PI)�ϹǷ� ������
		//			���ǿ��� 2 or 3���� ����ȴ�. (asin(0)�Ҷ� 3��)
		//			�׷��� asin�Ҷ����� ��� ����� ���� Euler->XYZ ���Ŀ� ����
		//			�Ͽ� �ùٸ� ���� ����Ǵ��� Ȯ���Ѵ�.				
		type_float	x_rad = 0.0f,
			y_rad = 0.0f,
			cosx = 0.0f;

		//		�õ� 1:
		x_rad = asinf(-look.y);
		//			�õ� 1-1:
		y_rad = asinf(look.x / cosf(x_rad));
		if(_isPredictionOfEulerConversionRight(x_rad, y_rad, look.z))
			goto POST;
		//			�õ� 1-2:
		y_rad = D3DX_PI - y_rad;
		if(_isPredictionOfEulerConversionRight(x_rad, y_rad, look.z))
			goto POST;

		//		�õ� 2:
		x_rad = D3DX_PI - x_rad;
		//			�õ� 2-1:
		y_rad = asinf(look.x / cosf(x_rad));
		if(_isPredictionOfEulerConversionRight(x_rad, y_rad, look.z))
			goto POST;
		//			�õ� 2-2:
		y_rad = D3DX_PI - y_rad;
		if(_isPredictionOfEulerConversionRight(x_rad, y_rad, look.z))
			goto POST;

		KERNAL_ERROR("�߸��� XYZ Coordinates --> EulerAngles ��ȯ�Դϴ�");


		//	post:
POST:
		const type_float adjustment = 180.0f / D3DX_PI;
		x_axis_angle = x_rad * adjustment;
		y_axis_angle = y_rad * adjustment;
	}

	bool Camera::EulerAngles::_isPredictionOfEulerConversionRight(type_float predicted_x_radian, type_float predicted_y_radian, type_float right_answer) const
	{
		//	���� ����:
		//		���� : z-coordinate = cosx * cosy��, Z ��ǥ�� �ٽ� �˻��غ����ν�
		//		�־��� ������ �´��� Ȯ���Ѵ�.
		type_float z = cosf(predicted_x_radian) * cosf(predicted_y_radian);
		return _isEqual(z, right_answer);
	}
}