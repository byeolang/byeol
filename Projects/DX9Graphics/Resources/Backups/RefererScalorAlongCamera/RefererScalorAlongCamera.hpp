#pragma once

#include "../RayGenerator/RayGenerator.hpp"

namespace DX9Graphics
{
	class NE_DLL RefererScalorAlongCamera : public RayGenerator
	{
		typedef RefererScalorAlongCamera ThisClass;
		typedef RayGenerator SuperClass;

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.isValid() != RESULT_SUCCESS)
			{
				_header.getName() = _T("RefererScalorAlongCamera.dev.dbg");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() = _T("0��. ������\n1��. ���۷�\n2��. ī�޶� ��⼿����");
				_header.getVersion()  = _T("0.0.1a");
				_header.getReleaseDate() = _T("");
				_header.getModuleDependencies().create(0);
				_header.setMaxErrorCodeCount(1);
			}

			return _header;
		}

	public:
		virtual type_result execute();

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	protected:
		void _computeCombinedMatrix(Referer& referer, Camera& camera);		
	};
}