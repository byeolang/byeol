#include "CArea.hpp"
//	�⺻ ���������
CArea::CArea() : _x1(0), _y1(0), _x2(0), _y2(0) 
{

}
CArea::CArea(float x1, float y1, float x2, float y2)
: _x1(x1), _y1(y1), _x2(x2), _y2(y2) 
{

}
//	���������
CArea::CArea(CArea& area)
: _x1(area._x1), _y1(area._y1), _x2(area._x2), _y2(area._y2) 
{

}
//	�Ҹ���:
CArea::~CArea() 
{

}
//	����
CArea	CArea::operator + (CArea& area) 
{ 
	return CArea(	_x1 + area._x1, _y1 + area._y1, _x2 + area._x2, _y2 + area._y2); 
}
CArea&	CArea::operator += (CArea& area) 
{ 
	_x1 += area._x1;
	_y1 += area._y1;
	_x2 += area._x2;
	_y2 += area._y2; 

	return *this;
}
//	����
CArea	CArea::operator - (CArea& area) 
{
	return CArea(	_x1 - area._x1, _y1 - area._y1, _x2 - area._x2, _y2 - area._y2); 
}
CArea&	CArea::operator -= (CArea& area) 
{	
	_x1 -= area._x1; 
	_y1 -= area._y1; 
	_x2 -= area._x2; 
	_y2 -= area._y2; 

	return *this;
}
//	����
CArea	CArea::operator * (CArea& area) 
{ 
	return CArea(	_x1 * area._x1, _y1 * area._y1, _x2 * area._x2, _y2 * area._y2); 
}
CArea&	CArea::operator *= (CArea& area) 
{	
	_x1 *= area._x1; 
	_y1 *= area._y1; 
	_x2 *= area._x2; 
	_y2 *= area._y2; 

	return *this;
}
//	������
CArea	CArea::operator / (CArea& area)
{ 
	return CArea(	_x1 / area._x1, _y1 / area._y1, _x2 / area._x2, _y2 / area._y2); 
}
CArea&	CArea::operator /= (CArea& area) 
{	
	_x1 /= area._x1;
	_y1 /= area._y1; 
	_x2 /= area._x2;
	_y2 /= area._y2; 

	return *this;
}	
//	���Կ���
CArea&	CArea::operator = (CArea& area) 
{ 
	_x1 = area._x1;
	_y1 = area._y1; 
	_x2 = area._x2; 
	_y2 = area._y2; 
	
	return *this; 
}
//	���� ������
bool	CArea::operator == (CArea& area) 
{ 
	if (	(_x1 == area._x1) &&
			(_y1 == area._y1) &&
			(_x2 == area._x2) &&
			(_y2 == area._y2))
		return true;
	else
		return false;
}