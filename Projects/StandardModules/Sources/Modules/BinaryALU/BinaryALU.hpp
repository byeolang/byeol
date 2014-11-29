#pragma once

#include "../../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL BinaryALU : public NEModule
	{
	public:
		typedef BinaryALU ThisClass;
		typedef NEModule SuperClass;

	public:
		NETArgument<NEKey> left_operand;
		NETArgument<NEKey> right_operand;
		NETArgument<NEKey> target;
		NETArgument<NEIntKey> method;

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("BinaryALU");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() = _T("Key ���� �⺻���� 2�� ��������� �����մϴ�.\n\
										  ����(0), �y��(1), ����(2), ������(3),	����������(4), n����(5) ���� ������ �� �ֽ��ϴ�.\n\
										  \"+\" �� ���, Target = LeftOperand + RightOperand���� ����˴ϴ�.\n\
										  \n���� \"^\"�� ���, Target = LeftOperand�� RightOperand�������� ����ǹǷ�,\n\
										  LightOperand = 10, RightOperand = 2�� ��� Target�� 100�� �˴ϴ�.\n\
										  ����, Target�� Operand�� ���� Ű�� �� ���� �ֽ��ϴ�. \n\
										  ��) Key1 = Key1 + Key2 \n\
										  �ٸ� Target�� �迭�� ��쿡�� ����� ���������Ƿ� ���ǹٶ��ϴ�.");
				_header.getVersion()  = _T("0.0.1a");
				_header.getReleaseDate() = _T("2014-11-27");
				_header.getModuleDependencies().create(0);
				_header.setMaxErrorCodeCount(0);
				NETStringSet& args = _header.getArgumentsComments();
				args.create(4);
				args.push("LeftOperand\n������ �� ���1 �Դϴ�.\n");
				args.push("RightOperand\n������ �� ���2 �Դϴ�.\n");
				args.push("Target\n������� �� Key�Դϴ�.");
				args.push("Operator\LeftOperand�� RightOperand���� � ������ �������� ���ڷ� �����ݴϴ�. \n(0: ����(+), 1: �y��(-), 2: ����(*), 3: ������(/), 4: ������(%), 5: n����(^), 6: <, 7: <=, 8: >, 9: >=, 10: ==, 11: !=");
			}

			return _header;
		}		

	protected:
		virtual type_result _onExecute()
		{
			const type_float _ECLIPSE = 0.000000001f;
			type_float	lvalue = left_operand.getValue(),
				rvalue = right_operand.getValue();

			switch (method.getValue())
			{
			case 0:
			default:	target.getValue() = lvalue + rvalue;  break;

			case 1:		target.getValue() = lvalue - rvalue;  break;
			case 2:		target.getValue() = lvalue * rvalue;  break;
			case 3:		
				if (rvalue.getValue() == 0.0f)
					rvalue = _ECLIPSE;
				target.getValue() = lvalue / rvalue; 
				break;

			case 4:		
				if (rvalue.getValue() == 0.0f)
					rvalue = _ECLIPSE;				
				target.getValue() = lvalue % rvalue; 
				break;

			case 5:
				{
					type_float multipled_lvalue = 1;
					for (int n = 1; n <= rvalue; n++)
						multipled_lvalue *= lvalue;

					target.getValue() = multipled_lvalue;
				}
				break;
			}

		}
		virtual type_result _onFetchArgument(NEArgumentInterfaceList& tray)
		{
			tray.push(left_operand);
			tray.push(right_operand);
			tray.push(target);
			tray.push(method);

			return tray;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new BinaryALU(*this));
		}
	};
}