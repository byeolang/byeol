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
#include "../NEStringSet/NEStringSet.hpp"

namespace NE
{
	template
	class NE_DLL NETKey<NEStringSet, NEType::NESTRING_SET_KEY>;
	typedef NETKey<NEStringSet, NEType::NESTRING_SET_KEY> NEStringSetKey;
}

#pragma warning(pop)