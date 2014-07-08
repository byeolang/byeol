#include "DXImage.h"

CDXImage::CDXImage(void)
{
	m_dwTriangleNumber = 0;
	m_pTexture = NULL;
	m_nCount = 0;
}

CDXImage::~CDXImage(void)
{
	for(int i = 0; i < m_dwTextureNum; i++)
	{
		if(m_pTexture[i] != NULL)
			SAFE_RELEASE(m_pTexture[i]);
	}

	if(m_pIB != NULL)
		SAFE_RELEASE(m_pIB);
	if(m_pVB != NULL)
		SAFE_RELEASE(m_pVB);						// ���ؽ�����
}

//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 �⺻ ����
//������������������������������������������������������������������������������
BOOL	CDXImage::Setting(BOOL Index, BOOL Texture, DWORD dwTextureNum)
{
	m_bIndex = Index;
	m_bTexture = Texture;

	// �ؽ��İ� True �̳� False�� ���� ���������� ����
	if(Texture == TRUE)
	{
		m_FVF = (D3DFVF_CUSTOMAXIS | D3DFVF_COLOR | D3DFVF_TEX1);
	}
	else if(Texture == FALSE)
	{
		m_FVF = (D3DFVF_CUSTOMAXIS | D3DFVF_COLOR);
	}
	m_dwTextureNum = dwTextureNum;
	m_pTexture = new LPDIRECT3DTEXTURE9[dwTextureNum];
	// �ʱ�ȭ
	for(int i = 0; i < dwTextureNum; i++)
	{
		m_pTexture[i] = NULL;
	}
	return TRUE;
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // �ؽ��İ� �������� ���ؽ��¾�
//������������������������������������������������������������������������������
BOOL CDXImage::VertexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nVertices, CUSTOMVERTEX* pVertices)
{
	char szBuffer[128];

	if(m_bTexture == FALSE)
	{
		MessageBox(g_hWnd, szBuffer, "�¾��� �ؽ��� ���۸� FALSE�� �ϼ̽��ϴ�, ���� ���ؽ����� �¾��Դϴ�.", MB_OK);
		return FALSE;
	}

	int	nSize = nVertices * sizeof(CUSTOMVERTEX);

	m_dwVertexNum = nVertices;

	if(m_bIndex == FALSE)
	{
		m_dwTriangleNumber	 =	nVertices / 3;
	}

	if( FAILED( pDevice->CreateVertexBuffer( nSize,
		0, m_FVF, D3DPOOL_DEFAULT, &m_pVB, NULL )))
	{
		MessageBox(g_hWnd, szBuffer, "���ؽ� �¾� ����", MB_OK);
		return FALSE;
	}

	VOID* pVer;

	if( FAILED( m_pVB->Lock( 0, nSize, (void**)&pVer, 0 ))) return false;
	memcpy( pVer, pVertices, nSize);
	m_pVB->Unlock();

	return TRUE;	
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // // �ؽ��İ� �������� ���ؽ��¾�
//������������������������������������������������������������������������������
BOOL CDXImage::NoneTextureVertexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nVertices, NONETEXTUREVERTEX* pVertices)
{
	char szBuffer[128];

	if(m_bTexture == TRUE)
	{
		MessageBox(g_hWnd, szBuffer, "�ؽ��� ���۸� TRUE�� �ϼ̽��ϴ�, ���� NONE���ؽ����� �¾��Դϴ�.", MB_OK);
		return FALSE;
	}

	int	nSize = nVertices * sizeof(NONETEXTUREVERTEX);

	m_dwVertexNum = nVertices;

	if(m_bIndex == FALSE)
	{
		m_dwTriangleNumber	 =	nVertices / 3;
	}

	if( FAILED( pDevice->CreateVertexBuffer( nSize,
		0, m_FVF, D3DPOOL_DEFAULT, &m_pVB, NULL )))
	{
		MessageBox(g_hWnd, szBuffer, "NONE ���ؽ� �¾� ����", MB_OK);
		return FALSE;
	}

	VOID* pVer;

	if( FAILED( m_pVB->Lock( 0, nSize, (void**)&pVer, 0 ))) return false;
	memcpy( pVer, pVertices, nSize);
	m_pVB->Unlock();

	return TRUE;
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // // �ε��� ���ۼ¾�
//������������������������������������������������������������������������������
BOOL CDXImage::IndexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nIndices, tagIndex* pIndices)
{ 
	char szBuffer[128];
	if(m_bIndex == FALSE)
	{
		MessageBox(g_hWnd, szBuffer, "�¾��� �ε��� ���۸� FALSE�� �ϼ̽��ϴ�", MB_OK);
		return FALSE;
	}
	m_dwIndexNum = nIndices;
	int	nSize = nIndices * sizeof(tagIndex);

	if( FAILED( pDevice->CreateIndexBuffer( nSize,
		0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL )))
	{
		MessageBox(g_hWnd, szBuffer, "�ε��� �¾� ����", MB_OK);
		return FALSE;
	}

	VOID* pIndex;
	if( FAILED( m_pIB->Lock( 0, nSize, (void**)&pIndex, 0 ))) return false;
	memcpy( pIndex, pIndices, nSize);
	m_pIB->Unlock();

	return TRUE;
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // // �ؽ��Ĺ��ۼ¾�
//������������������������������������������������������������������������������
BOOL CDXImage::TextureSetUp(LPDIRECT3DDEVICE9 pDevice, char* szFileName, DWORD dwTextureNum)
{
	char szBuffer[128];

	if(m_bTexture == FALSE)
	{
		MessageBox(g_hWnd, szBuffer, "�¾��� �ؽ��� ���۸� FALSE�� �ϼ̽��ϴ�", MB_OK);
		return FALSE;
	}

	if(FAILED(D3DXCreateTextureFromFile(pDevice, szFileName, &m_pTexture[dwTextureNum])))
	{
		MessageBox(g_hWnd, szBuffer, "�ؽ��� �¾� ����", MB_OK);
		return FALSE;
	}
	return TRUE;
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // ������
//������������������������������������������������������������������������������
void	CDXImage::Render(LPDIRECT3DDEVICE9 pDevice, DWORD dwTextureNum)
{
	if(m_bTexture == TRUE)
	{
		pDevice->SetTexture(0, m_pTexture[dwTextureNum]);
		pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
	}
	else
	{
		pDevice->SetTexture(0, NULL);
		pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(NONETEXTUREVERTEX) );
	}

	pDevice->SetFVF( m_FVF );

	if(m_bIndex == TRUE)
	{	
		pDevice->SetIndices(m_pIB);
		pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, 0, m_dwIndexNum);
	}
	else
	{
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_dwTriangleNumber);
	}
}
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 // ��Ÿ��
//������������������������������������������������������������������������������
void	CDXImage::RenderCoolTime(LPDIRECT3DDEVICE9 pDevice)
{
	m_dwTime = GetTickCount();

	if(m_dwTime > m_dwNowTime)
	{
		m_dwNowTime = m_dwTime + 10;
		m_nCount++;

		if((m_dwIndexNum - m_nCount) <= 0)
		{
			m_nCount = 0;
		}
	}
	pDevice->SetTexture(0, NULL);
	pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(NONETEXTUREVERTEX) );

	pDevice->SetIndices(m_pIB);

	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwVertexNum, m_nCount * 3, m_dwIndexNum - m_nCount);
}