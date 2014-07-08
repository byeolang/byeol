/*
	���ϸ�:	CArea.hpp
	�̸�:	���� Ŭ����
	���:	CRect�� float ��.
			x1, x2, y1, y2�� float���·� �����Ѵ�
			CArea���� operator ���� ����
	�޸�:	�׽�ƮX
*/
#pragma once
//	����:
//		Ŭ����:
class CArea
{
//	��� �Լ�:
public:	
	//	������:
	//		�⺻������:
	CArea();
	//		float 4��:
	CArea(float x1, float y1, float x2, float y2);
	//		���������:
	CArea(CArea& area);
	//	�Ҹ���:
	~CArea();
	//	������ ��ø:
	//		��Ģ����:
	//			����:
	CArea	operator + (CArea& area);
	CArea&	operator += (CArea& area);
	//			����:
	CArea	operator - (CArea& area);
	CArea&	operator -= (CArea& area);
	//			����:
	CArea	operator * (CArea& area);
	CArea&	operator *= (CArea& area);
	//			������:
	CArea	operator / (CArea& area);
	CArea&	operator /= (CArea& area);
	//			��ȣ:
	CArea&	operator = (CArea& area);
	//		������:
	//			������:
	bool	operator == (CArea& area);



//	�������:
public:	
	//	����:
	float	_x1, _y1, _x2,	_y2;
};
