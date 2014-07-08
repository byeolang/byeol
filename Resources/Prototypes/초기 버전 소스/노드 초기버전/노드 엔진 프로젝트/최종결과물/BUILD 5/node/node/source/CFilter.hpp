/*
	���ϸ�:	CFilter.hpp
	�̸�:	���� ���̽� Ŭ����
	���:	��� ���͵��� ���̽� Ŭ����
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//		Ŀ���� ��Ŭ���:
#include "CError.hpp"
#include "CCamera.hpp"
#include "CArea.hpp"
#include "CArgb.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CFilter
{
//	����Լ�:
public:
	//	������:
	CFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height);		
	//	�Ҹ���:
	virtual	~CFilter();
	virtual void	release();
	//	������:
	void	drawVertex(LPDIRECT3DDEVICE9 device);
	virtual	void excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera) = 0;
	//	���̴� ���� �ε�:
	void	loadShaderFile(LPCTSTR filename, LPDIRECT3DDEVICE9 device);	
	
//	�����Լ�:
protected:
	//	���ؽ�:
	void	setVertex(LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CArea vertex_area , DWORD color = D3DCOLOR_ARGB(255,255,255,255), CArea texture_area = CArea(0.0f, 0.0f, 1.0f, 1.0f));	
	//	����������Ʈ:
	void	setRenderState(LPDIRECT3DDEVICE9 device, char mode);



//	�������:
public:
	//	���º���:
	bool			_enable;

//	���Ⱥ���:
protected:
	//	���ε�:
	LPD3DXEFFECT	_effect;
	D3DXHANDLE		_technique;
	//	���κ���:
	LPD3DXBUFFER	_err;
	//	���º���:
	UINT			_render_target_width;
	UINT			_render_target_height;
	float			_ratio;
};