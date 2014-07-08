/*
	���ϸ�:	CShader.hpp
	�̸�:	���̴� Ŭ����
	���:	���̴��� ���õ� ��� �κ��� ����ϰ� ����. �ַ�, ���� ������.
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//		Ŀ���� ��Ŭ���:
#include "CFilter.hpp"
#include "CINI.hpp"
#include "CBlurFilter.hpp" // #����#�߰�#
#include "CNoiseFilter.hpp"
#include "CBloomFilter.hpp"
//	����:
//		Ŭ����:
class CShader
{
//	����Լ�:
public:
	//	������:
	CShader();
	//	�Ҹ���:
	~CShader();
	//	�ʱ�ȭ:
	void	initialize(LPDIRECT3DDEVICE9 device, CINI* ini);
	void	initializeLastPreserveCamera();
	//	����:
	void	release();
	//	������:
	//		ī�޶�:
	void	setLastPreserveCamera(CCamera* camera);
	//		������Ÿ��:
	void	swapOldToRecentIndex();
	int		getRecentIndex();
	int		getOldIndex();
	//	������ Ÿ��:
	void	swapToMainFrame();
	void	swapToRecentTarget();
	void	swapToOldTarget();
	//	���͸�:
	void	preFiltering(CCamera* camera);
	//	������ ��, ���͸��� �ϱ� ���� �������۾�
	void	postFiltering(CCamera* camera, LPDIRECT3DVERTEXBUFFER9 vertex_buffer);
	void	clearBuffer(D3DCOLOR color);
	void	filtering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer);

//	�����Լ�:
private:
	//	�ʱ�ȭ:
	void	initializeFilter(); 
	void	initializeRenderTargetTexture();
	void	initializeValue(CINI* ini, LPDIRECT3DDEVICE9 device);
	//	����:
	void	releaseFilter();
	void	releaseRenderTargetTexture();
	//	���͸�:
	//		�̱����͸�: ���Ͱ� �ϳ��� ���ִ� ���.
	void	singleFiltering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer);
	//		��ø���͸�: �ϳ��� ī�޶��� �������� ���Ͱ� ���ÿ� �������
	void	nestedFiltering(CCamera* camera, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, char last_filter_index);
	


//	�������:
public:
	//	���º���:
	bool	_iniShaderOn;

//	���κ���:
private:
	//	����:
	CFilter*			_filter[DEFAULT_CSHADER_FILTER_NUMBER];	
	//	���ҽ�:
	LPDIRECT3DTEXTURE9	_temp_texture[2];
	LPDIRECT3DSURFACE9	_temp_surface[2];
	LPDIRECT3DSURFACE9	_mainframe_surface;
	LPDIRECT3DTEXTURE9	_nested_filter_texture[2];
	LPDIRECT3DSURFACE9	_nested_filter_surface[2];
	//	�Ϲ� ����:
	int					_recent_index;
	UINT				_rendertarget_width;
	UINT				_rendertarget_height;
	CCamera*			_last_preserve_index; // last_preserve�� �������, ���⿡ �ּҰ��� ����.
	//	������:
	LPDIRECT3DDEVICE9	_device;
};