#include "CCartridgeHeader.hpp"
#include "CCartridgeUnit.hpp"
#include "CError.hpp"

class CCartridge : public CCartridgeHeader
{
public:
	CCartridge() : _unit(NULL), _loaded(false), _cartridge_path(_T(""))
	{}
	~CCartridge()
	{}
	//	������:
	bool	isLoaded()
	{
		return _loaded;
	}
	
	bool	open(CString cartridge_path)
	{

	}	
	bool	close()
	{

	}	
	void	extractAll(CString extract_path = _T(""))
	{

	}	
	void	extractOneUnit(int unit_index, CString extract_path = _T(""))
	{

	}	
	bool	create(CString load_path[], int count, CString save_path)
	{
		//	���ܻ�Ȳó��:
		if (count <= 0)
			return false;

		//	�����ڵ�:
		setUnitQuantity(count);

		if (!loadUnitHeader(load_path, count))
			return false;

		update(count);

		saveCartridge(save_path);
	}	
	
private:
	void	update(int count)
	{
		updateHeader(count);
		updateStartPoint();
	}
	void	loadUnitHeader(CString load_path[], int count)
	{
		bool error = false;
		for (int n=0; n < count ;n++)
		{
			error = _unit[n].loadHeader(load_path[n]);

			if (!error)
			{
				// �޼��� ���
				clear();
				return false; // �۵� ����
			}
		}
	}
	void	updateHeader(int count)
	{
		_version = DEFAULT_CCARTRIDGEHEADER_VERSION;
		_valid_code = DEFAULT_CCARTRIDGEHEADER_VALID_CODE;
		_algorithm_number = DEFAULT_CCARTRIDGEHEADER_ALGORITHM_NUMBER;
		_compressed = DEFAULT_CCARTRIDGEHEADER_COMPRESSED;
		_unit_count = count;
	}
	bool	saveCartridge(CString save_path)
	{
		saveHeader();
		saveUnitHeader();
	}
	void	saveUnitHeader()
	{
		for (int n=0; n < _unit_count ;n++)
			_unit[n].saveHeader();
	}
	void	updateStartPoint()
	{

	}
	//	īƮ���� ������ ������ ����
	void	setUnitQuantity(int quantity)
	{

	}
	//	��� �����͸� �ʱ�ȭ
	void	clear()
	{
		setUnitQuantity(0);
		_loaded = false;
		_cartridge_path = _T("");		
		clearHeader();
	}	

private:

	CCartridgeUnit*	_unit; // ��������
	CString			_cartridge_path;	
	bool			_loaded;
};