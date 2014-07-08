#pragma once

#include "../DockableResource/DockableResource.hpp"

namespace DX9Graphics
{
	class NE_DLL Mesh : public DockableResource
	{
		typedef Mesh ThisClass;
		typedef DockableResource SuperClass;

	public:
#include "innerclass/Material/Material.hpp"
#include "innerclass/BoundingSphere/BoundingSphere.hpp"

		typedef NEListTemplate<Material> MaterialList;
		typedef NEListTemplate<IDirect3DTexture9*, false> TextureList;

	public:
		Mesh()
			: SuperClass(), _mesh(NE_NULL)
		{
			_release();
		}
		Mesh(const Mesh& source)
			: SuperClass(source), _mesh(NE_NULL)
		{
			//	�̹� ��ΰ��� assign�� �����ִ�.
			//	_materials, _textures�� assign ������ ���ؼ� �����Ͽ���.
			_release();
		}

	public:
		virtual ~Mesh()
		{
			_release();
		}

	public:
		virtual bool isResourceRetrieved()
		{
			return _mesh != NE_NULL;
		}
		virtual type_result retrieveResource()
		{
			NEKey& key = getKeySet()[1];
			if(key.getType() != NEType::NETSTRING_KEY)
			{
				ALERT_ERROR(" : Ű���� �ջ�Ǿ����ϴ�. 1��Ű�� TSTRING_KEY�� �ƴմϴ�.");
				return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
			}
			NETString& path = static_cast<NETStringKey&>(key).getValue();			
			//	����� .x ���ϸ� �����Ѵ�.
			type_result result = _loadMeshFromX(path);
			if(NEResult::hasError(result))
			{
				ALERT_ERROR(" : ������ ��η� �޽ø� �ε��߿� ������ �߻��߽��ϴ�.\n\t���: %s\n\t�����ڵ�: %d", path.toCharPointer(), result);
				return RESULT_TYPE_ERROR;
			}
			if( NEResult::hasError(_computeBoundingSphere()) )
			{
				ALERT_ERROR(" : �� ��� ���߿� ���� �߻�");
				return RESULT_TYPE_ERROR;
			}

			return RESULT_SUCCESS;
		}
		virtual type_result render();
		virtual type_result renderOnPicked();
		virtual type_result renderOnHover();

	public:
		const ThisClass& operator=(const ThisClass& source)
		{
			SuperClass::operator=(source);

			return *this;
		}

	public:
		const MaterialList& getMaterialList() const
		{
			return _materiallist;
		}
		const TextureList& getTextureList() const
		{
			return _texturelist;
		}
		BoundingSphere& getBoundingSphere()
		{
    		return _bounding_sphere;
		}
		const BoundingSphere& getBoundingSphere() const
		{
			return _bounding_sphere;
		}
		ID3DXMesh& getMesh()
		{
			return *_mesh;
		}
		const ID3DXMesh& getMesh() const
		{
			return *_mesh;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;

	public:
		virtual type_result execute()
		{
			if( ! isResourceRetrieved())
				return retrieveResource();

			return RESULT_SUCCESS;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new Mesh(*this));
		}
		virtual void release()
		{
			SuperClass::release();

			_release();
		}
		virtual type_result isValid() const
		{
			type_result result = SuperClass::isValid();
			if(NEResult::hasError(result)) return result;
			if(	(_mesh && _materiallist.getLength() <= 0)	||
				( ! _mesh && _materiallist.getLength() > 0)	)
				return RESULT_TYPE_ERROR;

			return RESULT_SUCCESS;
		}

	private:
		NETString _createWorkDirectory(NETString& path)
		{
			NETStringSet buffer;
			path.split("\\", buffer);

			buffer.pop();
			
			NETString workdirectory;
			for(int n=0; n < buffer.getLength() ;n++)
				workdirectory += buffer[n] + _T("\\");

			return workdirectory;
		}
		type_result _loadMeshFromX(NETString& path);
		type_result _releaseTextures()
		{
			while(_texturelist.getLength() > 0)
			{
				_texturelist[0].Release();
				_texturelist.popFront();
			}

			return RESULT_SUCCESS;
		}
		void _release()
		{
			_materiallist.release();
			_releaseTextures();
			if(_mesh)
				_mesh->Release();
			_mesh = NE_NULL;
			_is_tried = false;
		}
		type_result _computeBoundingSphere()
		{
			//	pre:
			if( ! _mesh)
			{
				ALERT_ERROR(" : �޽ð� �ε���� �ʾ����Ƿ� �ٿ�� ���� ����� �� �����ϴ�.");
				return RESULT_TYPE_ERROR;
			}
			


			//	main:
			void* ptr=NULL;
			DWORD numVerts=_mesh->GetNumVertices();
			DWORD fvfSize=D3DXGetFVFVertexSize(_mesh->GetFVF());  // See DX8 Version
			if( FAILED(_mesh->LockVertexBuffer(0, &ptr)) )
			{
				ALERT_ERROR(" : �� ����");
				return RESULT_TYPE_ERROR;
			}
			BoundingSphere& sphere = getBoundingSphere();
			sphere.release();
			D3DXComputeBoundingSphere((D3DXVECTOR3 *) ptr, numVerts, fvfSize, 
				&sphere.getCenter(), &sphere.getRadius());



			//	post:
			_mesh->UnlockVertexBuffer();
			return RESULT_SUCCESS;
		}

	private:
		void _setD3DColorToRed(D3DCOLORVALUE& colorset)
		{
			colorset.a = 255;
			colorset.b = 0;
			colorset.g = 0;
			colorset.r = 255;
		}
		void _setD3DColorToGreen(D3DCOLORVALUE& colorset)
		{
			colorset.a = 255;
			colorset.b = 0;
			colorset.g = 255;
			colorset.r = 0;
		}

	protected:
		MaterialList _materiallist;
		TextureList _texturelist;
		ID3DXMesh* _mesh;
		BoundingSphere _bounding_sphere;
		bool _is_tried;
	};
}