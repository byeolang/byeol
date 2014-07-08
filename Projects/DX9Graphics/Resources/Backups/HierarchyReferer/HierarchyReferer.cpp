#include "HierarchyReferer.hpp"
#include "../HierarchySubReferer/HierarchySubReferer.hpp"

namespace DX9Graphics
{
	HierarchyMeshX& HierarchyReferer::_getBindedHierarchyMeshX()
	{
		HierarchyMeshX* nullpointer = NE_NULL;
		if( ! _resource_binder.isBinded())
		{
			if(NEResult::hasError(_bindResourceModule()))
			{
				ALERT_ERROR(" : ���ҽ� ���ε� ���з� ����");
				return *nullpointer;
			}
		}
		RenderableResource& resource = static_cast<RenderableResource&>(_resource_binder.getBinded());
		static type_code hierarchy_mesh_x_code = Kernal::getInstance().getModuleManager()
			.getModule(HierarchyMeshX().getHeader()).getScriptCode();
		if(resource.getScriptCode() != hierarchy_mesh_x_code)
		{
			ALERT_ERROR(" : �߸��� ���ҽ� ���ε�");
			return *nullpointer;
		}
		
		return static_cast<HierarchyMeshX&>(resource);
	}

	HierarchySubReferer& HierarchyReferer::_createSubReferer(HierarchyMeshX::BoneFrame& frame, Referer& owner)
	{
		NEModuleCodeSet& ms = owner.getConnector().getModuleCodeSetKey().getValue();
		if(ms.getLength() == ms.getSize())
			ms.resize(ms.getLength() + 1);
		NEModuleManager& moduler = Kernal::getInstance().getModuleManager();
		static type_code sub_referer_code = moduler.getModule(HierarchySubReferer().getHeader()).getScriptCode();
		ms.push(moduler.getModuleSet()[sub_referer_code]);
		HierarchySubReferer& sub = static_cast<HierarchySubReferer&>(ms[ms.getLengthLastIndex()]);
		sub.initialize();	//	���Ŵ����� Ű�� �����Ƿ�, ���⼭ Ű�� �����Ѵ�.
		sub.getName() = frame.Name;
		sub.setBallSize(getConnector().getBallSize().getValue());

		//	�������� ���ٰ� ����.
		D3DXMATRIX	&	trans	= sub.getTransitionMatrix(),
					&	rotate	= sub.getRotationMatrix();
		trans._41 = frame.TransformationMatrix._41;
		trans._42 = frame.TransformationMatrix._42;
		trans._43 = frame.TransformationMatrix._43;
		rotate = frame.TransformationMatrix;
		rotate._41 = 0.0f;
		rotate._42 = 0.0f;
		rotate._43 = 0.0f;	

		return sub;
	}

	type_result HierarchyReferer::render()
	{
		if( ! isEnable())
			return RESULT_SUCCESS | RESULT_ABORT_ACTION;

		if( ! _resource_binder.isBinded())
		{
			if(NEResult::hasError(_bindResourceModule()))
			{
				ALERT_ERROR(" : ���ҽ� ���ε� ���з� ����");
				return RESULT_SUCCESS;
			}
		}



		//	main:
		static type_code sub = Kernal::getInstance().getModuleManager()
			.getModule(HierarchySubReferer().getHeader()).getScriptCode();
		//		DX9 ���ε�:
		DX9& dx9 = getDependent();
		if( ! &dx9)
		{
			ALERT_ERROR(" : ����̽� ���ε� ���з� ����");
			return RESULT_TYPE_ERROR;
		}
		LPDIRECT3DDEVICE9 device = dx9.getDevice();
		HierarchyMeshX& mesh = _getBindedHierarchyMeshX();
		if( ! &mesh)
		{
			ALERT_ERROR(" : ���ε� ����");
			return RESULT_TYPE_ERROR;
		}
		//		������ǥ ����:
		D3DXMATRIX& world = getWorldMatrix();
		D3DXMATRIX origin;		
		//		������:
		mesh.setHierarchyReferer(*this);
		mesh.setFaceType(getConnector().getFaceType().getValue());
		if( &Referer::getGlobalHoveredReferer() == (Referer*)this)
			mesh.renderOnHover();
		else if( &Referer::getGlobalReferer() == (Referer*)this)
			mesh.renderOnPicked();
		else
			mesh.render();		
		//		�ڽİ� ���� �ձ�:
		_searchModuleSet(getConnector().getModuleCodeSetKey().getValue(), &ThisClass::_renderLineBetweenChild);



		//	post:		
		return RESULT_SUCCESS;
	}

	HierarchySubReferer& HierarchyReferer::_getRefererBy( NEModuleCodeSet& ms, const NETString& name )
	{
		static type_code referer_code = Kernal::getInstance().getModuleManager()
			.getModule(HierarchySubReferer().getHeader()).getScriptCode();

		for(int n=0; n < ms.getLength() ;n++)
		{
			if(ms[n].getScriptCode() != referer_code)
			{
				ALERT_ERROR(" : ���� �߸��Ǿ���.")
					continue;
			}

			HierarchySubReferer& casted = static_cast<HierarchySubReferer&>(ms[n]);
			if(casted.getName() == name)
				return casted;

			HierarchySubReferer& returned = _getRefererBy(casted.getConnector().getModuleCodeSetKey().getValue(), name);
			if( &returned)
				return returned;
		}

		HierarchySubReferer* nullpointer = NE_NULL;
		return *nullpointer;
	}

	void HierarchyReferer::_computeFinalWorldMatrix( int container_n, HierarchyMeshX::BoneMesh& container )
	{
		Matrixes& final_worlds = _final_worlds_set[container_n];
		MatrixPointers& bone_transforms = _matrixes_set[container_n];
		if( ! &final_worlds || ! &bone_transforms)
		{
			ALERT_ERROR(" : ����!");
			return;
		}

		for(int bone_n=0; bone_n < bone_transforms.getLength() ;bone_n++)
		{
			HierarchySubReferer& sub_referer = bone_transforms[bone_n];
			final_worlds[bone_n] = container.pBoneOffsetMatrices[bone_n] * sub_referer.getWorldMatrix();				
		}
	}

	void HierarchyReferer::_createSubReferers( HierarchyMeshX::BoneFrame& frame, Referer& owner )
	{
		HierarchySubReferer& me = _createSubReferer(frame, owner);

		if(frame.pFrameSibling)
			_createSubReferers((HierarchyMeshX::BoneFrame&)*frame.pFrameSibling, owner);
		if(frame.pFrameFirstChild)
			_createSubReferers((HierarchyMeshX::BoneFrame&)*frame.pFrameFirstChild, me);
	}

	type_result HierarchyReferer::execute()
	{
		HierarchyMeshX& mesh = _getBindedHierarchyMeshX();
		if( ! &mesh)
		{
			ALERT_ERROR(" : ���ε� ����");
			return RESULT_TYPE_ERROR;
		}
		if( ! &mesh.getRootFrame()) return RESULT_TYPE_ERROR;
		if( ! isMatrixPointersSetInitialized())
			_initializeSubReferers();



		//	main:
		//		CombinedMatrix ���:
		getWorldMatrix() = createCombinedMatrix() * getParentWorldMatrix();
		//		�ڽĿ��� ����:
		static type_code sub = Kernal::getInstance().getModuleManager()
			.getModule(HierarchySubReferer().getHeader()).getScriptCode();
		_searchModuleSet(getConnector().getModuleCodeSetKey().getValue(), &ThisClass::_updateParentWorldMatrix);
		getConnector().getModuleCodeSetKey().getValue().execute();



		_fillFinalWorldMatrix(mesh.getRootFrame(), 0);
		return RESULT_SUCCESS;
	}

}