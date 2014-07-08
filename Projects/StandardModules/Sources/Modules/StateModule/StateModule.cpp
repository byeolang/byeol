#include "StateModule.hpp"

namespace NE
{
	NE_DLL StateModule::StateModule()
		: NEModule()
	{
				
	}

	NE_DLL StateModule::StateModule(const StateModule& source)
		: NEModule(source)
	{
		
	}

	NE_DLL StateModule::~StateModule()
	{

	}
	
	type_result NE_DLL StateModule::initialize()
	{
		getKeySet().create(2);
		getKeySet().push(NEModuleCodeSetKey());
		getKeySet().push(NEIntKey());
		
		return RESULT_SUCCESS;
	}

	type_result NE_DLL StateModule::execute()
	{
		NEModuleCodeSet& actions = getActions();
		type_int state = getState();
		if( state < 0 || state > actions.getLengthLastIndex())
		{
			ALERT_WARNING("�ε��� ���� �ʰ� �Դϴ�.");
			return RESULT_TYPE_WARNING;
		}


		//	main:
		//		Enable ������Ʈ:
		//			�� �ٷ� actions[state].execute()�� �������� �ʴ°�?:
		//				�� ��� LocalStack�� RecentModuleCodeSet�� &actions��
		//				������� �ʱ� �����̴�. �Դٰ� LocalStack�� �ܺο���
		//				���� �� �� ����. ������ �̷������� ����Ѵ�.
		for(int n=0; n < actions.getSize(); n++)			
			actions[n].setEnable(n == state ? true : false);
		return actions.execute();
	}

	NEObject NE_DLL &StateModule::clone() const
	{
		return *(new StateModule(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &StateModule::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("StateModule");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��: ���¿� ���� �����ϰ� �� ����(���°� 1�̸� 1�� �ε����� ��⸸ �����)\n1��: ���� �ε���");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-15");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(0);
		}

		return _header;
	}
}