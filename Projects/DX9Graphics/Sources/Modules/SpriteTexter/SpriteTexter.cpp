#include "SpriteTexter.hpp"
#include "../DXFont/DXFont.hpp"
#include "../Texture/Texture.hpp"

namespace DX9Graphics
{
	NEExportable::ModuleHeader& SpriteTexter::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("SpriteTexter");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. ����� ��Ʈ\n1��. ����� ����\n2��. ��������(0:����, 1:���, 2:������)\n3��. ��������(4:���, 8:�Ʒ�, 0:��)\n4: ���� ����\n5: �ʷ� ���� \n6: �Ķ� ���� \n7: Alpha��");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-05");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	type_result SpriteTexter::initialize()
	{
		SuperClass::initialize();

		NEKeyCodeSet& keyset = getKeySet();
		keyset.resize(keyset.getLength() + 8);
		keyset.push(NEModuleSelector());		//	0: ����� ��Ʈ
		keyset.push(NETStringKey());			//	1: ����� �ؽ�Ʈ
		keyset.push(NEUByteKey());				//	2: ���� ����
		keyset.push(NEUByteKey());				//	3: ���� ����
		keyset.push(NEUByteKey(255));			//	4: ColorRed
		keyset.push(NEUByteKey(255));			//	5: ColorGreen
		keyset.push(NEUByteKey(255));			//	6: ColorBlue
		keyset.push(NEUByteKey(255));			//	7: ColorAlpha

		return RESULT_SUCCESS;
	}

	type_result SpriteTexter::execute()
	{		
		if( ! getFontSelector().getBinder().isBinded())
			_bindFont();
		return RESULT_SUCCESS;
	}

	NEObject& SpriteTexter::clone() const
	{
		return *(new SpriteTexter(*this));
	}

	type_result SpriteTexter::render(LPD3DXSPRITE sprite, const Texture& texture)
	{
		if( ! sprite)
		{
			ALERT_ERROR("��������Ʈ ���ε� ����");
			return RESULT_TYPE_ERROR;
		}
		if( ! &texture)
		{
			ALERT_ERROR("�ؽ��İ� �߸��Ǿ����ϴ�.")
			return RESULT_TYPE_ERROR;
		}
		NEModuleBinder& binder = getFontSelector().getBinder();
		if( ! binder.isBinded())
			return RESULT_TYPE_WARNING;

		LPD3DXFONT fnt = ((DXFont&) binder.getBinded()).getFont();
		if( ! fnt)
		{
			ALERT_ERROR("��Ʈ ���ε� ����");
			return RESULT_TYPE_ERROR;
		}
		
		
		//	main:
		DWORD flag = getHorizontalAlignment() | getVerticalAlignment();
		fnt->DrawText(sprite, getText().toCharPointer(), -1, &_createFontArea(texture), flag, createColor());
		
		//	post:
		return RESULT_SUCCESS;
	}

	type_result SpriteTexter::_bindFont()
	{
		NEModuleSelector& sel = getFontSelector();
		while(NEModule* itr = &sel.getModule())
		{
			static type_code code = Kernal::getInstance().getModuleManager()
				.getModule(DXFont().getHeader()).getScriptCode();
			if(itr->getScriptCode() == code)
			{
				sel.bind();
				sel.initializeReferingPoint();
				return RESULT_SUCCESS;
			}
		}

		ALERT_WARNING("��Ʈ ���ε� ����");
		return RESULT_TYPE_WARNING;
	}

	RECT SpriteTexter::_createFontArea(const Texture& texture)
	{
		type_int	hw = texture.getWidthOfOneFrame() / 2,
					hh = texture.getHeightOfOneFrame() / 2;
		RECT to_return = {-hw, -hh, hw, hh};
		
		return to_return;
	}

}