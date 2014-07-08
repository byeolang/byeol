/*


	������Ʈ��:	���ӿ��� ���
	���� ���:	C++
	���� ��:	VC++ 2008 SP1 �ѱ���, DirectX SDK 2010 JUNE
	���� ȯ��:	Window 7 x64
	
	�޸�:	
	����:	2010-07-05	���� ����			
			2010-07-08	build 2
			2010-07-13	build 3 
			2010-07-16	build 4
			2010-07-27	build 5		���� ���̻� ������ڴٰ� �ߴµ�... ������ 5�� ������ ^_^
*/

#include <Windows.h>
#include "CHUB.hpp"
#include "CWindow.hpp"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	try
	{	
		//	�ʱ�ȭ:
		CHUB	hub(hInstance, lpCmdLine, nCmdShow);
		//	���� �⵿:
		hub.handleMessage();
	}
	//	 �ɰ��� ������ ���⼭ �޼����ڽ��� ���� ���α׷��� �����Ѵ�
	catch(CError e)
	{
		e.showMessage();
		e.log();
	}
}


