#include <Windows.h>
#include <tchar.h>


//	���������ν���:
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // WM_QUIT�� ����
		return(0);
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}
