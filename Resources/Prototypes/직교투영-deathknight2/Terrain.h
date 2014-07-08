//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 ���� Ŭ����
//������������������������������������������������������������������������������
#pragma once
#include "defines.h"
#include "DXImage.h"

#define MAPSIZEX 128
#define MAPSIZEZ 128

class CTerrain
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;				// ��ġ ����̹�
	int							m_nTriangle;			// �ﰢ���� ����
	D3DXVECTOR3*		m_vHeightMap;		//����Ʈ���� ������ �����Ұ�
	CDXImage				m_Terrain;				// �����̹���
public:
	BOOL SetUp(LPDIRECT3DDEVICE9 pDevice); // �¾�
	CTerrain(void);
	virtual ~CTerrain(void);
};
