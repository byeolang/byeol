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
		no.text = "��ư�� �߸� ������";
		no.width = 20;
	}
	virtual void onButtonPressed(bool witch_button)
	{
		if(witch_button)
		{
			for(LG::WindowList::Iterator* itr = LG::Core::windows.getIterator(0); itr ;itr=itr->getNext())
				itr->getValue().delete_me = true;
		}

		delete_me = true;
	}
	NEObject& clone() const { return *(new Really(*this)); }
};