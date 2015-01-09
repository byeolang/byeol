#pragma once

#include "../Includes/Includes.hpp"
#include <cstdlib>
#include <ctime>

namespace NE
{
	class NE_DLL Random : public NEModule
	{
	public:
		typedef Random ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEFloatKey> arg_output;
		NETArgument<NEFloatKey>	arg_max;
		NETArgument<NEFloatKey>	arg_min;

	protected:
		virtual type_result _onFetchModule()
		{
			srand(static_cast<unsigned int>(time(0)));
			arg_output.setPurposeLimitation(NEArgumentBase::WRITTEN);
			arg_max.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_min.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_output);
			tray.push(arg_max);
			tray.push(arg_min);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			//	�� ����� Max�� Min�� ������ ��쵵 �����Ѵ�
			type_float	max = arg_max.getValue(),
				min = arg_min.getValue(),
				delta = max - min,
				scale_factor = static_cast<float>(rand()) / RAND_MAX,
				answer = scale_factor * delta;

			arg_output.setValue(answer);

			return answer >= 0 ? RESULT_TRUE : RESULT_FALSE;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Random";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013-10-15";
				_instance.getComment() = 
					"�־��� �� �Ǽ� ������ ������ ���� �����Ͽ� ��ȯ�մϴ�.\n"
					"������ ������ ���� ������ ��� FALSE��, 0 �Ǵ� ��� �ϰ�� TRUE�� Module �������� ��ȯ�մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(3);
				args.push("Output\n������ ���� �����Ǿ� ���⿡ ����˴ϴ�.\n�翬�� �������, KeySelector�� IntKey�� �����ŵ� �˴ϴ�.");
				args.push("Max\n������ ���� ���� �� �ִ� �ִ밪 �Դϴ�.");
				args.push("Min\n������ ���� ���� �� �ִ� �ּҰ� �Դϴ�.");
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
	};
}