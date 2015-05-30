#pragma once

#include "Channels.hpp"
#include "XAudio2Mixer.hpp"
#include "XAudio2Decoder.hpp"

namespace XA2
{
	using namespace NE;

	class NE_DLL XAudio2Player : public XAudio2Mixer
	{
	public:
		typedef XAudio2Player ThisClass;
		typedef XAudio2Mixer SuperClass;
		friend class XAudio2Manager;

	public:
		NETArgument<NEModuleSelector>	arg_decoder;
		NETArgument<NEModuleSelector>	arg_mixer;
		NETArgument<NEIntKey>			arg_play_command;	//	0: Pause, -1: Stop, �Ӽ�: ���Ƚ��

	public:
		XAudio2Decoder& getDecoder() 
		{
			return _cast<XAudio2Decoder>(arg_decoder);
		}
		XAudio2Mixer& getMixer()
		{
			return _cast<XAudio2Mixer>(arg_mixer);
		}
		type_result play(type_count loop_count);
		type_result stop();
		type_result pause();
		bool isStopped() const;
		virtual type_result setVolume(type_float new_volume)
		{
			if (!_real_channel && _real_channel->voice)
				return RESULT_TYPE_WARNING;

			_real_channel->voice->SetVolume(new_volume);

			return arg_volume.setValue(new_volume);
		}
		virtual type_float getVolume() const
		{
			if( ! _real_channel && _real_channel->voice)
				return RESULT_TYPE_WARNING;

			type_float to_return = 0.0f;
			_real_channel->voice->GetVolume(&to_return);

			return to_return;
		}
		Channel& getRealChannel()
		{
			return *_real_channel;
		}
		const Channel& getRealChannel() const
		{
			return *_real_channel;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(NEResult::hasError(_header.isValid()))
			{
				_header.getName() = "XAudio2Player";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2015-05-06";
				_header.getComment() =
					"���ڵ��� PCM �����͸� ����ϴ� �÷��̾��Դϴ�.\n"
					"���, �Ͻ�����, ������ �� �� ������, �̸� ����ϱ� ���ؼ��� ���ڴ��� �ʿ��մϴ�.\n"
					"Command�� �ѹ� Execute �Ǹ�, 0(�ƹ��͵� ����)���� Set �˴ϴ�.\n"
					"�̹� Playing �� Player���� �ٽ� Play�� ����� ������, ������ ������� ������ ������� �Ұ�, ������ ���ο� ä���� ��� ����մϴ�."
					"��������� ���� ���� 2���� ���ÿ� �鸮�� �Ǹ� ������ ����� �Ҹ��� ������ �� �����ϴ�.\n";
				"�ͼ��� �������� ���� ���� �ٷ� �����͹ͼ��� ����� �˴ϴ�.\n";

				NETStringSet& argcomments = _header.getArgumentsComments();
				argcomments.create(4);				
				argcomments.push("Decoder\n�Ҹ� ������ ��� �ִ� ���ڴ� 1���� ���⿡ �����մϴ�.");
				argcomments.push("Mixer\n������ �ͼ��� ������Ϸ��� ���⿡ �ͼ��� 1�� �����մϴ�.");
				argcomments.push("PlayCommand\n-3:����\t-2: �Ͻ�����\t-1: ���� ���\t0: �ƹ��͵� ����\n���: ��� Ƚ��. ���� 0���� Set �˴ϴ�.");
				argcomments.push(SuperClass::getHeader().getArgumentsComments()[0]);				
			}

			return _header;
		}

	protected:
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			_real_channel = 0;
			_is_paused = false;
			arg_mixer.setEnable(false);
			arg_decoder.setPurposeLimitation(NEArgumentBase::READ_BY);			
			arg_play_command.setValue(1);
			arg_play_command.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_decoder);
			tray.push(arg_mixer);
			tray.push(arg_play_command);

			SuperClass::_onFetchArguments(tray);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute();

	private:
		template <typename T>
		static T& _cast(NEModule& target)
		{
			const NECodeSet& cs = T::getModuleScriptCodes();
			T* nullpointer = 0x00;

			NECode sample(target.getScriptCode(), NECodeType(NECodeType::MODULE_SCRIPT));

			if (&target								&&
				cs.find(sample) != NE_INDEX_ERROR)
				return static_cast<T&>(target);

			return *nullpointer;
		}

		template <typename T>
		static T& _cast(NETArgument<NEModuleSelector>& target)
		{
			T* nullpointer = 0x00;
			NEModule& module = target.getValue().getModule();
			if (!&module) return *nullpointer;

			target.getValue().initializeReferingPoint();

			return _cast<T>(module);
		}

	private:
		Channel* _real_channel;
		bool _is_paused;
	};
}