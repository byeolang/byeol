#pragma once

#include "../Resource/Resource.hpp"

namespace DX9Graphics
{
	class NE_DLL DXFont : public Resource
	{
	public:
		typedef DXFont ThisClass;
		typedef Resource SuperClass;

	public:
		NETArgument<NETStringKey>	arg_name;
		NETArgument<NEUByteKey>		arg_charset;
		NETArgument<NEUIntKey>		arg_width;
		NETArgument<NEUIntKey>		arg_height;
		NETArgument<NEUIntKey>		arg_weight;
		NETArgument<NEBooleanKey>	arg_italic;

	protected:
		virtual type_result _onFetchModule()
		{
			arg_name.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_charset.setValue(DEFAULT_CHARSET);
			arg_charset.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_width.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_height.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_weight.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_italic.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_name);
			tray.push(arg_charset);
			tray.push(arg_width);
			tray.push(arg_height);
			tray.push(arg_weight);
			tray.push(arg_italic);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			if (!_font)
				return initializeResource();

			return RESULT_SUCCESS;
		}

	public:
		DXFont() : Resource(), _font(0) {}
		DXFont(const ThisClass& src) : SuperClass(src), _font(0)
		{
			_assign(src);
		}

	public:
		DXFont& operator=(const ThisClass& src)
		{
			if(this == &src) return *this;

			SuperClass::operator=(src);			

			return _assign(src);	//	_font�� �Ҵ����� �ʴ´�.
		}

	public:		
		const LPD3DXFONT getFont() const { return _font; }
		LPD3DXFONT getFont() { return _font; }

	public:
		virtual type_result initializeResource();
		virtual type_result releaseResource()
		{
			SuperClass::releaseResource();

			if(_font)
				_font->Release();
			_font = 0;
			return RESULT_SUCCESS;
		}

	public:
		virtual NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				_header.getName() = _T("DXFont");
				_header.getDeveloper() = _T("kniz");
				_header.setRevision(1);
				_header.getComment() = "SpriteTexter Module���� ����� Font�� �����մϴ�.";

				_T("0��. DX9\n1��. ��Ʈ�� �̸�\n2��. ĳ���ͼ�(0:ANSI, 1:�⺻, 129:�ѱ�, 128:SHIFT�Ϻ�, 136:�߱�BIG5\n3��. ��\n4��. ����(���� ���� 2��)\n5��. �β�(100 ~ 400-����- ~ 900)\n6��. ���Ÿ�?");
				_header.getVersion() = _T("0.0.1a");
				_header.getReleaseDate() = _T("2013-10-05");
				NETStringSet& args = _header.getArgumentsComments();
				args.create(6);
				args.push("��Ʈ�� �̸�");
				args.push("���� ĳ���ͼ�\n0: ANSI\t1:�⺻\t129:�ѱ�\n128:SHIFT�Ϻ�\t136:�߱�BIG5");
				args.push("Width\n���� �� ���� �� ��");
				args.push("Height\n���� �ѱ��ڴ� ���̸� ���մϴ�. ���� ���� 2�� �Դϴ�.");
				args.push("Weight\n������ �β��Դϴ�. 400�� �������� 100�̸� ���� �۾���, 900�̸� �β��� �۾��� ���ɴϴ�.");
				args.push("Italic\n���Ÿ����� ����ϴ��� ���մϴ�.");
			}

			return _header;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		ThisClass& _assign(const ThisClass& source)
		{
			if (this == &source) return *this;

			arg_name = source.arg_name;
			arg_charset = source.arg_charset;
			arg_width = source.arg_width;
			arg_height = source.arg_height;
			arg_weight = source.arg_weight;
			arg_italic = source.arg_italic;

			return *this;
		}

	private:
		LPD3DXFONT _font;

	public:
		static const NECodeSet& getModuleScriptCodes()
		{
			NECodeType type(NECodeType::MODULE_SCRIPT);
			static NECodeSet _instance(type);

			if(_instance.getSize() <= 0)
			{
				_instance.create(1);
				_instance.push(NEExportable::Identifier("DXFont.kniz"));
			}

			return _instance;
		}
	};
}