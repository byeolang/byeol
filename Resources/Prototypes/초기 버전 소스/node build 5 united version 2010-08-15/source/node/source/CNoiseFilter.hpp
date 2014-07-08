/*
	���ϸ�:	CNoiseFilter.hpp
	�̸�:	������ ����
	���:	ī�޶�Ŭ������ �����ؼ�, ī�޶� ���� ��󿡸� ����� ���δ�.
	�޸�:	���̴� ����Ƽ�� �ö󰡴� ���, �� �帴�ϰ� ���δ�. 
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3d9.h>
//		Ŀ���� ��Ŭ���:
#include "CFilter.hpp"
#include "define.hpp"
#include "CNoiseFilterData.hpp"
#include "CFilterData.hpp"
#include "CTexture.hpp"
//	����:
//		Ŭ����:
class CNoiseFilter : public CFilter
{
//	����Լ�:
public:	
	//	������:
	CNoiseFilter(LPDIRECT3DDEVICE9 device, int render_target_width, int render_target_height);
	//	�Ҹ���:	
	virtual ~CNoiseFilter();
	//	�ʱ�ȭ:
	virtual void	initializeShader(LPDIRECT3DDEVICE9 device);
	virtual void	initializeHLSLValue(LPDIRECT3DDEVICE9 device);
	void	initializeResource(LPDIRECT3DDEVICE9 device);
	//	����:
	void	releaseResource();
	//	����: ���� �Լ�
	virtual void	excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);
	//	ĳ���ú�ȯ:
	CNoiseFilterData*	casting(CFilterData* filter_data);
	//	���� ������Ʈ:
	void	update(CNoiseFilterData* noise_filter_data);
	void	updateNoiseLevel(CNoiseFilterData* noise_filter_data);
	//	���͸�:	
	void	filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);



//	���κ���:
private:
	//	�ڵ�:
	D3DXHANDLE	_xrepeat_handle;
	D3DXHANDLE	_yrepeat_handle;
	D3DXHANDLE	_source_texture_handle;
	D3DXHANDLE	_noise_texture_handle;
	//	���ҽ�:
	CTexture	_noisemap_texture;
};