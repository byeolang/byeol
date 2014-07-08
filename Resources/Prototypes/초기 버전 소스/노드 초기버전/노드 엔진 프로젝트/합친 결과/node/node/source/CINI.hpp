/*
	���ϸ�:	CINI.hpp
	�̸�:	INI �δ�
	���:	�ý��� ����(�׷���, ����, ����) && ���ӿɼ� �� ����
*/

#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <Windows.h>
#include <tchar.h>
#include <atlstr.h> // CString�� API���� ����ϱ�����
//		Ŀ���� ��Ŭ���:
#include "CError.hpp"
#include "define.hpp"
//		�������� ����:
extern	CINI	g_ini;
//	����:
//		Ŭ����:
class CINI
{
//	��� �Լ�:
public:
	//	������:
	CINI();
	//	�Ҹ���:
	~CINI();
	//	������:	
	//		����ǥ:
	int		getScreenWidth();
	int		getScreenHeight();
	//		���̴�:
	float	getShaderWidth();
	float	getShaderHeight();
	//	����Ʈ:
	//		��ü:
	void	setDefaultVeryLow(); // ���� �ο� �� (�������)		
	//		���̴�:
	void	setDefaultVeryLowShader();
	


//	�������:
public:	
	//	�׷���:
	//		����� ����: (config)
	bool	_graphic_window_mode; // directx�� ������ ����?	
	int		_graphic_width; // �ػ�
	int		_graphic_height; // �ػ�
	bool	_graphic_vertical_synchronize; // ��������ȭ	
	bool	_graphic_32colorbit; // 32��Ʈ�� �����ϴ°�
	bool	_grpahic_32bit_texture;
	bool	_graphic_isShaderOn; // �ϵ���������� �Ұ����ϰų� || ���̴��� �����
	//		�ϵ���� ����: (info)
	bool	_info_hardware_shader_support; // �ϵ��� ���̴��� �����ϴ��� ������ ������

//	���κ���:
private:
	//	�׷���:
	//		ȭ�� ����:
	WINDOW_RATIO	_graphic_ratio;
	//		���̴�:
	SHADER_QUALITY	_graphic_shader_quality;
};