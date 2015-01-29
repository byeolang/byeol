#include "BufferClearer.hpp"
#include "../DX9/DX9.hpp"

namespace DX9Graphics
{
	NEExportable::ModuleHeader& BufferClearer::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = "BufferClear";
			_header.getDeveloper() = "kniz";
			_header.setRevision(1);
			_header.getComment() = "NodeManager ������ �ִ� DX9�� ã�Ƽ�, ����۸� ������ ������ �ʱ�ȭ �մϴ�.\n�ݵ�� DX9 Module�� NodeManager ������ ���� ��, �����ؾ� �մϴ�.";
			_header.getVersion()  = "0.0.1a";
			_header.getReleaseDate() = "2013-01-29";
			NETStringSet& args = _header.getArgumentsComments();
			args.resize(4);
			args.push("Red\n�ʱ�ȭ�� ���� Red �����Դϴ�.\n�ִ� 255���� �Է°����մϴ�.");
			args.push("Green\n�ʱ�ȭ�� ���� Green �����Դϴ�.\n�ִ� 255���� �Է°����մϴ�.");
			args.push("Blue\n�ʱ�ȭ�� ���� Blue �����Դϴ�.\n�ִ� 255���� �Է°����մϴ�.");
			args.push("How will it be drawn\nȭ�鿡 ��� �׷����� ���մϴ�.");
		}

		return _header;
	}

	type_result BufferClearer::_onExecute()
	{
		//	main:
		DX9& dx9 = DX9::getInstancedDX();
		if( ! &dx9)
			return ALERT_ERROR(" : ����̽��� �ʱ�ȭ ���� �ʾ����Ƿ� �۾��� ����մϴ�.");

		LPDIRECT3DDEVICE9 device = dx9.getDevice();
		//		Ÿ����:
		type_uint color = D3DCOLOR_XRGB(arg_red.getValue(), arg_green.getValue(), arg_blue.getValue());
		
		if(FAILED(device->Clear(0, NULL, arg_flag.getValue(), color, 1.0f, 0)))
			return ALERT_ERROR(" : ���� Ŭ���� ����");		

		//	post:
		return RESULT_SUCCESS;
	}

	NEObject& BufferClearer::clone() const
	{
		return *(new BufferClearer(*this));
	}
}