#pragma once

#include "../../Includes/Includes.hpp"

class NameInputWindow : public ::LG::InputWindow
{
public:
	NameInputWindow(NETString& target) : InputWindow("���ο� ���ڿ��� �Է����ֽñ� �ٶ��ϴ�.", BLACK, LIGHTCYAN, target), _target(target) {}

	FUNC_CLONE(NameInputWindow)
	virtual void onInputed()
	{
		_target = input.text;
		delete_me = true;
	}

	NETString& _target;
};