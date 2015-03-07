#pragma once

#include "../../Commons/Templates/TValidator/TValidator.hpp"
#include "../Particle/Particle.hpp"

namespace DX9Graphics
{
	class NE_DLL ParticleVelocity : public TValidator<Particle>
	{
	public:
		typedef ParticleVelocity ThisClass;
		typedef TValidator<Particle> SuperClass;

	public:
		NETArgument<NEFloatKey>			arg_magnitude;
		NETArgument<NEModuleSelector>	arg_target;
		NETArgument<NEFloatKey>			arg_direction_x_angle;
		NETArgument<NEFloatKey>			arg_direction_y_angle;
		NETArgument<NEFloatKey>			arg_direction_z_angle;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass::_onFetchArguments(tray);

			tray.push(arg_magnitude);
			tray.push(arg_target);
			tray.push(arg_direction_x_angle);
			tray.push(arg_direction_y_angle);
			tray.push(arg_direction_z_angle);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_magnitude.setValue(1.0f);
			arg_magnitude.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_target.setEnable(true);
			arg_direction_x_angle.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_direction_y_angle.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_direction_z_angle.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			D3DXVECTOR3 delta, target_trans;
			type_bool branch = arg_target.isEnable();
			if( ! branch)
				delta = _createDeltaByAngle();
			else
			{
				arg_target.getValue().initializeReferingPoint();
				Particle& p = cast(arg_target.getValue().getModule());
				target_trans = p.createTranslationVector();
			}


			//	main:
			NEModule* e = 0x00;
			while(e = &arg_validator.getValue().getModule())
			{
				Particle& particle = cast(*e);
				if( ! &particle) continue;

				if(branch)
					delta = _createDeltaByTarget(target_trans, particle.createTranslationVector());

				particle.arg_trans_x.setValue(particle.arg_trans_x.getValue() + delta.x);
				particle.arg_trans_y.setValue(particle.arg_trans_y.getValue() + delta.y);
				particle.arg_trans_z.setValue(particle.arg_trans_z.getValue() + delta.z);
			}			

			return RESULT_SUCCESS;
		}
		D3DXVECTOR3 _createDeltaByTarget(const D3DXVECTOR3& target_trans, const D3DXVECTOR3& my_pos) const
		{
			D3DXVECTOR3 delta = target_trans - my_pos;
			D3DXVec3Normalize(&delta, &delta);
			delta *= arg_magnitude.getValue();

			return delta;
		}
		D3DXVECTOR3 _createDeltaByAngle() const
		{			
			D3DXVECTOR3 to_return;

			type_float	yaw = arg_direction_y_angle.getValue(),
				pitch = arg_direction_x_angle.getValue(),
				roll = arg_direction_z_angle.getValue();

			D3DXMATRIXA16 rmat = EulerTransitor::createRotationMatrix(
				DX9::createQuaternionFrom(yaw, pitch, roll)
				);

			D3DXVec3TransformNormal(&to_return, &D3DXVECTOR3(0, 0, 1), &rmat);

			return to_return * arg_magnitude.getValue();
		}

	public:
		virtual NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("ParticleVelocity");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() =
					"Particle���� Ư���� �������� Magnitude ��ŭ Particle�� X, Y, Z�� �̵���ŵ�ϴ�.\n"
					"���⼭ Ư���� ������ ��ǥ�� �Ǵ� Ÿ���� �����ϰų�, ���ع��⿡(= ���� 0, 0, 1)�� X�� ȸ��, Y�� ȸ��, Z�� ȸ������ "
					"���������� �����Ͽ� �������ϴ�.\n";
				"Particle�� ���ϴ� Module�� Camera, Model, AnimatedModel ���� �ֽ��ϴ�.\n"
					"�������, �� Particle���� ������ �ִ� X�� Angle, Y�� Angle, Z�� Angle�� "
					"�״�� �� Module�� Angle�� �Ҵ��ϸ�, ��ü�� ���ϴ� �������� ���ư��� �˴ϴ�.";
				_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2013-10-05");
				NETStringSet& args = _header.getArgumentsComments();
				args.create(6);
				args.push("Magnitude\n�Ʒ��� ���ڷ� �������� �������� �󸶳� �̵����� ���մϴ�.\n��, ������������� ���� ���Դϴ�.");
				args.push("Particle(s)\n����� �Ǵ� Particle Module���Դϴ�.\nCamera, Model, AnimatedModel���� ���⿡ ���մϴ�.");
				args.push("Target\nParticle�� ���ư����� �ϴ� ��ǥ Particle �Դϴ�.\nDisable�� �� ���, �Ʒ��� Angle ����� ������ ���մϴ�.");
				args.push("Direction Angle X\n�̵��� ����� ������, ���ع���v(0, 0, 1)�� ���� X�� ȸ�������Դϴ�.");
				args.push("Direction Angle y\n�̵��� ����� ������, ���ع���v(0, 0, 1)�� ���� Y�� ȸ�������Դϴ�.");
				args.push("Direction Angle Z\n�̵��� ����� ������, ���ع���v(0, 0, 1)�� ���� Z�� ȸ�������Դϴ�.");
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