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
#include "../NETStringKeyize/NETStringKeyize.hpp"

namespace NE
{
	template
	class NE_DLL NETStringKeyize<NEWString, NEType::NEWSTRING_KEY>;
	typedef NETStringKeyize<NEWString, NEType::NEWSTRING_KEY> NEWStringKey;
}

#pragma warning(pop)