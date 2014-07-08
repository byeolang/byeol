#include "CameraChaser.hpp"
#include "../Camera/Camera.hpp"
#include "../Model/Model.hpp"

namespace DX9Graphics
{
	const NEExportable::ModuleHeader& CameraChaser::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("CameraChaser");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. CameraSelector\n1��. ����� �Ǵ� ��ü(1���� ��ȿ)\n2��. ī�޶�� ��ü�� �Ÿ�\n3��. ������X \n4��. ������Y \n5��. ������Z");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	void CameraChaser::_onDirect(Camera& camera, Model& model)
	{
		const D3DXMATRIX& world = model.getWorldMatrix();
		const D3DXVECTOR3& trans = model.createTranslationVector();
		type_float distance = getDistance();
		D3DXVECTOR3 transited;
		D3DXVec3TransformCoord(&transited, &trans, &world);

		D3DXVECTOR3 cam_pos = camera.createTranslationVector(),
					look = transited - cam_pos, normalized;
		D3DXVec3Normalize(&normalized, &look);			
		cam_pos = cam_pos + (-normalized * distance);

		camera.setLookVector(normalized);
		getDestinationX() = cam_pos.x;
		getDestinationY() = cam_pos.y;
		getDestinationZ() = cam_pos.z;
	}
}