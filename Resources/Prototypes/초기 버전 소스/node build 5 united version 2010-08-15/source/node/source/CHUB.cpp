#include "CHUB.hpp"

//	������
CHUB::CHUB(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow)
:_window(hInstance, nCmdShow, &_scripter), _engine(), _scripter(&_engine)
//	�����ڱ����� ������ �߿��ϴ�. ini(�����а�)-> window(ini�� ������ ũ������
//	�� ����)-> engine(���������� �������ڵ�� �ʱ�ȭ)
{
	timeBeginPeriod(DEFAULT_CHUB_TIMER_PERIOD);
	srand( static_cast<unsigned int>(time(NULL)) );
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