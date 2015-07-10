#pragma once

#include "../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ALU : public NEModule
	{
	public:
		typedef ALU ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEFloatKey> arg_left_operand;
		NETArgument<NEFloatKey> arg_right_operand;
		NETArgument<NEFloatKey> arg_target;
		NETArgument<NEStringKey> arg_method;

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("ALU");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() = "Key ���� �⺻���� 2�� ��������� �����մϴ�.\n"
					"����, �y��, ����, ������,	����������, n���� ���� ������ �� �ֽ��ϴ�.\n"
					"�������, \"+\" �� ���, Target = LeftOperand + RightOperand���� ����Ǹ�,\n"
					"\n���� \"^\"�� ���, Target = LeftOperand�� RightOperand�������� ����ǹǷ�,\n"
					"LightOperand = 10, RightOperand = 2�� ��� Target�� 100�� �˴ϴ�.\n"
					"����, Target�� Operand�� ���� Ű�� �� ���� �ֽ��ϴ�. \n"
					"��) Key1 = Key1 + Key2 \n\n"
					"�ٸ� Target�� �迭�� ��쿡�� �������ϹǷ� ���ǹٶ��ϴ�.\n"
					"����� \"%\"�� ������ �����ڷν�, 10 % 3 �� 10�� 3���� ������ ���� �������� 1�� ������ �˴ϴ�.\n"
					"���� ������ =�� = ! �� ����� �����մϴ�.";
				_header.getVersion()  = _T("0.0.1a");
				_header.getReleaseDate() = _T("2014-11-27");
				_header.getModuleDependencies().create(0);
				_header.setMaxErrorCodeCount(0);
				NETStringSet& args = _header.getArgumentsComments();
				args.create(4);
				args.push("LeftOperand\n������ �� ���1 �Դϴ�.\n");
				args.push("RightOperand\n������ �� ���2 �Դϴ�.\n");
				args.push("Target\n������� �� Key�Դϴ�.");
				args.push("Operator(������)\n��밡���� ������ : \"+\" \"-\", \"*(����)\", \"/\"(������), \"%\"(����������), \"^\"n����, \"<\", \"<=\", \">\", \">=\", \"==\", \"!=\", \"=\", \"= !\"");
			}

			return _header;
		}		

	protected:
		virtual type_result _onExecute()
		{
			static	NETStringSet opers(14);
			if (opers.getLength() != opers.getSize())
			{
				opers.push("+");	//	idx 0
				opers.push("-");
				opers.push("*");
				opers.push("/");
				opers.push("%");

				opers.push("^");	//	5
				opers.push("<");
				opers.push("<=");
				opers.push(">");
				opers.push(">=");

				opers.push("==");	//	10
				opers.push("!=");
				opers.push("=");
				opers.push("= !");
			}

			type_float	loperand = arg_left_operand.getValue(),
				roperand = arg_right_operand.getValue();

			const NETString& op = arg_method.getValue();
			type_index oper_idx = opers.find(op);

			type_result result = RESULT_SUCCESS;

			switch(oper_idx)
			{
			case 0:		arg_target.setValue(loperand + roperand);		break;
			case 1:		arg_target.setValue(loperand - roperand);		break;
			case 2:		arg_target.setValue(loperand * roperand);		break;
			case 3:		arg_target.setValue(loperand / roperand);		break;
			case 4:		arg_target.setValue((float)((int) loperand % (int) roperand));		break;
			case 5:
				{
					type_float calculator = 1.0f;
					for (int n = 0; n <= roperand ;n++)
						calculator *= loperand;
					arg_target.setValue(calculator);
				}
				break;

			case 6:		arg_target.setValue(loperand < roperand);		break;
			case 7:		arg_target.setValue(loperand <= roperand);	break;
			case 8:		arg_target.setValue(loperand > roperand);		break;
			case 9:		arg_target.setValue(loperand >= roperand);	break;
			case 10:	arg_target.setValue(loperand == roperand);	break;
			case 11:	arg_target.setValue(loperand != roperand);	break;
			case 12:	
				arg_left_operand.getKey() = arg_right_operand.getKey();
				arg_target.getKey() = arg_right_operand.getKey();
				break;

			case 13:
				arg_left_operand.setValue( ! roperand);
				arg_target.setValue( ! roperand);
			}

			if(arg_target.getValue() == 0.0f)
				return RESULT_SUCCESS | RESULT_FALSE;

			return RESULT_SUCCESS | RESULT_TRUE;
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
			return *(new ALU(*this));
		}
	};
}