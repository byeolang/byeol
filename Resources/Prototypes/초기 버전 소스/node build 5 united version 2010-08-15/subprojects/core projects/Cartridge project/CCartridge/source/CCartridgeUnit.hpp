#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include "cartridge_define.hpp"
#include <atlstr.h> // CString�� API���� ����ϱ�����

class CCartridgeUnit
{
public:
	CCartridgeUnit() :_start_point(0), _filename(_T("")), _file_size(0), _path(_T(""))
	{

	}
	~CCartridgeUnit()
	{
		clear();
	}
	//	������:
	CCartridgeUnit&	operator = (CCartridgeUnit& rhs)
	{
		_start_point = rhs._start_point;
		_filename = rhs._filename;
		_file_size = rhs._file_size;
		_path = rhs._path;
	}
	//	������:
	void	setStartPoint(__int64 start_point)
	{
		_start_point = start_point;
	}
	//	�Ϲ��Լ�:
	void	clear()
	{
		_start_point = 0;
		_filename = _T("");
		_file_size = 0;
	}
	void	loadHeader(CString target)
	{

	}
	void	saveHeader()
	{

	}
	void	save()
	{

	}

public:	
	__int64	_start_point;
	CString	_filename; // �����̸���
	CString	_path;
	__int64	_file_size;
};