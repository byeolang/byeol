/*
	���ϸ�:	CCamera.hpp
	�̸�:	ī�޶� Ŭ����
	���:	��ü���� ������ ������, ���/Ȯ���Ͽ� ��������			
*/
#pragma once
//	����:
//		Ŀ���� ��Ŭ���:
#include "define.hpp"
#include "CArgb.hpp"
#include "CFilterData.hpp"
#include "CBlurFilterData.hpp" // #����#�߰�#
#include "CNoiseFilterData.hpp"
#include "CBloomFilterData.hpp"
//	����:
//		Ŭ����:
class CCamera
{
//	����Լ�:
public:	
	//	������:
	CCamera();
	CCamera(float x, float y, float z, float angle, float scale_x, float scale_y, CArgb argb, bool preserve, char render_state);
	//	�Ҹ���:
	~CCamera();
	//	���Ͱ˻�:
	bool	isThereEnabledFilter();
	void	setRenderState(char render_state);
	char	getRenderState();
	void	initializeRenderState();
	bool	isPreserved();



//	�����Լ�:
private:
	//	�ʱ�ȭ:
	void	initializeFilterData();



//	�������:
public:	
	//	ī�޶� �Ӽ�:		
	float	_angle,
			_x, _y, _z,
			_scale_x, _scale_y;	
	bool	_render_state_setted;
	CArgb	_argb;
	bool	_preserve;
	bool	_enable;
	//	����:
	CFilterData*	_filter_data[DEFAULT_CSHADER_FILTER_NUMBER];	

private:
	char	_render_state;	
};