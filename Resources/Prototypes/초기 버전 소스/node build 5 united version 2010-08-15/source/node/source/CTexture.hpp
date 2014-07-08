/*
	���ϸ�:	CTexutre.hpp
	�̸�:	�ؽ��� Ŭ����
	���:	�ؽ��ĸ� ��� / �Ҹ� / ũ���� �����ϴ� 
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <atlstr.h>
#include <Windows.h>
#include <d3dx9.h>
//		Ŀ���� ��Ŭ���:
#include "CError.hpp"
#include "define.hpp"
//	����:
//		Ŭ����:
class CTexture
{
//	����Լ�:
public:
	//	������:
	CTexture();
	//	�Ҹ���:
	~CTexture();
	//	�ؽ���:
	void	loadTexture(LPDIRECT3DDEVICE9 device, bool is32bit_texture);
	void	UnloadTexture();
	void	setTexture(LPDIRECT3DDEVICE9 device);
	//	���ϰ��:
	void	setFileName(CString filename);
	void	releaseFileName();	



//	�������:	
public:
	//	ũ��:
	float	_width,
			_height,
			_halfwidth,
			_halfheight;	
	//	�ؽ��� ������:
	LPDIRECT3DTEXTURE9	_texture;

//	���κ���:
private:	
	//	���ϸ�:
	CString				_filename;
};