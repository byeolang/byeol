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
#include "../NENodeCodeSet/NENodeCodeSet.hpp"

namespace NE
{
	template
	class NE_DLL NETKey<NENodeCodeSet, NEType::NENODE_CODESET_KEY>;
	typedef NETKey<NENodeCodeSet, NEType::NENODE_CODESET_KEY> NENodeCodeSetKey;
}

#pragma warning(pop)