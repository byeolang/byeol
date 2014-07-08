/*
	���ϸ�:	CEngine.hpp
	�̸�:	���� Ŭ����
	���:	���̷�ƮX�� ���õ� ��� ����� �� �ٷ��
*/
#pragma once
//	����:
//		���̺귯��:
#pragma comment (lib, "d3dxof.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3d9.lib")
//			����� �� ������ ����:
#ifdef	_DEBUG
#pragma comment (lib, "d3dx9d.lib")
#else
#pragma comment (lib, "d3dx9.lib")
#endif
//		�⺻ ��Ŭ���:
#include <windows.h>
#include <d3dx9.h>
//		Ŀ���� ��Ŭ���:
#include "CINI.hpp"
#include "CWindow.hpp"
#include "CError.hpp"
#include "CArea.hpp"
#include "CRgb.hpp"
#include "CCamera.hpp"
#include "CTexture.hpp"
#include "define.hpp"
#include "CShader.hpp"
//	����:
//		Ŭ����:
class CEngine
{
//	����Լ�:
public:	
	//		������:
	CEngine(CINI* ini);
	//		�Ҹ���:
	~CEngine();
	//		����:
	void	restore();
	//		������:
	//			����:
	void	rendering(CCamera* camera, int index);
	void	render(CCamera* camera, int index);
	//			���� ���:
	HRESULT	present();
	//			ȭ�� ����:
	void	clearBuffer(D3DCOLOR color);

//	�����Լ�:
private:	
	//	�ʱ�ȭ:
	void	initializeDirectx();
	void	initializeDevice();
	void	initializeVertexBuffer();
	void	initializeRenderState();
	void	initializeMatrix();
	void	initializeDisplayMode();
	void	initializeCapabilities();	
	void	initializeTextureCapacity(int texture_capacity);
	void	initializeShader();
	void	initializeFilter();
	//	�Ҹ���:
	void	releaseResource();
	HRESULT	releaseDevice();
	void	releaseShader();
	void	releaseFilter();
	//	����:
	void	restoreDevice();
	void	restoreResource();	
	//	���÷��̸��:
	void	getCurrentDisplayMode(D3DDISPLAYMODE* display);
	void	getEnumerateDisplayMode(bool is32bitcolor);
	void	enumerateOneDisplayMode(D3DFORMAT format, int start_index, int count);
	int		getEnumerateDisplayModeCount(bool is32bitcolor);
	void	selectModeInEnumeratedMode();
	//	���͸�:
	void	filtering(CCamera* camera);	
	//	���ؽ�:
	void	setVertex(	CArea vertex, 
						DWORD color = D3DCOLOR_ARGB(255,255,255,255), 
						CArea texture = CArea(0.0f, 0.0f, 1.0f, 1.0f));
	void	drawVertex();
	//	ī�޶�:	
	void	getWorldMatrix(CCamera* camera);
	void	setRenderState(char mode);
	//	������Ÿ��:
	void	swapRenderTargetToTexture();
	void	swapRenderTargetToFrameBuffer();
	//	�ؽ���:	
	void	loadTexture(CString filename); 


	
//	�������:
public:
	CShader	_shader;

//	���κ���:
private:
	//	���̷�Ʈ:
	LPDIRECT3D9			_direct;
	LPDIRECT3DDEVICE9	_device;		
	D3DCAPS9			_caps;
	//	���÷��� ���:
	D3DDISPLAYMODE*	_playable_display_modes;
	int				_playable_display_mode_number;
	D3DDISPLAYMODE	_selected_display_mode;
	int				_isHardwareVertexProcessingEnable;	
	//	������:
	D3DPRESENT_PARAMETERS	_present_parameter; 
	LPDIRECT3DVERTEXBUFFER9	_vertex_buffer; 
	//	���ҽ�:	
	CTexture*	_texture; // �߿�! �ؽ��� ��ü�� CEngine�� �ְ�, CNode�� �ؽ��� ����, CScripter->_engine�� ���ؼ� �ؽ��Ŀ� �����Ѵ�
	int			_texture_capacity; // ������ ����Ǵ� �� [0 ~ capacity-1] ������
	int			_texture_size;
	//	������������:
	CINI*	_ini;
};