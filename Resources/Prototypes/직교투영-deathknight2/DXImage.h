//������������������������������������������������������������������������������
// Narusia 07 - 10 - 10 ���� �̹��� �ε��� ������ ���� Ŭ����
//������������������������������������������������������������������������������
#pragma once
#include "defines.h"

class CDXImage
{
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;						// ���ؽ�����
	LPDIRECT3DTEXTURE9*				m_pTexture;				// �ؽ��� ����
	LPDIRECT3DINDEXBUFFER9			m_pIB;						// �ε��� ����
	DWORD									m_FVF;						// FVF
	DWORD									m_dwVertexNum;			// ���ؽ����� ����
	DWORD									m_dwIndexNum;			// �ε������� ����
	DWORD									m_dwTriangleNumber;	// �ε������۾������� �ﰢ�� ����
	BOOL										m_bIndex;					// �ε��� ���۸� �����Ұ��ΰ��� ����
	BOOL										m_bTexture;				// �ؽ��� ���۸� �����Ұ��ΰ��� ����
	DWORD									m_dwTextureNum;
	DWORD									m_dwTime;
	DWORD									m_dwNowTime;
	int											m_nCount;
public:
	BOOL	Setting(BOOL Index, BOOL Texture, DWORD dwTextureNum = 0); // �ʱ� ����
	BOOL VertexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nVertices, CUSTOMVERTEX* pVertices); // ���ؽ� ���� �¾�
	BOOL NoneTextureVertexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nVertices, NONETEXTUREVERTEX* pVertices); // NONE ���ؽ� ���� �¾�
	BOOL IndexBufferSetUp(LPDIRECT3DDEVICE9 pDevice, int nIndices, tagIndex* pIndices); // �ε��� ���� �¾�
	BOOL TextureSetUp(LPDIRECT3DDEVICE9 pDevice, char* szFileName, DWORD dwTextureNum); // �ؽ��� ���� �¾�
	void	Render(LPDIRECT3DDEVICE9 pDevice, DWORD dwTextureNum = 0); // ����
	void	RenderCoolTime(LPDIRECT3DDEVICE9 pDevice); // ��Ÿ��
	CDXImage(void);
public:
	virtual ~CDXImage(void);
};
