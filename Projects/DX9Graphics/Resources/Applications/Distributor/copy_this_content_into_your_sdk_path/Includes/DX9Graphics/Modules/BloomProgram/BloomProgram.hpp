#pragma once

#include "../ShaderProgram/ShaderProgram.hpp"

namespace DX9Graphics
{
	class NE_DLL BloomProgram : public ShaderProgram
	{
	public:
		typedef BloomProgram ThisClass;
		typedef ShaderProgram SuperClass;

	public:
		friend class DX9;

	public:
		type_float& getBloomLevel()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getBloomLevel());
		}
		const type_float& getBloomLevel() const
		{
			const NEKey& key = getKeySet()[4];
			if( ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("4��Ű�� NEFLOAT_KEY���� �մϴ�");
				type_float* nullpoint = 0;
				return *nullpoint;
			}

			const NEFloatKey& target = static_cast<const NEFloatKey&>(key);
			return target.getValue();
		}

	public:
		virtual type_result initialize();
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const;

	private:
		virtual type_result _onRender(DX9& dx9, Camera& camera)
		{
			if( ! isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;		

			
			//	main:
			//		�غ�:
			LPDIRECT3DDEVICE9 device = dx9.getDevice();
			//		�ڵ�� �� ����:
			_updateBloomLevel();
			_updateTextureSize();
			RenderTargetSet& targets = _getRenderTargetSet(dx9);
			ShaderHandleSet& handles = getShaderHandleSet();
			if(targets.getSize() <= 0)
			{
				ALERT_ERROR("DX9 ���ε� ���з� ����Ÿ���� �������� ���߽��ϴ�.");
				return RESULT_TYPE_ERROR;
			}
			//			Matrixġȯ:
			D3DXMATRIX new_w, new_v, new_p = _getOrthoMatrix();
			D3DXMatrixIdentity(&new_w);
			D3DXMatrixLookAtLH(&new_v, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
			device->SetTransform(D3DTS_WORLD, &new_w);
			device->SetTransform(D3DTS_VIEW, &new_v);
			device->SetTransform(D3DTS_PROJECTION, &new_p);
			//			�ӽ� ����Ÿ�� �غ�:
			RenderTargetSet& temp = _getTemporaryRenderTargetSet(dx9);
			if(temp.getSize() <= 0) 
				return RESULT_TYPE_ERROR;
			ID3DXEffect& effect = getEffect();


			//	main:
			//			���̴� �غ�:
			device->BeginScene();
			effect.SetTechnique(handles[0].getHandle());
			effect.Begin(NULL, 0);
			//		0�н�: ���� ���̱�
			_beginPass(0, device, temp.getEmptyTarget());
			device->SetTexture(0, &targets.getFilledTarget().getTexture());
			_renderTargetVertex(device);			
			_endPassAndNotifyRenderTargetFilled(temp);
 			//		2�н�: Y���� ��
 			for(int pass=1; pass < 3 ;pass++)	//	0 <= pass <= 2
 			{
 				_beginPass(pass, device, temp.getEmptyTarget());
 				device->SetTexture(0, &temp.getFilledTarget().getTexture());
 				_renderTargetVertex(device);
 				_endPassAndNotifyRenderTargetFilled(temp);				
 			}
			//		3�н�: ���ļ� ���� ���
			_beginFinalRenderPass(3, dx9);
			device->SetTexture(0, &targets.getFilledTarget().getTexture());		//	����(TEXTURE1)			
			device->SetTexture(1, &temp.getFilledTarget().getTexture());		//	����(TEXTURE1)
			_renderTargetVertex(device);		
			_endFinalRenderPass(targets);


			//	post:
			//		����:
			effect.End();
			device->EndScene();
			device->SetTexture(1, NULL);
			return RESULT_SUCCESS;
		}
		virtual LPCVOID _onRequestShaderCode(OUT type_int& size_of_binary) const;
		virtual type_result _onSetShaderHandles(ShaderHandleSet& handles)
		{
			handles.release();
			handles.create(4);
			handles.push(ShaderHandle(_T("Tech"), true));
			handles.push(ShaderHandle(_T("_bloom")));
			handles.push(ShaderHandle(_T("m_TexW")));
			handles.push(ShaderHandle(_T("m_TexH")));			

			return RESULT_SUCCESS;
		}

	private:
		type_result _updateBloomLevel()
		{
			type_float& bloom = getBloomLevel();
			if(bloom < 0.0f)
				bloom = 0.0f;
			if( ! &getEffect())
			{
				ALERT_ERROR("����Ʈ�� �ʱ�ȭ �Ǿ����� �ʾƼ� �ڵ�� ���� �ѱ� �� �����ϴ�.");
				return RESULT_TYPE_ERROR;
			}
			ShaderHandle& handle = getShaderHandleSet()[1];
			if( ! &handle || handle.isTechniqueHandle())
			{
				ALERT_ERROR("�߸��� �ڵ鰪�Դϴ�.");
				return RESULT_TYPE_ERROR;
			}

			getEffect().SetFloat(handle.getHandle(), bloom);

			return RESULT_SUCCESS;
		}
		type_result _updateTextureSize()
		{
			type_int	tex_width = getRenderTargetWidth(),
						tex_height = getRenderTargetHeight();
			if( ! &getEffect())
			{
				ALERT_ERROR("����Ʈ�� �ʱ�ȭ �Ǿ����� �ʾƼ� �ڵ�� ���� �ѱ� �� �����ϴ�.");
				return RESULT_TYPE_ERROR;
			}
			ShaderHandle& width_handle = getShaderHandleSet()[2],
						& height_handle = getShaderHandleSet()[3];
			if( ! &width_handle		||	! width_handle.getHandle()	|| width_handle.isTechniqueHandle()		||
				! &height_handle	||	! height_handle.getHandle()	|| height_handle.isTechniqueHandle()	)
			{
				ALERT_ERROR("�߸��� �ڵ鰪�Դϴ�.");
				return RESULT_TYPE_ERROR;
			}

			getEffect().SetFloat(width_handle.getHandle(), static_cast<float>(tex_width));
			getEffect().SetFloat(height_handle.getHandle(), static_cast<float>(tex_height));

			return RESULT_SUCCESS;
		}

	private:
		static RenderTargetSet& _getTemporaryRenderTargetSet(DX9& dx9)
		{
			static RenderTargetSet _instance;
			if( ! &dx9)
			{
				KERNAL_ERROR("�ӽ� ����Ÿ�� �ʱ�ȭ ����");
				return _instance;
			}

			if(	_instance.getSize() <= 0	&&
				&dx9						&&
				dx9.getDevice()				)
			{
				_instance.create(2);
				_instance.push(RenderTarget());
				_instance.push(RenderTarget());
				
				for(int n=0; n < _instance.getSize() ;n++)
					_instance[n].initialize(dx9);
			}

			return _instance;
		}

	};
}