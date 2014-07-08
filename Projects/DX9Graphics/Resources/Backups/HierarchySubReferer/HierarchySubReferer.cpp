#include "HierarchySubReferer.hpp"
#include "../PerspectiveCamera/PerspectiveCamera.hpp"

namespace DX9Graphics
{
	type_result HierarchySubReferer::_render( D3DMATERIAL9& material )
	{
		_initializeMesh();
		//	main:
		//		DX9 ���ε�:
		DX9& dx9 = getDependent();
		if( ! &dx9)
		{
			ALERT_ERROR(" : ����̽� ���ε� ���з� ����");
			return RESULT_TYPE_ERROR;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();		
		//		������ǥ ����:
		D3DXMATRIX& world = getWorldMatrix();
		D3DXMATRIX origin;
		device->GetTransform(D3DTS_WORLD, &origin);
		device->SetTransform(D3DTS_WORLD, &world);

		device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		//device->SetTexture(0, NULL);
		device->SetMaterial(&material);
		device->SetTexture(0, 0);
		if(_ball)
		{			
			_ball->DrawSubset(0);
		}

		//		�ڽİ� ���� �ձ�:
		device->SetTransform(D3DTS_WORLD, &origin);

		_searchModuleSet(getConnector().getModuleCodeSetKey().getValue(), &ThisClass::_renderLineBetweenChild);			//		���� ��ǥ ����:
		return RESULT_SUCCESS;
	}
}