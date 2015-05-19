#include "XAudio2Mixer.hpp"
#include "XAudio2Manager.hpp"

namespace XA2
{
	void XAudio2Mixer::_initializeRealMixer()
	{
		if(_real_channel)
			_release();
		IXAudio2& xa2 = XAudio2Manager::getInstance().getXAudio2();
		if( ! &xa2) return;

		if(FAILED(xa2.CreateSubmixVoice((IXAudio2SubmixVoice**)&_real_channel, 2, 44100)))
			ALERT_ERROR(" : Mixer ���� ����")
	}

	const NECodeSet& XAudio2Mixer::getModuleScriptCodes()
	{
		static NECodeSet instance;
		if (instance.getLength() <= 0)
		{
			instance.create(1);
			instance.push(ThisClass().getHeader());
		}

		return instance;
	}

}