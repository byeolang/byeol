#include "CharInputSpy.hpp"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

namespace
{
	using namespace NE;
	void _pipeInputToString(NETArgument<NETStringKey>& arg, TCHAR inputed)
	{
		NETString casted = arg.getValue();

		switch(inputed)
		{
		case VK_BACK:	//	�齺���̽�
			//	���� �����ϱ�:
			//		���ڸ� �����ϱ� ���ؼ��� �ܼ��� pop�� �ƴ϶� �ι��ڸ� ����� ��� �Ѵ�.
			if(casted.getLength() > 0) 
			{
				casted[casted.getLengthLastIndex()] = 0;
				casted.pop();
			}
			arg.setValue(casted);
			break;

		case VK_RETURN:
			if(casted.getLength() == casted.getSize())
				casted.resize(casted.getSize() * 2);
			
			casted += '\n';
			arg.setValue(casted);
			break;

		default:				
			if(casted.getLength() == casted.getSize())
				casted.resize(casted.getSize() * 2);

			casted += inputed;
			arg.setValue(casted);
		}
	}
}

namespace NE
{
	type_result CharInputSpy::_onExecute()
	{
		NEModuleSelector& sel = arg_window_selector.getValue();
		NEModule& m = sel.getModule();
		sel.initializeReferingPoint();
		if( ! &m)	return RESULT_FALSE;
		Window& win = _castToWindow(m);
		if( ! &win) return RESULT_FALSE;
	

		//	main:
		type_uint	end_time = _delay_start_time + arg_max_delay.getValue(),
					cur_time = timeGetTime();
		const MSG& msg = win.getMSG();
		if(	msg.message == WM_CHAR	&& 
			cur_time >= end_time	)
		{
			TCHAR inputed = (TCHAR) msg.wParam;

			_pipeInputToString(arg_sentence, inputed);

			_delay_start_time = cur_time;
			sel.initializeReferingPoint();
			return RESULT_TRUE;	
		}


		//	post:
		return RESULT_FALSE;
	}

	type_result CharInputSpy::_onFetchModule()
	{
		arg_window_selector.getValue().isUsingAutoBinding() = true;

		return arg_max_delay.setDefault(500);
	}

	type_result CharInputSpy::_onFetchArguments(NEArgumentList& tray)
	{
		tray.push(&arg_window_selector);
		tray.push(&arg_sentence);
		return tray.push(&arg_max_delay);
	}

	CharInputSpy::CharInputSpy()
		: WNDSpy(), _delay_start_time(0)
	{

	}

	const NEExportable::ModuleHeader& CharInputSpy::getHeader() const
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

	void CharInputSpy::release()
	{
		SuperClass::release();

		_delay_start_time = 0;
	}

	NEObject& CharInputSpy::clone() const
	{
		return *(new ThisClass(*this));
	}

}	