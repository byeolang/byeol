//���� ����
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6
//������������������������������������������������������������������������������
#include "defines.h"
#include ".\maingame.h"


//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6 ��������
//������������������������������������������������������������������������������
HINSTANCE				g_hInst;									//���� �ν��Ͻ��� �ڵ鼱��
char  						g_ClassName[128] = "Narusia";		//Ŭ���� �̸�
char  						g_WinName[128] = "Narusia";			//������ �̸�
HWND					g_hWnd;									//�������� �ڵ�
LPDIRECT3D9			g_pD3D;
LPDIRECT3DDEVICE9	g_pd3dDevice;
CMainGame				g_MainGame;

//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6 �Լ�
//������������������������������������������������������������������������������
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
bool SetUp(char *szError);

//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6
//������������������������������������������������������������������������������
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam, int nCmdShow)
{
	MSG			Message;		//�޼��� ����ü ����
	WNDCLASS	WndClass;		//������ Ŭ���� ����
	g_hInst =	hInstance;		//�ν��Ͻ��� ��´�
	char			szError[128];

	WndClass.cbClsExtra		= 0;									//���и޸�	
	WndClass.cbWndExtra		= 0;									//���и޸�
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ����
	WndClass.hCursor		= LoadCursor(NULL,IDC_ARROW);			//Ŀ���� �д´�
	WndClass.hIcon			= LoadIcon(NULL,IDI_APPLICATION);		//�������� �д´�
	WndClass.hInstance		= hInstance;							//������ Ŭ������ �ν��Ͻ�
	WndClass.lpfnWndProc	= (WNDPROC)WndProc;						//������ ���ν����� �̸�
	WndClass.lpszClassName	= g_ClassName;							//������ Ŭ������ �̸�
	WndClass.lpszMenuName	= NULL;									//�޴��� �̸�
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;				//�������� ��Ÿ��
	RegisterClass(&WndClass);										//Ŭ������ ����Ѵ�

	//�����츦 �����
	g_hWnd = CreateWindow(g_ClassName, g_WinName, WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, int(WINSIZEX), int(WINSIZEY),
		NULL, (HMENU)NULL, hInstance, NULL);
	if(SetUp(szError) == false)
	{
		MessageBox(g_hWnd, szError, "����", MB_OK);
		return 0;
	}
	ShowWindow(g_hWnd, nCmdShow);

	while(true)
	{
        // �޽���ť�� �޽����� ������ �޽��� ó��
		if( PeekMessage( &Message, NULL, 0U, 0U, PM_REMOVE ) )
		{
			if(Message.message == WM_QUIT) break;
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		// ó���� �޽����� ������ Render()�Լ� ȣ��
		else g_MainGame.Management();
		
	}
	return int(Message.wParam);
}


//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6
//������������������������������������������������������������������������������
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostMessage( hWnd, WM_DESTROY, 0, 0L );
				break;
			}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 6 DX SetUp
//������������������������������������������������������������������������������
bool SetUp(char *szError)
{
	//�������̽� ����
	if(NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		strcpy(szError, "��ġ ���� ����");
		return false;
	}

	//����̽� ��������ؼ� ����ü �� �Է�
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
#ifdef FULLMODE
	d3dpp.BackBufferWidth = WINSIZEX;
	d3dpp.BackBufferHeight = WINSIZEY;
	d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = false;
	// ����ī���� ���ɴ�� �����ӽ��� �������Ѵ�.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	
	//z���۸� ���̷�Ʈ������ �����ϰڴ� 
	d3dpp.EnableAutoDepthStencil	= true;
	//EnableAutoDepthStencil Ʈ���϶��� �ǹ� ���ִ°�
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
#else
	d3dpp.BackBufferWidth = WINSIZEX;
	d3dpp.BackBufferHeight = WINSIZEY;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = true;
	// ����ī���� ���ɴ�� �����ӽ��� �������Ѵ�.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	
	//z���۸� ���̷�Ʈ������ �����ϰڴ� 
	d3dpp.EnableAutoDepthStencil	= true;
	//EnableAutoDepthStencil Ʈ���϶��� �ǹ� ���ִ°�
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
#endif

	//����̽��� �����
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		strcpy(szError, "����̽� ���� ����");
		return false;     
	}

	//�ø����
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	//���� ����
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//z���� ����
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	g_MainGame.SetUp(g_pd3dDevice);
	return true;
}

