#pragma once

#include "../ShaderProgram/ShaderProgram.hpp"

namespace DX9Graphics
{
	class NE_DLL MosaicProgram : public ShaderProgram
	{
	public:
		typedef MosaicProgram ThisClass;
		typedef ShaderProgram SuperClass;

	public:
		friend class DX9;

	public:
		type_uint& getMosaicPrecision()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_uint&>(casted->getMosaicPrecision());
		}
		const type_uint& getMosaicPrecision() const
		{
			const NEKey& key = getKeySet()[4];
			if( ! key.isSubClassOf(NEType::NEUINT_KEY))
			{
				ALERT_ERROR("4��Ű�� NEUINT_KEY���� �մϴ�");
				type_uint* nullpoint = 0;
				return *nullpoint;
			}

			const NEUIntKey& target = static_cast<const NEUIntKey&>(key);
			return target.getValue();
		}

	public:
		virtual type_result initialize()
		{
			SuperClass::initialize();

			NEKeyCodeSet& keyset = getKeySet();
			keyset.resize(keyset.getSize() + 1);			
			return keyset.push(NEUIntKey(30));
		}
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
			_updateMosaicPrecision();	//	�ؽ���ũ�Ⱑ �� Blur������ �ȴ�.
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
			ID3DXEffect& effect = getEffect();


			//	main:
			//			���̴� �غ�:
			device->BeginScene();
			effect.SetTechnique(handles[0].getHandle());
			effect.Begin(NULL, 0);
			//		0�н�: ������ũ
			_beginFinalRenderPass(0, dx9);
			device->SetTexture(0, &targets.getFilledTarget().getTexture());
			_renderTargetVertex(device);
			_endFinalRenderPass(targets);


			//	post:
			//		����:
			effect.End();
			device->EndScene();
			return RESULT_SUCCESS;
		}
		virtual LPCVOID _onRequestShaderCode(OUT type_int& size_of_binary) const;
		virtual type_result _onSetShaderHandles(ShaderHandleSet& handles)
		{
			handles.release();
			handles.create(2);
			handles.push(ShaderHandle(_T("Tech"), true));			
			handles.push(ShaderHandle(_T("mosaic_precision")));	//	������ũ ���е�.		

			return RESULT_SUCCESS;
		}

	private:		
		type_result _updateMosaicPrecision()
		{
			type_uint& precision = getMosaicPrecision();
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

			getEffect().SetInt(handle.getHandle(), static_cast<type_int>(precision));
			
			return RESULT_SUCCESS;
		}
	};
}