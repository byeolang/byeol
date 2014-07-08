#include "CArgb.hpp"
CArgb::CArgb() : CRgb(), _alpha(255)
{}

CArgb::CArgb(char alpha, char red, char green, char blue) 
: CRgb(red, green, blue), _alpha(alpha), _alpha_setted(false)
{}

CArgb::CArgb(CArgb& argb): CRgb(argb),
_alpha_setted(argb._alpha_setted), _alpha(argb._alpha)
{}

CArgb::~CArgb()
{}

CArgb&	CArgb::operator = (CArgb& argb)
{
	_alpha = argb._alpha;
	_alpha_setted = argb._alpha_setted;
	CRgb::operator = (argb);

	return *this;
}	
//	D3DCOLOR_ARGB ��(DWORD)�� ����ȯ�Ѵ�.
DWORD	CArgb::toD3DCOLOR()
{
	return D3DCOLOR_ARGB(_alpha, _red, _green, _blue);
}
//	�ѹ��̶� ������ ����Ǿ����� ��ȯ�Ѵ�.
//	
bool	CArgb::isSetted()
{
	if (	CRgb::isSetted() ||
			_alpha_setted		)
		return true;
	else
		return false;
}
//	���İ��� �����Ѵ�
void	CArgb::setAlpha(int alpha)
{
	if (alpha > 255)
		alpha = 255;
	if (alpha < 0)
		alpha = 0;

	_alpha = alpha;
	_alpha_setted = true;
}
//	���İ��� �����´�
int		CArgb::getAlpha()
{
	return _alpha;
}
//	ARGB ������ �ѹ��� �����Ѵ�
void	CArgb::setARGB(int alpha, int red, int green, int blue)
{
	setAlpha(alpha);
	setRGB(red, green, blue);
}
//	������ �ʱ�ȭ�Ѵ�
void	CArgb::initialize()
{
	_alpha = 255;
	_alpha_setted = false;
	CRgb::initialize();
}

