#pragma once

#include "../ShaderProgram/ShaderProgram.hpp"

namespace DX9Graphics
{
	class NE_DLL FixedProgram : public ShaderProgram
	{
	public:
		typedef ShaderProgram SuperClass;
		typedef FixedProgram ThisClass;

	public:
		NETArgument<NEBooleanKey>	arg_is_source_rendertargets;

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = "FixedProgram";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getComment() =
					"������ ���ŵ� ���� ���������� Shader���α׷� �Դϴ�.\n"
					"���� �⺻���� ���·� ��ü�� ����մϴ�.\n"
					"FinalRenderTarget�� PREVIOUS_BUFFER�� ����, ���� ���ۿ� ����� Surface�� �����Ͽ� �ٸ� ���۷� �ű�ϴ�.";
				"�� �̿��� �������� ���ۿ� ���� �׸��ϴ�.";
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2013-08-24";
				_header.getArgumentsComments() = SuperClass::getHeader().getArgumentsComments();				
			}

			return _header;
		}

	private:
		virtual type_result _onRender(DX9& dx9, Camera& camera)
		{
			//	pre:
			if(arg_final_render_target.getValue() != ShaderProgram::FINAL_RENDER_TARGET_PREVIOUS_BUFFER)
				return SuperClass::_onRender(dx9, camera);

			if( ! &dx9)
				return ALERT_ERROR("DX9 ���ε� ����. �۾��� �����մϴ�.");

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
				return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");

			device->BeginScene();

			device->SetTexture(0, &targets.getFilledTarget().getTexture());
			_renderTargetVertex(device);


			//	post:
			device->EndScene();
			_endFinalRenderTarget(targets);
			return RESULT_SUCCESS;
		}
		virtual LPCVOID _onRequestShaderCode(OUT type_int& size_of_binary) const { size_of_binary = 0; return 0; }
		virtual type_result _onSetShaderHandles(ShaderHandleSet&) { return RESULT_SUCCESS; }
	};
}