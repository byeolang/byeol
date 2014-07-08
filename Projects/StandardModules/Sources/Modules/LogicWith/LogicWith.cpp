#include "LogicWith.hpp"
#include "define/define.hpp"

namespace NE
{
	NE_DLL LogicWith::LogicWith()
		: NEModule()
	{

	}

	NE_DLL LogicWith::LogicWith(const LogicWith& source)
		: NEModule(source)
	{

	}

	NE_DLL LogicWith::~LogicWith()
	{

	}

	type_result NE_DLL LogicWith::initialize()
	{
		getKeySet().create(_KEY_NUMBER);
		getKeySet().push( NEKeySelector() );
		getKeySet().push( NEKeySelector() );
		getKeySet().push( NEIntKey(OPERATOR_IS_EQUAL_TO) );

		return RESULT_SUCCESS;
	}

	type_result NE_DLL LogicWith::execute()
	{
		//	pre:
		NEKeySelector&	source_selector		=	static_cast<NEKeySelector&>(	getKeySet()[0]	),
			&	target_selector		=	static_cast<NEKeySelector&>(	getKeySet()[1]	);
		NEIntKey	 &	operate_type_key	=	static_cast<NEIntKey&>(			getKeySet()[2]	);
		int operate_type = operate_type_key.getValue();
		if( operate_type < OPERATOR_IS_EQUAL_TO || operate_type >= OPERATOR_END)
		{
			ALERT_ERROR("�߸��� ���� Ÿ���Դϴ�.\n�־��� ���� Ÿ�� : %d", operate_type)
				return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION | INVALID_OPERATOR_TYPE;
		}


		//	main:
		while(NEKey* source_key = &source_selector.getKey())
		{
			while( NEKey* target_key = &target_selector.getKey())
			{
				//	���� ����:
				//		���� ����� ���� ����:
				bool result_of_operation = false;
				//		����:
				switch(operate_type)
				{
				case OPERATOR_IS_EQUAL_TO:
					result_of_operation = (*source_key == *target_key);
					break;
				case OPERATOR_IS_NOT_EQUAL_TO:
					result_of_operation = (*source_key != *target_key);
					break;
				case OPERATOR_IS_LESS_THAN:
					result_of_operation = (*source_key < *target_key);
					break;
				case OPERATOR_IS_LESS_THAN_OR_EQUAL_TO:
					result_of_operation = (*source_key <= *target_key);
					break;
				case OPERATOR_IS_MORE_THAN:
					result_of_operation = (*source_key > *target_key);
					break;
				case OPERATOR_IS_MORE_THAN_OR_EQUAL_TO:
					result_of_operation = (*source_key >= *target_key);
					break;
				default:
					ALERT_ERROR("�߸��� ���� Ÿ���Դϴ�.\n�־��� ���� Ÿ�� : %d", operate_type)
						return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION | INVALID_OPERATOR_TYPE;
				}
				//	��� �м�:
				//		�ϳ��� false�� �ȴٸ� �˻� ����:	��� Key1�� ���ؼ� true�� �Ǿ߸� true�� ��ȯ�Ѵ�.
				if( ! result_of_operation)
				{
					target_selector.initializeReferingPoint();
					source_selector.initializeReferingPoint();
					return RESULT_SUCCESS | RESULT_FALSE | RESULT_ABORT_ACTION;
				}
			}
		}


		return RESULT_SUCCESS | RESULT_TRUE;
	}

	NEObject NE_DLL &LogicWith::clone() const
	{
		return *(new LogicWith(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &LogicWith::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T( _NAME );
			_header.getDeveloper() = _T( _DEVELOPER );
			_header.setRevision(_REVISION);
			_header.getComment() = _T( _COMMENT );
			_header.getVersion()  = _T( _VERSION );
			_header.getReleaseDate() = _T( _DATE );
			_header.getModuleDependencies().create(_DEPENDENCIES_COUNT);
			_header.setMaxErrorCodeCount(LogicWith::ERROR_CODE_END - 1);
		}

		return _header;
	}

	LPCTSTR NE_DLL LogicWith::getErrorMessage(type_errorcode errorcode) const
	{
		switch(errorcode)
		{
		case MODULE_IS_NOT_VALID:
			return _T("�߸��� Module");

		case THERE_IS_NO_SOURCE_KEY:
			return _T("�ҽ�Ű(0��) ��� Ű�� ������� �ʾҽ��ϴ�");

		case INVALID_OPERATOR_TYPE:
			return _T("�߸��� ���� Ÿ���Դϴ�.");

		default:
			return _T("�˼� ���� ����");
		}
	}
}