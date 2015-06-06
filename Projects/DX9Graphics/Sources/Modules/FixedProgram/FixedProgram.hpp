#pragma once

#include "../ShaderProgram/ShaderProgram.hpp"
#include "../Sprite/Sprite.hpp"
#include "../TabledTexture/TabledTexture.hpp"

namespace DX9Graphics
{
	class NE_DLL FixedProgram : public ShaderProgram
	{
	public:
		typedef ShaderProgram SuperClass;
		typedef FixedProgram ThisClass;

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
			type_result result = RESULT_SUCCESS;
			switch (arg_final_render_target)
			{
			case ShaderProgram::ONLY_ONE:
				if(NEResult::hasError(_standByFinalRenderTarget(dx9, ShaderProgram::READY_RENDER_TARGET_OUTPUT))) 
					return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");

				result = SuperClass::_onRender(dx9, camera);
				break;

			case ShaderProgram::FIRST:
				if(NEResult::hasError(_standByFinalRenderTarget(dx9, ShaderProgram::READY_RENDER_TARGET_NEW_BUFFER))) 
					return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");

				result = SuperClass::_onRender(dx9, camera);
				_endFinalRenderTarget(_getRenderTargetSet(dx9));
				break;

			case ShaderProgram::MIDDLE:
				if(NEResult::hasError(_standByFinalRenderTarget(dx9, ShaderProgram::READY_RENDER_TARGET_FILLED_BUFFER))) 
					return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");

				result = SuperClass::_onRender(dx9, camera);
				break;

			case ShaderProgram::LAST:
				{
					//	1-Pass:
					arg_final_render_target = ShaderProgram::MIDDLE;
					if(NEResult::hasError(_standByFinalRenderTarget(dx9, ShaderProgram::READY_RENDER_TARGET_FILLED_BUFFER))) 
						return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");
					result = SuperClass::_onRender(dx9, camera);

					//	2-Pass:
					arg_final_render_target = ShaderProgram::LAST;
					if( ! &dx9)
						return ALERT_ERROR("DX9 ���ε� ����. �۾��� �����մϴ�.");
					LPDIRECT3DDEVICE9 device = dx9.getDevice();
					RenderTargetSet& targets = _getRenderTargetSet(dx9);
					if(targets.getSize() <= 0)
						return ALERT_ERROR("DX9 ���ε� ���з� ����Ÿ���� �������� ���߽��ϴ�.");

					//			Matrixġȯ:
					D3DXMATRIX new_w, new_v, new_p = _getOrthoMatrix();
					D3DXMatrixIdentity(&new_w);
					D3DXMatrixLookAtLH(&new_v, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
					device->SetTransform(D3DTS_WORLD, &new_w);
					device->SetTransform(D3DTS_VIEW, &new_v);
					device->SetTransform(D3DTS_PROJECTION, &new_p);

					//	main:
					if(NEResult::hasError(_standByFinalRenderTarget(dx9, ShaderProgram::READY_RENDER_TARGET_OUTPUT)))
						return ALERT_ERROR("���� ����Ÿ���� �����ϴ�");
					device->BeginScene();
					device->SetTexture(0, &targets.getFilledTarget().getTexture());

					device->SetRenderState(D3DRS_ZENABLE, FALSE);					
					_renderTargetVertex(device);
					_endFinalRenderTarget(targets);
					device->SetRenderState(D3DRS_ZENABLE, TRUE);

					//	post:
					device->EndScene();
				}			
				break;
			}

			return result;
		}
		virtual LPCVOID _onRequestShaderCode(OUT type_int& size_of_binary) const { size_of_binary = 0; return 0; }
		virtual type_result _onSetShaderHandles(ShaderHandleSet&) { return RESULT_SUCCESS; }
	};
}