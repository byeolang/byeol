#pragma once
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

//������������������������������������������������������������������������������
// Narusia 07 - 10 - 6 Header
//������������������������������������������������������������������������������
#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <math.h>
#include <iostream>
//������������������������������������������������������������������������������
// Narusia 07 - 10 - 6 enum
//������������������������������������������������������������������������������
enum VERTEX {VER_CUBE, VER_LAND, VER_END};
enum TEXTURE {TEX_SOPI, TEX_END};
enum PLANET { PLANET_END};
//������������������������������������������������������������������������������
// Narusia 07 - 10 - 6 struct
//������������������������������������������������������������������������������
//struct tagPlanet
//{
//	D3DXMATRIXA16	matWorld;	//����Ϸ�� ���
//	D3DXVECTOR3		vPos;		//��ġ
//	D3DXVECTOR3		vRot;		//ȸ���� ����
//	float			fMag;		//��� Ȯ��
//	float			fRot;		//ȸ�� ����
//	float			fRotSpeed;	//ȸ�� ���ǵ�
//	TEXTURE	Texture;	//� �ؽ��ĸ� ������
//	bool			bGo;
//};
//������������������������������������������������������������������������������
// Narusia 07 - 10 - 9 struct
//������������������������������������������������������������������������������
// �ؽ��İ� �ʿ��Ҷ�
struct CUSTOMVERTEX
{
	FLOAT	x, y, z;	//����
	DWORD	color;		//����
	float	tu, tv;		//�ؽ��� ��ǥ 
};
// �ؽ��İ� �ʿ������
struct NONETEXTUREVERTEX
{
	FLOAT	x, y, z;	//����
	DWORD	color;		//����
};
// �ε���
struct tagIndex
{
	WORD _0;
	WORD _1;
	WORD _2;
};
//������������������������������������������������������������������������������
// Narusia 07 - 10 - 6 define
//������������������������������������������������������������������������������
//#define FULLMODE
#define WINSIZEX	800.0f
#define WINSIZEY	600.0f
#define D3DFVF_CUSTOMAXIS D3DFVF_XYZ 
#define D3DFVF_COLOR D3DFVF_DIFFUSE
#define MouseFLOAT 0.05f
#define MovingSpeed 0.06f
#define COOLTIME_MAX_DIVIDE 3000
#define SAFE_DELETE(p)   if(p) { delete (p); p = NULL; }
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}
//������������������������������������������������������������������������������
// Narusia 07 - 10 - 6 extern
//������������������������������������������������������������������������������
extern HWND						g_hWnd;		
extern HINSTANCE				g_hInst;	


