#include "CHUB.hpp"

//	������
CHUB::CHUB(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow)
:_window(hInstance, nCmdShow, &_input), _input()
//	�����ڱ����� ������ �߿��ϴ�. ini(�����а�)-> window(ini�� ������ ũ������
//	�� ����)-> engine(���������� �������ڵ�� �ʱ�ȭ)
{
	timeBeginPeriod(DEFAULT_CHUB_TIMER_PERIOD);
}
//	�Ҹ���
CHUB::~CHUB()
{
	timeEndPeriod(DEFAULT_CHUB_TIMER_PERIOD);
}
//	�޼����� �ڵ��ϴ� �Լ��� �̵��Ѵ�.
void	CHUB::handleMessage() 
{ 
	_window.HandleMessage(); 
}