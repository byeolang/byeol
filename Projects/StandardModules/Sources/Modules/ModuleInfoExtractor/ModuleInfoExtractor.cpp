#include "ModuleInfoExtractor.hpp"

namespace NE
{
	NE_DLL ModuleInfoExtractor::ModuleInfoExtractor()
		: NEModule()
	{

	}

	NE_DLL ModuleInfoExtractor::ModuleInfoExtractor(const ModuleInfoExtractor& source)
		: NEModule(source)
	{

	}

	NE_DLL ModuleInfoExtractor::~ModuleInfoExtractor()
	{

	}

	type_result NE_DLL ModuleInfoExtractor::initialize()
	{
		getKeySet().create(3);
		getKeySet().push(NEIntKey());	//	����ڵ�
		getKeySet().push(NEUByteKey());	//	�������� �������� Ÿ��
		getKeySet().push(NETStringKey());	//	OUT ���� ������


		return RESULT_SUCCESS;
	}

	type_result NE_DLL ModuleInfoExtractor::execute()
	{
		//	pre:		
		const NEModuleSet& ms = Kernal::getInstance().getModuleManager().getModuleSet();
		type_int code = getModuleCode();
		const NEModule& m = ms[code];
		if( ! &m)
		{
			ALERT_ERROR("�־��� ����ڵ�(Ű#0 : %d)�� �߸��Ǿ����ϴ�.\n����ڵ��� ����(0 ~ %d)�� ���� ���� �ƴѰ���?", code, ms.getLengthLastIndex());
			return RESULT_TYPE_ERROR;
		}
		const NEModule::Header& head = m.getHeader();


		//	post:
		const NETString* rhs = 0;
		switch(getInfoType())
		{
		case INFOTYPE_NAME:			rhs = &head.getName();					break;
		case INFOTYPE_DEVELOPER:	rhs = &head.getDeveloper();				break;
		case INFOTYPE_REVISION:		rhs = &NETString(head.getRevision());	break;
		case INFOTYPE_RELEASE_DATE:	rhs = &head.getReleaseDate();			break;
		case INFOTYPE_VERSION:		rhs = &head.getVersion();				break;
		case INFOTYPE_COMMENT:		rhs = &head.getComment();				break;
		case INFOTYPE_CONTACT:		rhs = &head.getContact();				break;
		
		default:
			ALERT_WARNING("�߸��� InfoType(%d) �Դϴ�.", getInfoType());
		}
		
		getExtracted() = *rhs;
		return RESULT_SUCCESS;
	}

	NEObject NE_DLL &ModuleInfoExtractor::clone() const
	{
		return *(new ModuleInfoExtractor(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &ModuleInfoExtractor::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("ModuleInfoExtractor");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. ������ ����� ScriptCode \n1��. �������� ������ ����(0:�̸�, 1:�����ڸ�, 2:������ȣ, 3:������¥, 4:����, 5:�ڸ�Ʈ, 6:����ó");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-05");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(0);
		}

		return _header;
	}
}