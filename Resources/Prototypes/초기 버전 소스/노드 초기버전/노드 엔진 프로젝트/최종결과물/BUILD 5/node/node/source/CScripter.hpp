/*
	���ϸ�:	CScripter.hpp
	�̸�:	��ũ���� Ŭ����
	���:	��ũ��Ʈ ����(��)�κ��� ��ũ��Ʈ �����͸� �д´�. �׿� ����, ��ü���� �����Ѵ�
			��ũ��Ʈ Ŭ������ ����Ʒ�, ��� ��ü���� �����̰�, �������� �ȴ�

	�޸�:	�������� �׽�Ʈ������ ä������ �ڵ尡 ���� (2010-07-08)
*/
#pragma once
//	����:
//		Ŀ���� ��Ŭ���:
#include "CEngine.hpp"
#include "CCamera.hpp"
#include "CError.hpp"
#include "define.hpp"
//�ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ�
#define	TEMP_CAMERA_NUMBER	5 //  �ӽ��ӽ��ӽ��ӽ��ӽ�
//�ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ��ӽ�

//	����:
//		Ŭ����:
class CScripter
{
//	����Լ�:
public:	
	//	������:
	CScripter(CEngine* engine);
	//	�Ҹ���:
	~CScripter();
	//	�ͽ�ťƮ�Լ�:	�� ������ ��� �� ����ϱ�	
	void	excute(); //	�ͽ�ťƮ �Լ���? ... �� Ŭ������ �������� ����ϴ� ���� �ٽ����� ���
	
//	�����Լ�:
private:
	//	������:
	void	rendering();
	//	������ ���� �۾�:	
	void	preRender();
	void	searchLastPreserveCamera();
	//	������ ���� �۾�:	
	HRESULT	postRender();



//	�������:
private:
	//	ī�޶�: ( #�۾�# �ӽ�)
	CCamera		_camera[TEMP_CAMERA_NUMBER]; //	#�۾�#	�׽�Ʈ��. �����δ�, �������� ��ġ���ִ�. 	
	//	�����ں���:
	CEngine*	_engine;
};