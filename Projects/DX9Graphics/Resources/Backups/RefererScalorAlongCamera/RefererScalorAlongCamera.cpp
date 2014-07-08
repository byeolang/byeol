#include "RefererScalorAlongCamera.hpp"
#include "../Referer/Referer.hpp"
#include "../../../../WindowModules/Sources/WindowModule/Sources/EntryPoints/Headers.hpp"

namespace DX9Graphics
{
	void RefererScalorAlongCamera::_computeCombinedMatrix(Referer& referer, Camera& camera)
	{
		//	pre:
		//		Ÿ����:		
		NE::Window& window = getDependent();
		if( ! &window)
		{
			ALERT_ERROR(" : ���콺 ������ ����!");
			return;
		}
		USHORT simultenoused_press = LOWORD(window.getMSG().wParam);
		type_int message = window.getMSG().message;		
		if(message != WM_MOUSEWHEEL) return;
		if(simultenoused_press & MK_RBUTTON) return;
		//			Z ���:		
		//				���:
		//				����� �������� ��Ŭ�� ������ ���� �ǹ��Ѵ�.
		//				0 ~ 120 ������ ���� ���´�.
		SHORT wheel_degree = HIWORD(window.getMSG().wParam);
		float referer_scale = D3DXVec3Length(&D3DXVECTOR3(referer.getScaleMatrix()._11, referer.getScaleMatrix()._12, referer.getScaleMatrix()._13));
		float click = wheel_degree > 0 ? 1.1f : 0.9f;
		if((GetAsyncKeyState(VK_SPACE)) & 0x8000)
			click = wheel_degree > 0 ? 1.3f : 0.7f;
		
		referer_scale *= click;
		//		��Ʈ���� ����:
		D3DXMatrixScaling(&referer.getScaleMatrix(), referer_scale, referer_scale, referer_scale);		
	}

	type_result RefererScalorAlongCamera::execute()
	{
		//	pre:
		//		Ÿ����:
		Camera& camera = _castCamera();
		if( ! &camera)
		{
			ALERT_ERROR(" : ī�޶� ����");
			return RESULT_TYPE_ERROR;
		}
		Referer& referer = Referer::getGlobalReferer();
		if( ! &referer) return RESULT_TYPE_ERROR;	



		//	main:
		//		����:
		_computeCombinedMatrix(referer, camera);

		//	post:
		return RESULT_SUCCESS;
	}
}