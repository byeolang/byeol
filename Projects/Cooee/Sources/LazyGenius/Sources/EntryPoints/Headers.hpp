#pragma once

#include "../Modules/Core/Core.hpp"
#include "../Modules/ListGliph/ListGliph.hpp"
#include "../Modules/TextGliph/TextGliph.hpp"
#include "../Modules/SwitchGliph/SwitchGliph.hpp"
#include "../Modules/MessageWindow/MessageWindow.hpp"
#include "../Modules/QueryWindow/QueryWindow.hpp"
#include "../Modules/InputWindow/InputWindow.hpp"
#include "../Modules/ListWindow/ListWindow.hpp"
#include "../Commons/Defines/ClassMacros/ClassMacros.hpp"
#include "../Modules/FloatingGliph/FloatingGliph.hpp"
#include "../Modules/GearGliph/GearGliph.hpp"
#include "../Modules/TextListWindow/TextListWindow.hpp"


//	NEModule�� kernal�� �־�� �ϴ� ����
//	���ø�Ŭ�����̱� �����̴�. ���ø�Ŭ������ Ư���� �ν��Ͻ��� �����ϸ鼭 �����ȴ�. NE_DLL�� �̹� �����ϴ� DLL�� �ν��Ͻ��� �������� ��ũ��.
//	������ ���ø� Ŭ������ DLL���� ����ϰ� �ʹٸ� 2������ ���ؾ��Ѵ�.
//		1. DLL ���ο� �ν��Ͻ��� �̸� �����ϰ� _declspec(dllimport)�� �ܺο��� ������ ����
//		2. �ܺο��� ���ø� Ŭ���� �ν��Ͻ��� �����Ѵ�. �̶� ���ø�Ŭ������ NE_DLL�� ���� �ܺο��� include�Ҷ� _declspec(dllimport)�� ��ȯ�ǹǷ�
//		NE_DLL ���� ����� �ۼ��ؾ� �ϰ�, �׷��ԵǸ� ���ο��� ������ Ŭ�������ø��� export�ؾ��ϴ� ����� �����ؾ� �Ұ��̴�.
//	���⼭�� ���ø�Ŭ������ ����� DLL�� ���� NEModule�� ���������ν� ���ŷο� 2�� ���, 1���� ���ߴ�.

#ifndef LAZYGENIUSFRAMEWORK_EXPORTS
#	ifdef DEVELOPER
#		ifdef _DEBUG
#			ifdef _UNICODE
#				pragma comment(lib, "LazyGeniusFramework.dev.dbg.lib")
#			else
#				pragma comment(lib, "LazyGeniusFramework.dev.dbg.mbcs.lib")
#			endif
#		else
#			ifdef _UNICODE
#				pragma comment(lib, "LazyGeniusFramework.dev.lib")
#			else
#				pragma comment(lib, "LazyGeniusFramework.dev.mbcs.lib")
#			endif
#		endif
#	else
#		ifdef _UNICODE
#			pragma comment(lib, "LazyGeniusFramework.lib")
#		else
#			pragma comment(lib, "LazyGeniusFramework.mbcs.lib")
#		endif
#	endif
#endif
