//	����	:	NETArithmeticKey�� ����� ����� ���ø� �ν��Ͻ�ȭ.
//				Kernal�� NETArithmeticKey�� �ν��Ͻ��� �̸� ����� ����, �ܺο��� �̸�
//				include �ؼ� ����Ѵ�.
//				���� �ܺο��� ���� NETArithmeticKey�� �ν��Ͻ��� ����� ����ϰ� �����ϰ�
//				�Ѵٸ�, �� ��⸶�� NETArithmeticKey���� �ν��Ͻ��� ���� �ǹǷ� ��ü��
//				���� ����� ũ�Ⱑ �þ�� �ɰ��̴�.
//	�޸�	:
//	�����丮:	2011-06-04	������	���� �Ϸ�
//	---------------------------------------------------------------------------------
#pragma once
#pragma warning(push)
#pragma warning(disable : 4800)
#pragma warning(disable : 4804)

//	include:
#include "../../Commons/Templates/NETArithmeticKey/NETArithmeticKey.hpp"

namespace NE
{
	template
	class NE_DLL NETArithmeticKey<type_short, NEType::NESHORT_KEY>;
	typedef NETArithmeticKey<type_short, NEType::NESHORT_KEY> NEShortKey;
}

#pragma warning(pop)