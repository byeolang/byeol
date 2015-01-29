#pragma once

#include "../../Commons/Templates/TValidator/TValidator.hpp"
#include "../Particle/Paritcle.hpp"

namespace DX9Graphics
{
	//	Particle�� Look�� Ư�� ��ǥ �� ������� ���ϵ��� Angle�� �����Ѵ�.
	class NE_DLL ParticleObserver : public TValidator<Particle>
	{
	public:
		typedef SpriteTexter ThisClass;
		typedef TValidator<Particle> SuperClass;

	public:
		NETArgument<NEModuleSelector>	arg_target;
		NETArgument<NEFloatKey>			arg_xpos;
		NETArgument<NEFloatKey>			arg_ypos;
		NETArgument<NEFloatKey>			arg_zpos;		

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass:_onFetchArguments(tray);

			tray.push(arg_target);
			tray.push(arg_xpos);
			tray.push(arg_ypos);
			tray.push(arg_zpos);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_xpos.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_ypos.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_zpos.setPurposeLimitation(NEArgumentBase::READ_BY);			

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			//	pre:			
			Particle* target = 0x00;
			if(arg_target.isEnable())
			{
				arg_target.getValue().initializeReferingPoint();
				target = &_cast(arg_target.getValue().getModule());
			}
			D3DXVECTOR3 target_point = _createTargetPoint(*target);


			//	main:
			NEModule* e = 0x00;
			while(e = &arg_validator.getValue()getModule())
			{
				Particle& p = _cast(*e);
				if( ! &p) continue;

				D3DXVECTOR3 direction = _createDirectionVector(*e, *target_point);
				D3DXVec3Normalize(&direction, &direction);

				D3DXVECTOR3 degree = DX9::createYawPitchRollFrom(DX9::createQuaternionFrom(direction));
				p.arg_rotate_x.setValue(degree.x);
				p.arg_rotate_y.setValue(degree.y);
				p.arg_rotate_z.setValue(degree.z);
			}			

			return RESULT_SUCCESS;
		}

	public:
		virtual NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("ParticleObserver");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() =
					"�־��� Particle�� RotateX, RotateY, RotateZ degree�� ȸ�����Ѽ�, �ٸ� Particle�̳� Ư�� ��ǥ�� �ٶ󺸰� �� �մϴ�.\n"
					"���� : Target Argument �� Disabled �ؾ� X,Y,Zpos ��ǥ���� �ٶ󺸰� �մϴ�.\n"
					"Source�� �Ǵ� Particle�� �������� �ǵ��� �� ���� �ֽ��ϴ�."
					_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2015-01-26");
				NETStringSet& args = _header.getArgumentsComments();
				args.create(5);
				args.push("Sources\n����� �Ǵ� Particle Module���Դϴ�.\nCamera, Model, AnimatedModel���� ���⿡ ���մϴ�.");
				args.push("Target\nParicle�� �ٶ󺸰� �ϰ� ���� �ٸ� Particle �Դϴ�.\nDisabled �Ǹ� X/Y/Z Pos Arguments�� ��� ����մϴ�.");
				args.push("X Pos\nParitcle�� �ٶ󺸰� �ϰ� ���� ������ X ��ǥ�Դϴ�.\nTarget�� Disabled �Ǿ�� �����մϴ�.");
				args.push("Y Pos\nParitcle�� �ٶ󺸰� �ϰ� ���� ������ Y ��ǥ�Դϴ�.\nTarget�� Disabled �Ǿ�� �����մϴ�.");
				args.push("Z Pos\nParitcle�� �ٶ󺸰� �ϰ� ���� ������ Z ��ǥ�Դϴ�.\nTarget�� Disabled �Ǿ�� �����մϴ�.");				
			}

			return _header;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		D3DXVECTOR3 _createTargetPoint(const Particle& target)
		{
			if(&target)
				return target.createTranslationVector();

			return D3DXVECTOR3(arg_xpos.getValue(), arg_ypos.getValue(), arg_zpos.getValue());
		}
		D3DXVECTOR3 _createDirectionVector(const Particle& me, const D3DXVECTOR3& target) const
		{
			return target - me.createTranslationVector();
		}
	};
}