#include "CINI.hpp"
#include "CInputNodeSample.hpp"
CINI	g_ini;

	//	������:
CINI::CINI() : _window_activate(true)
{
	// #��ǲ������#�߰�#
	_sample	= new CInputNodeSample();
	_sample->intializeToXbox360Pad(); // #�۾�# �����, ���⼭ ���Ϸκ��� serialize��.

	setAnalogstickDeadzone(DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_DEADZONE);
	setAnalogstickMaxDeadzone(DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_MAX_DEADZONE);
	setAnalogstickToButtonDeadzone(DEFAULT_CINPUTCONSOLEXBOX360PAD_ANALOGSTICK_TO_BUTTON_DEADZONE);
	setTriggerDeadzone(DEFAULT_CINPUTCONSOLEXBOX360PAD_TRIGGER_DEADZONE);
	setTriggerToButtonDeadzone(DEFAULT_CINPUTCONSOLEXBOX360PAD_TRIGGER_TO_BUTTON_DEADZONE);
}
	//	�Ҹ���:
CINI::~CINI()
{
	delete _sample;
	_sample = NULL;
}
