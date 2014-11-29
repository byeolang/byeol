#pragma once

#include "../WNDSpy/WNDSpy.hpp"

namespace NE
{
	class NE_DLL CharInputSpy : public WNDSpy
	{
	public:
		typedef CharInputSpy ThisClass;
		typedef WNDSpy SuperClass;

	public:
		NEITArgument<NEModuleSelector> arg_window_selector;
		NEITArgument<NETStringKey> arg_sentence;
		NEITArgument<NEUIntKey> arg_max_delay;

	protected:
		virtual type_result _onFetchArguments(NEArgumentInterfaceList& tray) const
		{
			arg_window_selector.getValue().isUsingAutoBinding() = true;
			arg_max_delay.getDefault() = 500;
			tray.push(&arg_window_selector);
			tray.push(&arg_sentence);
			return tray.push(&arg_max_delay);
		}
		virtual type_result _onExecute();

	public:
		CharInputSpy()
			: WNDSpy(), _delay_start_time(0)
		{
			
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = "CharInputSpy";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getComment() = "�ؽ�Ʈ�ڽ� � ���ڸ� �Է��ϸ� TRUE�� ��ȯ�˴ϴ�.\nKeyboardSpy�� � Ű�� ���������� �Ǵ��Ѵٸ�, CharInputSpy�� � ���ڰ� �ԷµǾ������� �Ǵ��Ѵٴ� ���� ���̰� ������ ������ �μ���.";
				_header.getReleaseDate() = "2014-01-29";
				NETStringSet& args = _header.getArgumentsComments();
				args.create(3);
				args.push("���� �Է��� ������ ������(Window)");
				args.push("[OUT] �Է��� ���ڸ� �̰��� �����մϴ�.");
				args.push("�� ����ŭ �����Ҷ� �����̸� �ݴϴ�.");
			}

			return _header;
		}

	public:
		virtual void release()
		{
			SuperClass::release();

			_delay_start_time = 0;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		/*
			�� _delay_start_time�� serialize ���� �ʴ°�:
				_delay_start_time�� ���α׷��� ������ �󸶳� �����°��� timeGetTime�κ����� 
				������ �����Ѵ�. �׷��� timeGetTime�� �츮�� setter�� �� �����Ƿ�,
				_delay_start_time�� ���� �� �� ����.
		*/
	private:
		int _delay_start_time;
	};
}