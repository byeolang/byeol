#pragma once

#include "../../Commons/Templates/TValidator/TValidator.hpp"
#include "../Model/Model.hpp"

namespace DX9Graphics
{
	//	Particle�� Look�� Ư�� ��ǥ �� ������� ���ϵ��� Angle�� �����Ѵ�.
	class NE_DLL ModelMediator : public TValidator<Model>
	{
	public:
		typedef ModelMediator ThisClass;
		typedef TValidator<Model> SuperClass;

	public:
		NETArgument<NEModuleSelector>	arg_target;
		NETArgument<NEFloatKey>			arg_distance;
		NETArgument<NEFloatKey>			arg_threshold;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass::_onFetchArguments(tray);

			tray.push(arg_target);
			tray.push(arg_distance);
			tray.push(arg_threshold);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_distance.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_threshold.setPurposeLimitation(NEArgumentBase::WRITTEN);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute();

	public:
		virtual NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("ModelMediator");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() =
					"�־��� 2 Model ���� �Ÿ��� �浹����(threshold)�� ����մϴ�.\n"
					"���� �浹�� ���, TRUE�� ��ȯ�մϴ�.";
				_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2015-02-12");
				NETStringSet& args = _header.getArgumentsComments();
				args.create(4);
				args.push("Source\n����� �Ǵ� Model �Դϴ�.\nModel, AnimatedModel���� ���⿡ ���մϴ�.");
				args.push("Target\nSource�� �Ÿ� / �浹���θ� ������ �ٸ� Model �Դϴ�.");
				args.push("Distance\nSource�� Target���� �Ÿ��� ����Ͽ� ���⿡ ��ϵ˴ϴ�.");
				args.push("Threshold(�浹�Ӱ谪)\n0�� ���ų� ������ �浹�� ���Դϴ�.\n����̸� �浹���� ���� �Ÿ� �Դϴ�.");				
			}

			return _header;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:		
		type_float _createDistance(const D3DXVECTOR3& point1, const D3DXVECTOR3& point2)
		{
			return D3DXVec3Length(&(point2 - point1));
		}
	};
}