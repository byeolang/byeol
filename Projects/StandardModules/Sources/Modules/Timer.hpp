#pragma once

#include "../Includes/Includes.hpp"
#include <mmstream.h>
#pragma comment(lib, "winmm.lib")

namespace NE
{
	class NE_DLL Timer : public NEModule
	{
	public:
		typedef Timer ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEIntKey> arg_term_ms;

	protected:
		virtual type_result _onFetchModule()
		{
			timeBeginPeriod(1);
			_before = -1;
			arg_term_ms.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_term_ms);

			return RESULT_SUCCESS;
		}

	protected:
		virtual type_result _onExecute()
		{
			type_uint new_evaluted= timeGetTime();
			if(_before < 0)
				_before = new_evaluted;

			type_int	term = new_evaluted - _before,
				arg_term = arg_term_ms.getValue();

			if (term > arg_term)
			{
				_before += arg_term;
				return RESULT_TRUE;
			}

			return RESULT_TRUE;
		}

	public:
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
		{
			NEModule::serialize(saver);

			return saver << _before;
		}
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
		{
			NEModule::serialize(loader);

			return loader >> _before;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if (_instance.isValid() != RESULT_NOTHING)
			{
				_instance.getName() = "Timer";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013-10-15";
				_instance.getComment() =
					"�־��� Term(milli second)���� TRUE�� ��ȯ�մϴ�.";
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(1);
				args.push("Term\n�� �ð��� ���������� Module �������� TRUE�� ��ȯ�մϴ�.\n������ �и�������(0.001��) �̱⿡, 1000�� �Է��ϸ� 1�ʰ� �˴ϴ�.");				
			}

			return _instance;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		type_int _before;
	};
}