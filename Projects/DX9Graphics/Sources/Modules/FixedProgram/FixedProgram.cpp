#include "FixedProgram.hpp"
#include "define.hpp"
#include "../Model/Model.hpp"

namespace DX9Graphics
{
	const NEExportable::ModuleHeader& FixedProgram::getHeader() const
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

	type_result FixedProgram::_onRender(DX9& dx9, Camera& camera)
	{
		//	pre:
		if( ! isInputRenderTarget())
			return SuperClass::_onRender(dx9, camera);
		if( ! &dx9)
		{
			ALERT_ERROR("DX9 ���ε� ����. �۾��� �����մϴ�.");
			return RESULT_TYPE_ERROR;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();
		//		RenderTarget�� Quad ���·� ����ϱ� ���� �غ�:
		//			Matrixġȯ:
		D3DXMATRIX new_w, new_v, new_p = _getOrthoMatrix();
		D3DXMatrixIdentity(&new_w);
		D3DXMatrixLookAtLH(&new_v, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
		device->SetTransform(D3DTS_WORLD, &new_w);
		device->SetTransform(D3DTS_VIEW, &new_v);
		device->SetTransform(D3DTS_PROJECTION, &new_p);
		//			Effect ��������:
		ID3DXEffect& effect = getEffect();
		//			RenderTargetSet ��������:
		RenderTargetSet& targets = _getRenderTargetSet(dx9);


		//	main:
		//		Output RenderTarget �غ�:
		if(NEResult::hasError(_standByFinalRenderTarget(dx9))) 
		{
			ALERT_ERROR("���� ����Ÿ���� �����ϴ�");
			return RESULT_TYPE_ERROR;
		}
		device->BeginScene();
		
		device->SetTexture(0, &targets.getFilledTarget().getTexture());
		_renderTargetVertex(device);			
		

		//	post:
		device->EndScene();
		_endFinalRenderTarget(targets);
		return RESULT_SUCCESS;
	}
}