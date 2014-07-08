#include "CRgb.hpp"
//	�⺻������
CRgb::CRgb() : _red(255), _green(255), _blue(255), _setted(false)
{

}
//	������:
CRgb::CRgb(char red, char green, char blue)
: _red(red), _green(green), _blue(blue), _setted(true)
{
}
//	���������
CRgb::CRgb(CRgb& rgb)
: _red(rgb._red), _green(rgb._green), _blue(rgb._blue), _setted(rgb._setted) 
{

}
//	�Ҹ���
CRgb::~CRgb() 
{

}
//	RGB���� D3DCOLOR ��(DWORD)�� �����Ѵ�
DWORD	CRgb::toD3DCOLOR()
{
	return D3DCOLOR_XRGB(_red, _green, _blue);
}
//	RGB���� ������ �Ǿ������� ��ȯ�Ѵ�.
//	�ʱ�ȭ ��, �ѹ��̶� RGB���� �����Ѱ��, �� ī�޶�� �������ϴ�
//	��� ��� ��ü�� RGB���� ���� �����Ѵ�.
bool	CRgb::isSetted()
{
	return _setted;
}
//	���� ������
CRgb&	CRgb::operator = (CRgb& rgb)
{
	_red = rgb._red;
	_green = rgb._green;
	_blue = rgb._blue;
	_setted = rgb._setted;

	return *this;
}
//	���� ������ �����Ѵ�
void	CRgb::setRed(int red)
{	
	if (red > 255)
		red = 255;
	if (red < 0)
		red = 0;

	_red = red;
	_setted = true;
}
//	���� ������ �����´�
int		CRgb::getRed()
{
	return _red;
}
//	�ʷ� ������ �����Ѵ�
void	CRgb::setGreen(int green)
{	
	if (green > 255)
		green = 255;
	if (green < 0)
		green = 0;

	_green = green;
	_setted = true;
}
//	�ʷ� ������ �����´�
int		CRgb::getGreen()
{
	return _green;
}
//	�Ķ� ������ �����Ѵ�
void	CRgb::setBlue(int blue)
{	
	if (blue > 255)
		blue = 255;
	if (blue < 0)
		blue = 0;

	_blue = blue;
	_setted = true;
}
//	�Ķ� ������ �����´�
int		CRgb::getBlue()
{
	return _blue;
}
//	RGB ������ ��� �����Ѵ�
void	CRgb::setRGB(int red, int green, int blue)
{	
	setRed(red);
	setGreen(green);
	setBlue(blue);
}
//	RGB ������ �ʱ�ȭ�Ѵ�. �ѹ��� ������ �ȵ� �ɷ� ��
void	CRgb::initialize()
{
	_red = 255;
	_blue = 255;
	_green = 255;
	_setted = false;
}
