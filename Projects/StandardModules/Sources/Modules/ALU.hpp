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
		NETArgument<NEKey> arg_left_operand;
		NETArgument<NEKey> arg_right_operand;
		NETArgument<NEKey> arg_target;
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
					"����� \"%\"�� ������ �����ڷν�, 10 % 3 �� 10�� 3���� ������ ���� �������� 1�� ������ �˴ϴ�.";
				_header.getVersion()  = _T("0.0.1a");
				_header.getReleaseDate() = _T("2014-11-27");
				_header.getModuleDependencies().create(0);
				_header.setMaxErrorCodeCount(0);
				NETStringSet& args = _header.getArgumentsComments();
				args.create(4);
				args.push("LeftOperand\n������ �� ���1 �Դϴ�.\n");
				args.push("RightOperand\n������ �� ���2 �Դϴ�.\n");
				args.push("Target\n������� �� Key�Դϴ�.");
				args.push("Operator\nLeftOperand�� RightOperand���� � ������ �������� �����ϴ�. \n������ �����ڸ� ���� �ֽ��ϴ� : \"+\" \"-\", \"*(����)\", \"/\"(������), \"%\"(����������), \"^\"n����, \"<\", \"<=\", \">\", \">=\", \"==\", \"!=\"");
			}

			return _header;
		}		

	protected:
		virtual type_result _onExecute()
		{
			static	NETStringSet opers(12);
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
			}

			NEKey	&lkey	= arg_target.getValueKey(),
					&rkey1	= arg_left_operand.getValueKey(),
					&rkey2	= arg_right_operand.getValueKey();

			const NETString& op = arg_method.getValue();
			type_index oper_idx = opers.find(op);

			type_result result = RESULT_SUCCESS;

			switch(oper_idx)
			{
			case 0:		result = rkey1.add(lkey, rkey2);		break;
			case 1:		result = rkey1.sub(lkey, rkey2);		break;
			case 2:		result = rkey1.multiply(lkey, rkey2);	break;
			case 3:		result = rkey1.divide(lkey, rkey2);	break;
			case 4:						
				break;

			case 5:
				{
					NEIntKey count(rkey2);
					type_float	converted = NEFloatKey(rkey2).getValue();
					NEFloatKey calculated = 1.0f;
					for (int n = 1; n <= count.getValue(); n++)
						calculated.getValue() *= converted;

					result = arg_target.getValueKey().assign(calculated);
				}
				break;

			case 6:		result |= _applyLogicOperation(NEBooleanKey(lkey < rkey2));	break;
			case 7:		result |= _applyLogicOperation(NEBooleanKey(lkey <= rkey2));	break;
			case 8:		result |= _applyLogicOperation(NEBooleanKey(lkey > rkey2));	break;
			case 9:		result |= _applyLogicOperation(NEBooleanKey(lkey >= rkey2));	break;
			case 10:	result |= _applyLogicOperation(NEBooleanKey(lkey == rkey2));	break;
			case 11:	result |= _applyLogicOperation(NEBooleanKey(lkey != rkey2));	break;
			}

			return result;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_left_operand);
			tray.push(arg_right_operand);
			tray.push(arg_target);
			tray.push(arg_method);

			return RESULT_SUCCESS;
		}

	private:
		type_result _applyLogicOperation(const NEBooleanKey& result)
		{
			arg_target.getValueKey() = result;

			return result.getValue() ? RESULT_TRUE : RESULT_FALSE;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ALU(*this));
		}
	};
}