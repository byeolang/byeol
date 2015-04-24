#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL SLU : public NEModule
	{
	public:
		typedef SLU ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NETStringKey> arg_left_operand;
		NETArgument<NETStringKey> arg_right_operand;
		NETArgument<NETStringKey> arg_target;
		NETArgument<NEStringKey> arg_method;

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("SLU");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() = "String ���� ������ �����մϴ�.\n"
					"�߰�, ��, �Ҵ��� ������ �� �ֽ��ϴ�.\n"
					"�������, \"+\" �� ���, Target = LeftOperand + RightOperand���� ����˴ϴ�.\n"
					"����, Target�� Operand�� ���� Ű�� �� ���� �ֽ��ϴ�. \n"
					"��) Key1 = Key1 + Key2 \n\n"					
					"�׿ܿ��� =�� ==, !=�� ����� �����մϴ�.";
				_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2015-04-23");
				_header.getModuleDependencies().create(0);
				_header.setMaxErrorCodeCount(0);
				NETStringSet& args = _header.getArgumentsComments();
				args.create(4);
				args.push("LeftOperand\n������ �� ���1 �Դϴ�.\n");
				args.push("RightOperand\n������ �� ���2 �Դϴ�.\n");
				args.push("Target\n������� �� Key�Դϴ�.");
				args.push("Operator(������)\n��밡���� ������ : \"+\", \"==\", \"!=\", \"=\"");
			}

			return _header;
		}

	protected:
		virtual type_result _onExecute()
		{
			static	NETStringSet opers(4);
			if (opers.getLength() != opers.getSize())
			{
				opers.push("+");	//	idx 0
				opers.push("==");
				opers.push("!=");
				opers.push("=");
			}

			const NETString&	op = arg_method.getValue(),
				&	loperand = arg_left_operand.getValue(),
				&	roperand = arg_right_operand.getValue();
			type_index oper_idx = opers.find(op);

			type_result result = RESULT_SUCCESS;
			NETString rvalue;
			type_bool return_value = false;

			switch (oper_idx)
			{
			case 0:		arg_target.setValue( rvalue = loperand + roperand);		break;
			case 1:		arg_target.setValue( rvalue = loperand == roperand);	break;
			case 2:		arg_target.setValue( rvalue = loperand != roperand);	break;
			case 3:		
				rvalue = roperand;
				arg_left_operand.setValue(roperand);
				arg_target.setValue(roperand);
				break;			
			}

			static const NETString true_string("true");

			if(rvalue == true_string)
				return RESULT_SUCCESS | RESULT_TRUE;

			return RESULT_SUCCESS | RESULT_FALSE;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_left_operand);
			tray.push(arg_right_operand);
			tray.push(arg_target);
			tray.push(arg_method);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			return arg_method.setPurposeLimitation(NEArgumentBase::READ_BY);
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new SLU(*this));
		}
	};
}