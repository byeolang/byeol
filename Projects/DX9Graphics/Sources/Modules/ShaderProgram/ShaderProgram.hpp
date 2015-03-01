#pragma once

#include "../Resource/Resource.hpp"
#include "../DX9/DX9.hpp"
#include "../../Commons/Units/RenderTarget/RenderTarget.hpp"
#include "../../Commons/Units/RenderTargetSet/RenderTargetSet.hpp"
#include "../../Commons/Units/ShaderHandle/ShaderHandle.hpp"
#include "../../Commons/Units/ShaderHandleSet/ShaderHandleSet.hpp"	

namespace DX9Graphics
{
	class Model;
	class Camera;

	class NE_DLL ShaderProgram : public Resource
	{
	public:
		typedef Resource SuperClass;
		typedef ShaderProgram ThisClass;

	public:
		enum ERenderTarget
		{
			FINAL_RENDER_TARGET_OUTPUT			= 0,
			FINAL_RENDER_TARGET_NEW_BUFFER		= 1,
			FINAL_RENDER_TARGET_PREVIOUS_BUFFER	= 2,
			FINAL_RENDER_TARGET_NEW_OUTPUT		= 3
		};
		static const int RENDER_TARGET_VERTEX_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	public:
		friend class Camera;
		friend class DX9;

	public:
		NETArgument<NEFloatKey>	arg_width_rate;
		type_int arg_final_render_target;
		NETArgument<NEFloatKey>	arg_height_rate;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_width_rate);
			tray.push(arg_height_rate);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_width_rate.setValue(1.0f);
			arg_height_rate.setValue(1.0f);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			return RESULT_SUCCESS;	//	ShaderProgram�� ��ü��, render() �� �ʿ��ϴ�.
		}
		virtual type_result _onRender(DX9& dx9, Camera& camera);
		virtual type_result _render(Camera& camera)
		{
			//	pre:
			DX9& dx9 = DX9::getInstancedDX();
			if (!&dx9)
				return KERNAL_ERROR("DX9 ���ε� ���з� ���̴� ���α׷��� ������ �� �� �����ϴ�");

			LPDIRECT3DDEVICE9 device = dx9.getDevice();
			if (!_effect)
				initializeResource();
			//		���� ����:
			device->GetRenderTarget(0, &_original_surface);
			//		Matrix:
			D3DXMATRIX old_v, old_p, old_w;
			device->GetTransform(D3DTS_WORLD, &old_w);
			device->GetTransform(D3DTS_VIEW, &old_v);
			device->GetTransform(D3DTS_PROJECTION, &old_p);


			//	main:
			_onRender(dx9, camera);


			//	post:
			device->SetTransform(D3DTS_WORLD, &old_w);
			device->SetTransform(D3DTS_VIEW, &old_v);
			device->SetTransform(D3DTS_PROJECTION, &old_p);
			if (_original_surface)
			{
				device->SetRenderTarget(0, _original_surface);
				_original_surface->Release();
			}
			_original_surface = 0;
			return RESULT_SUCCESS;
		}

	public:
		ShaderProgram()
			: SuperClass(), _effect(0), _original_surface(0)
		{

		}
		~ShaderProgram()
		{
			ThisClass::releaseResource();
		}

	public:
		ID3DXEffect& getEffect() { return *_effect; }
		const ID3DXEffect& getEffect() const { return *_effect; }
		ShaderHandleSet& getShaderHandleSet() { return _handles; }
		const ShaderHandleSet& getShaderHandleSet() const { return _handles; }
		type_int getRenderTargetWidth() const;
		type_int getRenderTargetHeight() const;
		LPDIRECT3DSURFACE9 getOriginalSurface() { return _original_surface; }
		const LPDIRECT3DSURFACE9 getOriginalSurface() const { return _original_surface; }

	public:
		virtual type_result initializeResource();
		virtual type_result releaseResource()
		{
			if(_effect)
				_effect->Release();
			_effect = 0;
			_handles.release();

			return SuperClass::releaseResource();
		}
		virtual void release()
		{
			SuperClass::release();

			releaseResource();
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;			

			if (_header.getArgumentsComments().getLength() <= 0)
			{
				NETStringSet& args = _header.getArgumentsComments();
				args.create(2);
				args.push("Width Muliplier\n�������� RenderTarget�� ���� �ʺ� ������ ���ۿ� ������ �������� ���մϴ�.\n�������� ���� ������� �ػ󵵰� �������ϴ�.");
				args.push("Height Muliplier\n�������� RenderTarget�� ���� ���� ������ ���ۿ� ������ �������� ���մϴ�.\n�������� ���� ������� �ػ󵵰� �������ϴ�.");				
			}

			return _header;
		}

	private:

		virtual type_result _onRenderModel(LPDIRECT3DDEVICE9 device, Camera& camera, Model& model);
		virtual LPCVOID _onRequestShaderCode(OUT type_int& size_of_binary) const = 0;
		virtual type_result _onSetShaderHandles(ShaderHandleSet& handles) = 0;

	protected:		
		void _clear(LPDIRECT3DDEVICE9 device)
		{
			device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		}
		void _beginPass(int new_pass, LPDIRECT3DDEVICE9 device, RenderTarget& new_one)
		{
			device->SetRenderTarget(0, &new_one.getSurface());
			_clear(device);
			_effect->BeginPass(new_pass);
		}
		void _endFinalRenderTarget(RenderTargetSet& targets);
		void _endFinalRenderPass(RenderTargetSet& targets)
		{
			_endFinalRenderTarget(targets);
			_effect->EndPass();
		}
		type_result _beginFinalRenderPass(int new_pass, DX9& dx9)
		{
			_standByFinalRenderTarget(dx9);			

			_effect->BeginPass(new_pass);

			return RESULT_SUCCESS;
		}
		type_result _renderTargetVertex(LPDIRECT3DDEVICE9 device);
		void _endPassAndNotifyRenderTargetFilled(RenderTargetSet& targets)
		{
			_effect->EndPass();
			targets.notifyTargetFilled();
		}

		D3DXMATRIX _getOrthoMatrix()
		{
			D3DXMATRIX _instance;
			D3DXMatrixOrthoLH(&_instance, 2.0f, 2.0f, 0.0f, 1000.0f);
			return _instance;
		}
		type_result _standByFinalRenderTarget(DX9& dx9);

	private:				
		type_result _initializeShader(LPDIRECT3DDEVICE9 device);		
		type_result _bindHandles()
		{
			if( ! _effect)
			{
				KERNAL_ERROR("Shader�� �������� �������Ƿ� �ڵ��� ���ε� �� �� �����ϴ�");
				return RESULT_TYPE_ERROR;
			}
			_onSetShaderHandles(_handles);


			//	main:
			for(int n=0; n < _handles.getSize() ;n++)
			{
				ShaderHandle& handle = _handles[n];
				if(handle.isTechniqueHandle())
					handle.getHandle() = _effect->GetTechniqueByName(handle.getName().toCharPointer());
				else
					handle.getHandle() = _effect->GetParameterByName(NULL, handle.getName().toCharPointer());				
			}


			//	post:
			if(NEResult::hasError(_handles.isValid()))
			{
				KERNAL_ERROR("�ڵ��� �޾ƿ�����, �޾ƿ� �ڵ��� �߸��Ȱ� �����ϴ�");
				return RESULT_TYPE_ERROR;
			}

			return RESULT_SUCCESS;
		}
		type_result _updateBlendingStateToCamerasOne(LPDIRECT3DDEVICE9 device, Camera& camera);

	private:
		LPD3DXEFFECT _effect;
		ShaderHandleSet _handles;
		LPDIRECT3DSURFACE9 _original_surface;

	public:
		static const NECodeSet& getModuleScriptCodes();

	protected:
		static IDirect3DVertexBuffer9& _getRenderTargetVertex(LPDIRECT3DDEVICE9 device);
		static RenderTargetSet& _getRenderTargetSet(DX9& dx9) 
		{
			static RenderTargetSet _instance;
			if(	_instance.getSize() <= 0	&&	
				&dx9						&&
				dx9.getDevice()				)
			{
				_instance.create(2);
				_instance.push(RenderTarget());
				_instance.push(RenderTarget());
				_instance.initialize(dx9);	
			}

			return _instance; 
		}
	};
}