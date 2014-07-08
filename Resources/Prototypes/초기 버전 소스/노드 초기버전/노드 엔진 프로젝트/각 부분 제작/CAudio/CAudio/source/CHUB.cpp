#include "CHUB.hpp"

//	������
CHUB::CHUB(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow)
:_window(hInstance, nCmdShow, &_ini, &_audio), _audio(64)
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
	// ���ҽ�
	
	_audio.initializeMusicCapacity(2); // 0
	_audio.loadMusic(_T("..\\resource\\moteke.ogg"));
	_audio.loadMusic(_T("..\\resource\\moteke.ogg"));
	_audio.initializeSoundCapacity(1); // 0 ~ 1
	_audio.loadSound(_T("..\\resource\\heli.wav"));
	
	_window.HandleMessage(); 
}