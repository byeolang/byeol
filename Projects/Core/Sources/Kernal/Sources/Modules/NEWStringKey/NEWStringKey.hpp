//	����	:	NEKeyTemplate�� ����� ����� ���ø� �ν��Ͻ�ȭ.
//				Kernal�� NEKeyTemplate�� �ν��Ͻ��� �̸� ����� ����, �ܺο��� �̸�
//				include �ؼ� ����Ѵ�.
//				���� �ܺο��� ���� NEKeyTemplate�� �ν��Ͻ��� ����� ����ϰ� �����ϰ�
//				�Ѵٸ�, �� ��⸶�� NEKeyTemplate���� �ν��Ͻ��� ���� �ǹǷ� ��ü��
//				���� ����� ũ�Ⱑ �þ�� �ɰ��̴�.
//	�޸�	:
//	�����丮:	2011-06-04	������	���� �Ϸ�
//	---------------------------------------------------------------------------------
#pragma once
#pragma warning(push)
#pragma warning(disable : 4800)
#pragma warning(disable : 4804)

//	include:
#include "../../Commons/Templates/NEStringKeyTemplate/NEStringKeyTemplate.hpp"

namespace NE
{
	template
	class NE_DLL NEStringKeyTemplate<NEWString, NEType::NEWSTRING_KEY>;
	typedef NEStringKeyTemplate<NEWString, NEType::NEWSTRING_KEY> NEWStringKey;
}

#pragma warning(pop)