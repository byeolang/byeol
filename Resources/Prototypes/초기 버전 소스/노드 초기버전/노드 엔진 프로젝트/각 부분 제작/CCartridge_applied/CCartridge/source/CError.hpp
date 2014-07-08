/*
	���ϸ�:	CError.hpp
	�̸�:	���� Ŭ����
	���:	��������(�����̸�, ��ġ����, ������ �Լ���, �ð�, ��¥)�� error.log
			�� �����ϰ�, �ɰ��� ���� �޼����ڽ��� ����� �����Ѵ�.
			(���, �޼����ڽ����� �����ϴ°�, main.cpp���� CHUB�� �ϰ�����)
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <fstream>
#include <time.h>
#include <atlstr.h>
#include <windows.h>
#include <tchar.h>
//		Ŀ���� ��Ŭ���:
#include "define.hpp"
//		���漱��:
class CWindow;
//		���ӽ����̽�:
using namespace std;
//	����:
//		Ŭ����:
class CError
{
//	��� �Լ�:
public:
	//	������:
	CError(CString error_name, CString error_explain, CString function_name, bool never_alert = false);
	//	�Ҹ���:
	~CError();
	//	���:	
	void	log(); 	
	void	showMessage();

//	���� �Լ�:
private:		
	//	�ð� �� ��¥:
	CString	getCurrentDate();
	CString	getCurrentTime();	


	
//	��� ����:
public:	
	//	��������:
	CString	_error_name,
			_error_explain,
			_function_name;	
	//	���º���:
	bool	_never_alert; // �α� ������� ����
};
