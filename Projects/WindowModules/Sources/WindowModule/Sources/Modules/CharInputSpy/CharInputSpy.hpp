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
		NEArgumentInterfaceTemplate<NEModuleSelector> arg_window_selector;
		NEArgumentInterfaceTemplate<NETStringKey> arg_sentence;
		NEArgumentInterfaceTemplate<NEUIntKey> arg_max_delay;

	protected:
		virtual type_result _onArgumentsFetched(NEArgumentInterfaceList& tray)
		{
			arg_window_selector.getValue().isUsingAutoBinding() = true;
			arg_max_delay.getDefault().getValue() = 500;
			tray.push(&arg_window_selector);
			tray.push(&arg_sentence);
			return tray.push(&arg_max_delay);
		}

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
				_header.getComment() = "0�� �����츦 ���ؼ� ����ڰ� �Է¹��� ���ڸ� 1���� �����մϴ�.\nŰ�� �Էµɶ����� TRUE�� ��ȯ�մϴ�.\nKeyboardSpy�� � Ű�� ���������� �Ǵ��Ѵٸ�, CharInputSpy�� � ���ڰ� �ԷµǾ������� �Ǵ��Ѵٴ� ���� ���̰� ������ ������ �μ���.\n\t0��Ű:	Spying �� ������(\"�ڵ� ���ε�\"�� �⺻��)\n\t1��Ű: �Էµ� ����";
				_header.getReleaseDate() = "2014-01-29";
			}

			return _header;
		}

	public:
		virtual type_result execute();

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