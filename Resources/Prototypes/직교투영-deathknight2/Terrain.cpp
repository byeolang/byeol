#include ".\terrain.h"

CTerrain::CTerrain(void)
{
}

CTerrain::~CTerrain(void)
{
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 ���� ��
//������������������������������������������������������������������������������
BOOL CTerrain::SetUp(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	//�ﰢ���� ����
	m_nTriangle = (MAPSIZEX - 1) * (MAPSIZEZ - 1) * 2;

	///����Ʈ���� ������ ������ �迭�� �����Ҵ��Ѵ�
	m_vHeightMap = new D3DXVECTOR3[MAPSIZEX * MAPSIZEZ];
	// ���� ����
	m_Terrain.Setting(TRUE, TRUE);
	return TRUE;
}
