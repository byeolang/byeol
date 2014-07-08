#include "DXFont.hpp"

namespace DX9Graphics
{
	NEExportable::ModuleHeader& DXFont::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("DXFont");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. DX9\n1��. ��Ʈ�� �̸�\n2��. ĳ���ͼ�(0:ANSI, 1:�⺻, 129:�ѱ�, 128:SHIFT�Ϻ�, 136:�߱�BIG5\n3��. ��\n4��. ����(���� ���� 2��)\n5��. �β�(100 ~ 400-����- ~ 900)\n6��. ���Ÿ�?");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-05");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(1);
		}

		return _header;
	}

	type_result DXFont::initialize()
	{
		SuperClass::initialize();

		NEKeyCodeSet& keyset = getKeySet();
		keyset.resize(keyset.getLength() + 6);
		keyset.push(NETStringKey(""));	//	��Ʈ �̸�
		keyset.push(NEUByteKey(DEFAULT_CHARSET));
		keyset.push(NEUIntKey());		//	��Ʈ�� ��
		keyset.push(NEUIntKey());		// ��Ʈ�� ����
		keyset.push(NEUIntKey());		//	��Ʈ�� �β�(100 ~ 400 ~ 900)
		keyset.push(NEBooleanKey());	//	���Ÿ�?

		return RESULT_SUCCESS;
	}

	type_result DXFont::execute()
	{
		if( ! _font)
			return initializeResource();
		return RESULT_SUCCESS;
	}

	NEObject& DXFont::clone() const
	{
		return *(new DXFont(*this));
	}

	type_result DXFont::initializeResource()
	{
		DX9& dx9 = getBinded();
		LPDIRECT3DDEVICE9 dev = &dx9 ? dx9.getDevice() : 0;
		if( ! &dev)
		{
			ALERT_ERROR("dx9 ���ε� ���з� ����");
			return RESULT_TYPE_ERROR;
		}


		//	main:
		if( ! _font)
		{
			D3DXFONT_DESC desc;
			_tcscpy_s(desc.FaceName, LF_FACESIZE, getName().toCharPointer());
			desc.CharSet = getCharSet();
			desc.Width = getWidth();
			desc.Height = getHeight();
			desc.Weight = getWeight();
			desc.Quality = DEFAULT_QUALITY;
			desc.MipLevels = 1;
			desc.Italic = isItalic() ? 1 : 0;
			desc.OutputPrecision = OUT_DEFAULT_PRECIS;
			desc.PitchAndFamily = FF_DONTCARE;

			D3DXCreateFontIndirect(dev, &desc, &_font);
			
			return SuperClass::initializeResource();
		}

		return RESULT_SUCCESS;
	}
}