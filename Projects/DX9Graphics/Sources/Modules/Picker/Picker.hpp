#pragma once

#include "../../Commons/Templates/TValidator/TValidator.hpp"
#include "../Camera/Camera.hpp"

namespace DX9Graphics
{	
	class Model;
	template
	class NE_DLL TValidator<Camera>;

	class NE_DLL Picker : public NEModule
	{
	public:
		typedef NEModule SuperClass;
		typedef Picker ThisClass;

	public:
		NETArgument<NEModuleSelector>	arg_camera;
		NETArgument<NEModuleSelector>	arg_model;
		NETArgument<NEBooleanKey>		arg_is_picked;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_camera);
			tray.push(arg_model);
			tray.push(arg_is_picked);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_camera.getValue().NENodeSelector::isUsingAutoBinding() = true;
			arg_camera.getValue().isUsingAutoBinding() = true;
			arg_is_picked.setPurposeLimitation(NEArgumentBase::WRITTEN);

			return RESULT_SUCCESS;
		}

		virtual type_result _onExecute();

	public:
		class Vector : public D3DXVECTOR3
		{
		public:
			Vector() : D3DXVECTOR3() {}
			Vector(type_float x, type_float y, type_float z)
				: D3DXVECTOR3(x, y, z) {}
			NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
			{
				return saver << x << y << z;
			}
			NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
			{
				return loader >> x >> y >> z;
			}
		};
		typedef NEArrayTemplate<Vector> Vectors;

	public:		
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = "Picker";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getComment() = 
					"ī�޶� �Կ��ϴ� �𵨵��� ���콺�� ���ؼ� Hover �Ǿ������� �Ǵ��մϴ�.\n"
					"Hover�� �Ǹ� is picked�� TRUE�� �Ǹ�, Module ��ȯ�� TRUE�� ��ȯ�մϴ�.";
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2013-09-23";
				NETStringSet& args = _header.getArgumentsComments();
				args.create(3);
				args.push("Cameras\n�� ī�޶� �Կ��ϴ� Model���� ���콺�� Hover �Ǿ������� �Ǵ��մϴ�.\n������ Camera�� �������� ����, �� ������ Camera�� ���� ��� �����մϴ�.");
				args.push("Models\n�ռ� ������ Camera�� ���� �Կ��Ǵ� Model�Դϴ�.\n�� ���� ���콺�� Hover�Ǹ� is picked�� true�� �˴ϴ�.");
				args.push("Is picked\n�ռ� ������ Camera��, Models �� �ϳ��� Hover�� �Ǹ� true�� �˴ϴ�. ���ÿ� Module�� ����� TRUE�� ��ȯ�մϴ�.");
			}

			return _header;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		Vectors _createRayInWorldCoords(const Camera& camera) const;
		Vectors _createQuadOfSprite(type_uint width, type_uint height) const;		
		bool _isRayIntersectedToQuad(const Vectors& quad, const Vectors& ray, const D3DXMATRIX& world) const;
	};
}