#pragma once

#include "../CommandSet/CommandSet.hpp"

class Commander : public CommandSet
{
public:		
	NEString command(const NEString& command_name) {
		for(int n=0; n < getLength() ; n++) {
			Command& itr = getElement(n);

			if(itr.name == command_name)
				return itr.execute();
		}

		return command_name + "�̶� ��ɾ�� ���ٱ���.\n��Ÿ? �ƴ� ����?";
	}
};