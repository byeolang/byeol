#include "CharInputSpy.hpp"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

namespace
{
	using namespace NE;
	void _pipeInputToString(NETStringKey& key, TCHAR inputed)
	{
		NETString& casted = key.getValue();

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
			break;

		case VK_RETURN:
			if(casted.getLength() == casted.getSize())
				casted.resize(casted.getSize() * 2);
			
			casted += '\n';
			break;

		default:				
			if(casted.getLength() == casted.getSize())
				casted.resize(casted.getSize() * 2);

			casted += inputed;
		}
	}
}

namespace NE
{
	type_result CharInputSpy::execute()
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

			_pipeInputToString(arg_sentence.getKey(), inputed);

			_delay_start_time = cur_time;
			sel.initializeReferingPoint();
			return RESULT_TRUE;	
		}


		//	post:
		return RESULT_FALSE;
	}
}	