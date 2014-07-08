#include ".\texture.h"

CTexture::CTexture(void)
{
	m_pTexture = NULL;
}

CTexture::~CTexture(void)
{
	SAFE_RELEASE(m_pTexture);
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 11 �ؽ��ĸ� ����
//������������������������������������������������������������������������������
BOOL CTexture::SetUp(LPDIRECT3DDEVICE9 pd3dDevice, char *szFileName)
{
	if(FAILED(D3DXCreateTextureFromFile(pd3dDevice, szFileName, &m_pTexture)))
	{
		MessageBox(g_hWnd, "�ؽ��� �¾� ����", "�ؽ��� Ŭ���� ����", MB_OK);
		return FALSE;
	}
	return TRUE;
}
//������������������������������������������������������������������������������
//  Narusia.07 - 10 - 11 �ؽ��� ���¸� �����Ѵ� 
//������������������������������������������������������������������������������
void CTexture::SetState(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetTexture( 0, m_pTexture );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0�� �ؽ�ó ���������� Ȯ�� ����
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}