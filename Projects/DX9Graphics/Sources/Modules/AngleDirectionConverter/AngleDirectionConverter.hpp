#pragma once

#include "../../Includes/Includes.hpp"

namespace DX9Graphics
{
	//	Particle�� Look�� Ư�� ��ǥ �� ������� ���ϵ��� Angle�� �����Ѵ�.
	class NE_DLL AngleDirectionConverter : public NEModule
	{
	public:
		typedef AngleDirectionConverter ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEFloatKey>			arg_x_angle;
		NETArgument<NEFloatKey>			arg_y_angle;
		NETArgument<NEFloatKey>			arg_z_angle;
		NETArgument<NEFloatKey>			arg_xpos;
		NETArgument<NEFloatKey>			arg_ypos;
		NETArgument<NEFloatKey>			arg_zpos;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_x_angle);
			tray.push(arg_y_angle);
			tray.push(arg_z_angle);
			tray.push(arg_xpos);
			tray.push(arg_ypos);
			tray.push(arg_zpos);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_x_angle.setPurposeLimitation(NEArgumentBase::READ_OR_WRITTEN);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute();

	public:
		virtual NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("AngleDirectionConverter");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() =
					"���Ϸ� ������ ���⺤�� �� ��ȯ�� ���ݴϴ�.\n"
					"Pitch Argument�� Purpose�� READ�� �ϸ� ���Ϸ��� -> ���⺤�ͷ� ��ȯ�˴ϴ�.\n"
					"�ݴ�� WRITTEN���� �ϸ� ���Ϸ��� <- ���⺤�ͷ� ��ȯ�˴ϴ�.";
				_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2015-01-26");
				NETStringSet& args = _header.getArgumentsComments();
				args.create(6);
				args.push("Pitch(degree)\nPurpose�� READ�� �ϸ� ���Ϸ��� -> ���⺤�ͷ� ��ȯ�˴ϴ�.\nWRITTEN���� �ϸ� ���Ϸ��� <- ���⺤�ͷ� ��ȯ�˴ϴ�.");
				args.push("Yaw(degree)");
				args.push("Roll(degree)");
				args.push("X Pos");
				args.push("Y Pos");
				args.push("Z Pos");
			}

			return _header;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}