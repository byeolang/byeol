#pragma once

#include "../Model/Model.hpp"

namespace DX9Graphics
{
	class TabledTexture;

	class NE_DLL AnimatedModel : public Model
	{
	public:
		typedef AnimatedModel ThisClass;
		typedef Model SuperClass;

	public:
		NETArgument<NEIntKey>	arg_animation_index;
		NETArgument<NEIntKey>	arg_key_frame;
		NETArgument<NEIntKey>	arg_delay_per_frame;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass::_onFetchArguments(tray);

			tray.push(arg_animation_index);
			tray.push(arg_key_frame);
			tray.push(arg_delay_per_frame);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_delay_per_frame.setValue(-2);	//	-1 is Automatic, -2 : use the defaults defined on TabledTexture.
			arg_delay_per_frame.setPurposeLimitation(NEArgumentBase::READ_BY);			
			arg_animation_index.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute();

	public:
		AnimatedModel()
			: SuperClass()
		{
			_release();
		}
		AnimatedModel(const ThisClass& source)
			: SuperClass(source)
		{
			_assign(source);
		}

	public:
		const ThisClass& operator=(const ThisClass& source)
		{
			SuperClass::operator=(source);

			return _assign(source);
		}
		bool operator==(const ThisClass& source) const
		{
			if(SuperClass::operator!=(source)) return false;
			return _delay == source._delay;
		}
		bool operator!=(const ThisClass& source) const
		{
			return ! operator==(source);
		}

	public:	
		type_int getDelay() const { return _delay; }
		type_result setDelay(type_int new_delay) { _delay = new_delay; return RESULT_SUCCESS; 	}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = "AnimatedModel";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				const NEExportable::ModuleHeader& super = SuperClass::getHeader();
				_header.getComment() = super.getComment() + 
					"\nAnimation ȿ���� �� �� �ֽ��ϴ�.";
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2013-08-14";
				NETStringSet& args = _header.getArgumentsComments();
				args = super.getArgumentsComments();
				args.resize(args.getLength() + 3);
				args.push("AnimationIndex\n���° �ִϸ��̼��� ����� ������ �ε����� �����ݴϴ�.");
				args.push("KeyFrame\n�ش� �ִϸ��̼��� Key Frame �Դϴ�.");
				args.push("DelayPerFrame\n�� �����Ӵ� ������ �Դϴ�.\n�ð��� �ƴ϶�, ������ ������ �����մϴ�.");
			}

			return _header;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual void release()
		{
			SuperClass::release();

			_release();
		}

	private:
		type_result _adjustValuesBy(const TabledTexture& tabled);
		type_result _updateDelayBy(const TabledTexture& tabled_has_max_delay);
		void _release()
		{
			_delay = 0;
		}
		ThisClass& _assign(const ThisClass& source)
		{
			if(this == &source) return *this;

			_delay = source._delay;
			arg_animation_index = source.arg_animation_index;
			arg_key_frame = source.arg_key_frame;
			arg_delay_per_frame = source.arg_delay_per_frame;

			return *this;
		}

	private:
		type_int _delay;
	};
}