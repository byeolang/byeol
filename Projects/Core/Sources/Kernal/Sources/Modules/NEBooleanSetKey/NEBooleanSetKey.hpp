//	����	:	NETKey�� ����� ����� ���ø� �ν��Ͻ�ȭ.
//				Kernal�� NETKey�� �ν��Ͻ��� �̸� ����� ����, �ܺο��� �̸�
//				include �ؼ� ����Ѵ�.
//				���� �ܺο��� ���� NETKey�� �ν��Ͻ��� ����� ����ϰ� �����ϰ�
//				�Ѵٸ�, �� ��⸶�� NETKey���� �ν��Ͻ��� ���� �ǹǷ� ��ü��
//				���� ����� ũ�Ⱑ �þ�� �ɰ��̴�.
//	�޸�	:
//	�����丮:	2011-06-04	������	���� �Ϸ�
//	---------------------------------------------------------------------------------
#pragma once
#pragma warning(push)
#pragma warning(disable : 4800)
#pragma warning(disable : 4804)

//	include:
#include "../NETKey/NETKey.hpp"
#include "../NEBooleanSet/NEBooleanSet.hpp"

namespace NE
{
	template
	class NE_DLL NETKey<NEBooleanSet>;
	typedef NETKey<NEBooleanSet> NEBooleanSetKey;
}

#pragma warning(pop)