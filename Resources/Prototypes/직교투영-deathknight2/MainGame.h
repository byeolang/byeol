//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 ���ӿ� �ʿ��� ��� �κ��� �̰��� ���δ�.
//������������������������������������������������������������������������������
#pragma once
#include "defines.h"
#include "DXImage.h"
#include "Camera.h"
#include "CInput.h"
#include "Terrain.h"
#include "Texture.h"
#include "Axis.h"

class CMainGame
{
private:
	CInputDevice				m_InputMouse;	// ���콺
	CInputDevice				m_InputKeyBoard;// Ű����
	CCamera						m_Camera;
	LPDIRECTINPUT8			m_pDI;				// ��ǲ��ġ
	CDXImage					m_Image[2];
	LPDIRECT3DDEVICE9	m_pDevice;				// ��ġ ����̹�
	CUSTOMVERTEX			m_Vertices[6];
	DWORD						m_dwTime;		
	DWORD						m_dwNowTime;
	CAxis							m_Axis;
	float							m_fAngle;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;						// ���ؽ�����
	LPDIRECT3DINDEXBUFFER9			m_pIB;						// �ε��� ����
	DWORD						m_dwColor;
private:
	void	CameraSetUp(void);						// ī�޶� �¾�
	BOOL InputSetUp(void);						// ��ǲ �¾�
	void	KeyBordMouseCameraUpdate(void);	// Ű���� ���콺 ī�޶� ������Ʈ
	void	Render(void);								// �׸� ����
public:
	BOOL SetUp(LPDIRECT3DDEVICE9 Device); // �¾� ����
	CMainGame(void);
	void Management(void); // PeekMessage �����ȿ�����
	virtual ~CMainGame(void);
};
