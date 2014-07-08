/*
	���ϸ�:	CBloomFilter.hpp
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
#include "CBloomFilterData.hpp"
#include "CFilterData.hpp"
#include "CTexture.hpp"
//	����:
//		Ŭ����:
class CBloomFilter : public CFilter
{
//	����Լ�:
public:	
	//	������:
	CBloomFilter(LPDIRECT3DDEVICE9 device, UINT render_target_width, UINT render_target_height);
	//	�Ҹ���:	
	virtual ~CBloomFilter();
	//	�ʱ�ȭ:
	virtual void	initializeShader(LPDIRECT3DDEVICE9 device);
	virtual void	initializeHLSLValue(LPDIRECT3DDEVICE9 device);
	void	initializeResource(LPDIRECT3DDEVICE9 device);
	//	����:
	void	releaseResource();
	//	����: ���� �Լ�
	virtual void	excute(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);
	//	ĳ���ú�ȯ:
	CBloomFilterData*	casting(CFilterData* filter_data);
	//	���� ������Ʈ:
	void	update(CBloomFilterData* bloom_filter_data);
	void	updateBloomLevel(CBloomFilterData* bloom_filter_data);
	//	���͸�:	
	void	filtering(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DVERTEXBUFFER9 vertex_buffer, CCamera* camera);



//	���κ���:
private:
	//	�ڵ�:	
	D3DXHANDLE	_bloom_level_handle;
	D3DXHANDLE	_dif_texture_handle;
	D3DXHANDLE	_dib_texture_handle;
	//	�ؽ���:
	LPDIRECT3DTEXTURE9	_temp_texture[2];
	LPDIRECT3DSURFACE9	_temp_surface[2];
	
};