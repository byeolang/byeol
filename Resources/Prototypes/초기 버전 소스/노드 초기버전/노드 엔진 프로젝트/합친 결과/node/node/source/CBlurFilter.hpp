/*
	���ϸ�:	CBlurFilter.hpp
	�̸�:	�� ����
	���:	ī�޶�Ŭ������ �����ؼ�, ī�޶� ���� ��󿡸� ���� ���δ�.
	�޸�:	���̴� ����Ƽ�� �ö󰡴� ���, �� �帴�ϰ� ���δ�. 
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3d9.h>
//		Ŀ���� ��Ŭ���:
#include "CFilter.hpp"
#include "define.hpp"
#include "CBlurFilterData.hpp"
#include "CFilterData.hpp"
//	����:
//		Ŭ����:
class CBlurFilter : public CFilter
{
//	����Լ�:
public:	
	//	������:
	CBlurFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height);	
	//	�Ҹ���:	
	virtual ~CBlurFilter();
	//	�ʱ�ȭ:
	virtual void	initializeShader(LPDIRECT3DDEVICE9 device);
	virtual void	initializeHLSLValue(LPDIRECT3DDEVICE9 device);
	void	initializeResource(LPDIRECT3DDEVICE9 device);
	//	����:
	void	releaseResource();
	//	����: ���� �Լ�
	virtual void	excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);
	//	ĳ���ú�ȯ:
	CBlurFilterData*	casting(CFilterData* filter_data);
	//	���� ������Ʈ:
	void	update(CBlurFilterData* blur_filter_data);
	void	updateBlurLevel(CBlurFilterData* blur_filter_data);
	//	���͸�:	
	void	filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);



//	���κ���:
private:
	//	�ڵ�:
	D3DXHANDLE	_blur_level_width_handle;
	D3DXHANDLE	_blur_level_height_handle;
	//	���ҽ�:
	LPDIRECT3DTEXTURE9	_xblured_texture;
	LPDIRECT3DSURFACE9	_xblured_surface;
};