#pragma once

#include "../../Includes/Includes.hpp"

class Really : public ::LG::QueryWindow
{
public:
	Really()
		: QueryWindow("�����ҷ����? �̷��� ��մ� ���α׷��� ���ΰ�?", WHITE, LIGHTBLUE)
	{
		panel.text = "�׸��ҷ���";
		panel.width = 15;
		no.text = "��ư �Ǽ�~";
		no.width = 13;
	}
	virtual void onButtonPressed(bool witch_button);
	NEObject& clone() const { return *(new Really(*this)); }
};