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
		NETArgument<NEModuleSelector> arg_window_selector;
		NETArgument<NETStringKey> arg_sentence;
		NETArgument<NEUIntKey> arg_max_delay;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray);
		virtual type_result _onFetchModule();
		virtual type_result _onExecute();

	public:
		CharInputSpy();

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;

	public:
		virtual void release();
		virtual NEObject& clone() const;
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