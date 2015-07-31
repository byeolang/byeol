#include "XAudio2Decoder.hpp"
#include "XAudio2WAVEDecoder.hpp"
#include "XAudio2OGGDecoder.hpp"
#include "XAudio2Manager.hpp"
#include "XAudio2Player.hpp"

namespace XA2
{
	const NECodeSet& XAudio2Decoder::getModuleScriptCodes()
	{
		NECodeType type(NECodeType::MODULE_SCRIPT);
		static NECodeSet instance(type);

		if (instance.getSize() <= 0)
		{
			instance.create(2);
			instance.push(XAudio2WAVEDecoder().getHeader());
			instance.push(XAudio2OGGDecoder().getHeader());
		}

		return instance;		
	}
	void XAudio2Decoder::_stopAllRelativePlayers()
	{
		if( ! arg_load_success.getValue()) return;
		static NECodeSet cs(1, NECodeType::ALL);
		if (cs.getLength() <= 0)
			cs.push(NEExportable::Identifier("XAudio2Player.kniz"));

		NEModuleSelector ms;
		ms.setManager(NEType::NENODE_MANAGER);
		ms.setCodes(NECodeType(NECodeType::ALL));
		ms.setModuleCodes(cs);

		//	������ü �Ҹ� ��� ������:
		//		�ڽ� Decoder�� �����ϰ� �ִ� ��ü�� ���� �Ҹ���Ѿ� �Ѵ�.
		//		�밳 Player�� �� �����͸� ��� �ְ� �Ǵµ�, Player�� ������� ���
		//		���� XAudio2Manager�� ���� �����ؾ߸� �Ѵ�.
		//	Player Ž��:
		while(NEModule* e = &ms.getModule())
		{
			XAudio2Player& pl = static_cast<XAudio2Player&>(*e);
			if(	cs.find(pl.getScriptCode()) >= 0	&&
				&pl.getDecoder() == this			&&
				! pl.isStopped()					)
				pl.stop();
		}
		//	Manager Ž��:
		XAudio2Manager& man = XAudio2Manager::getInstance();
		Channels& chs = man.getChannels();
		for(int n=0; n < chs.getLength() ;n++)
		{
			Channel& ch = chs[n];
			if( (! ch.isStopped())	&& 
				ch.decoder == this	)
				ch.release();			

		}
	}
}