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
#include "../NETArithmeticKey/NETArithmeticKey.hpp"

namespace NE
{
	template
	class NE_DLL NETArithmeticKey<type_int, NEType::NEINT_KEY>;
	typedef NETArithmeticKey<type_int, NEType::NEINT_KEY> NEIntKey;
}

#pragma warning(pop)