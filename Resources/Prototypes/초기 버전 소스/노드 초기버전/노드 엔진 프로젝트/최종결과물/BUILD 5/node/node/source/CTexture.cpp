#include "CTexture.hpp"

//	������
CTexture::CTexture() : _texture(NULL), _filename(_T("")),
_width(0), _height(0), _halfwidth(0), _halfheight(0)
{}	
//	�Ҹ���
CTexture::~CTexture()
{
	if( _texture )
		_texture->Release();	
}	
//	�̸� �Էµ� ��η�, �ؽ��ĸ� �о����
void	CTexture::loadTexture(LPDIRECT3DDEVICE9 device, bool is32bit_texture)
{
	//	���ܻ�Ȳ ó��:
	//		filename�� ���� ���:
	if (_filename == _T(""))
	{
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���06:\t�ؽ��ĸ� �о�� ���������� �����ϴ�."), 
			_T("�ذ���: ."),
			_T(__FUNCTION__)
		);
		e.log();
		return ;
		//	�α׸� ����ϰ� �ؽ��ĸ� ��� �ε��Ѵ�
	}
	//		�ؽ��İ� �̹� �ִ� ���:
	if ( _texture )
	{
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���01:\t1. �ؽ��İ� �ʱ�ȭ ���� �ʾҽ��ϴ�. �ؽ����� ������ �����ϴ� ����(_texture_size)�� �ܺο��� ������� �ʾҳ� �ǽ��մϴ�."), 
			_T("�ؽ��ĸ� ������ ������ �߽��ϴ�"),
			_T(__FUNCTION__)
		);
		e.log();
		UnloadTexture();			
	}


	//	�����ڵ�:
	//		�ؽ��� �ε�:
	D3DXIMAGE_INFO	info;
	D3DFORMAT		format;
	if (is32bit_texture)
		format = D3DFMT_A8B8G8R8; // ���÷����� 32��Ʈ��, �ؽ����� 32��Ʈ�� �ٸ����̴�. 16��Ʈ-32��Ʈ �� �ǰ�, 32��Ʈ-16��Ʈ�� �ȴ�.
	else
		format = D3DFMT_A1R5G5B5;

	D3DXCreateTextureFromFileEx(device, _filename,D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, DEFAULT_CTEXTURE_MIPMAPLEVEL, 0, 
		D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, DEFAULT_CENGINE_COLORKEY,
		&info, NULL, &_texture);
	//(info.Width);
	//	���ܻ�Ȳó��:
	//		�ؽ��� �ε� ����:
	if (!_texture)
	{
		//	���� ����. ���� ������ ����Ѵ�
		CString	message;
		message = _T("���03:\t�ؽ��ĸ� �дµ� �����߽��ϴ�. ������ �������� ���� �� �����ϴ�. ���ϸ�:" + _filename);
		CError e
		(	
			message, 
			_T("�ذ���"),
			_T(__FUNCTION__)
		);
		e.log();	
	}
	else
	{
		//	�ؽ����� ������ �����Ѵ�
		_width = static_cast<float> (info.Width);
		_halfwidth = _width / 2.0f;
		_height = static_cast<float> (info.Height);
		_halfheight = _height / 2.0f;
	}
}
//	�ؽ��ĸ� ������ �Ѵ�. ��δ� �����ֱ⶧����, ��θ� ��ǲ���� �ʰ� �ٷ� �ε尡 �����ϴ�.
void	CTexture::UnloadTexture() 
{
	if (_texture)
	{
		_texture->Release();
		_texture = NULL;
		_height = 0;
		_width = 0;
		_halfheight = 0;
		_halfwidth = 0;
	}
}
//	�ؽ��ĸ� ����̽��� �����Ѵ�.
void	CTexture::setTexture(LPDIRECT3DDEVICE9 device)
{
	if (_texture)
		device->SetTexture(0, _texture);
}
//	�ؽ��� ��θ� ��ǲ�Ѵ�.
void	CTexture::setFileName(CString filename)
{
	_filename = filename;
}
void	CTexture::releaseFileName()
{
	_filename = _T("");
}