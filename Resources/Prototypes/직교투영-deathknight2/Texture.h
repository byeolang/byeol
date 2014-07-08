//������������������������������������������������������������������������������
// Narusia.07 - 10 - 11 �ؽ��ĸ� �����ϴ� Ŭ����
//������������������������������������������������������������������������������
#pragma once
#include "defines.h"

class CTexture
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DTEXTURE9	m_pTexture;
public:
	LPDIRECT3DTEXTURE9*	GetTexture(void) {return &m_pTexture;}
	void SetState(LPDIRECT3DDEVICE9 pd3dDevice);
	BOOL SetUp(LPDIRECT3DDEVICE9 pd3dDevice, char *szFileName);
	CTexture(void);
	virtual ~CTexture(void);
};
