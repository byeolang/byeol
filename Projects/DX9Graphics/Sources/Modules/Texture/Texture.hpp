#pragma once

#include "../FileResource/FileResource.hpp"

namespace DX9Graphics
{
	class NE_DLL Texture : public FileResource
	{
	public:
		typedef FileResource SuperClass;
		typedef Texture ThisClass;

	public:
		Texture()
			: _texture(0)
		{
			ThisClass::releaseResource();
		}
		~Texture()
		{
			ThisClass::releaseResource();
		}

	public:
		NETArgument<NEUByteKey>	arg_transparent_red;
		NETArgument<NEUByteKey>	arg_transparent_green;
		NETArgument<NEUByteKey>	arg_transparent_blue;
		NETArgument<NEUByteKey>	arg_transparent_alpha;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass::_onFetchArguments(tray);

			tray.push(arg_transparent_red);
			tray.push(arg_transparent_green);
			tray.push(arg_transparent_blue);
			tray.push(arg_transparent_alpha);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_transparent_red.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_transparent_green.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_transparent_blue.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_transparent_alpha.setValue(255);
			arg_transparent_alpha.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute()
		{
			if (!isEnable()) return RESULT_SUCCESS;
			if (isResourceInitialized())
				return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			return initializeResource();
		}

	public:
		IDirect3DTexture9& getTexture()
		{
			return *_texture;
		}
		const IDirect3DTexture9& getTexture() const
		{
			return *_texture;
		}
		type_uint getWidth() const
		{
			if( ! _texture)
				return ALERT_WARNING("�ؽ��İ� ���µ� �ʺ� �˼��� ������");

			return _texture_description.Width;
		}
		type_uint getHeight() const
		{
			if( ! _texture)
				return ALERT_WARNING("�ؽ��İ� ���µ� ���̸� �˼��� ������");

			return _texture_description.Height;
		}
		virtual type_uint getWidthOfOneFrame() const
		{
			return getWidth();
		}
		virtual type_uint getHeightOfOneFrame() const
		{
			return getHeight();
		}
		D3DSURFACE_DESC getTextureDescription() const
		{
			return _texture_description;
		}
		type_uint createColorKey() const
		{
			return D3DCOLOR_RGBA(arg_transparent_red.getValue(), arg_transparent_green.getValue(), 
				arg_transparent_blue.getValue(), arg_transparent_alpha.getValue());
		}

	public:
		virtual RECT createSourceRect() const
		{
			RECT to_return = {
				0,			0, 
				getWidth(), getHeight()
			};

			return to_return;
		}
		virtual type_result releaseResource()
		{
			if(_texture)
				_texture->Release();
			_texture = 0;

			memset(&_texture_description, 0, sizeof(_texture_description));

			return SuperClass::releaseResource();
		}
		virtual type_result initializeResource();
		virtual type_result dock(Model& model);
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(NEResult::hasError(_instance.isValid()))
			{
				_instance.getName() = "Texture";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getComment() =
					"�𵨸��� �����ϰ� �� �׸�, �� �ؽ��� Module �Դϴ�.\n"
					"�ؽ��Ŀ��� �������ν�, �ε�� ������ �κ��� ColorKey�ν� ������ �� �ֽ��ϴ�.\n"
					"�������, TransparentRed = 0, Blue = 0, Green = 0, Alpha = 255�� �����ϸ�, �׸����� �������κ��� ������� �˴ϴ�.";
				_instance.getVersion() = "0.0.1a";
				_instance.getReleaseDate() = "2013-08-07";	
				NETStringSet& args = _instance.getArgumentsComments();
				args.create(5);
				args.push("���� �׸��� ���\n.jpg, .bmp, .png ���� �����մϴ�.");
				args.push("Transparent Red Key\n������ ������ �����Դϴ�. 255�� �ִ밪�Դϴ�.");
				args.push("Transparent Green Key\n������ �ʷϻ� �����Դϴ�. 255�� �ִ밪�Դϴ�.");
				args.push("Transparent Blue Key\n������ �Ķ��� �����Դϴ�. 255�� �ִ밪�Դϴ�.");
				args.push("Transparent Alpha Key\n������ ������ �Դϴ�. 255�� �ִ밪�Դϴ�.");
			}

			return _instance;
		}
		virtual void release()
		{
			SuperClass::release();

			ThisClass::releaseResource();
		}
		NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
		{
			SuperClass::serialize(loader);

			ThisClass::releaseResource();

			return loader;
		}
		virtual NEObject& clone() const
		{
			return *(new Texture(*this));
		}

	private:
		bool _isTextureSizePowerOf2(D3DXIMAGE_INFO& image_info)
		{
			if( ! _isPowerOf2(image_info.Width)		||
				! _isPowerOf2(image_info.Height)	)
				return false;

			return true;
		}
		bool _isPowerOf2(type_uint size)
		{
			while(1)
			{
				type_uint remainder = size % 2;
				size /= 2;

				if(remainder)
					return false;
				if(size == 1)
					break;
			}

			return true;
		}

	private:
		LPDIRECT3DTEXTURE9 _texture;
		D3DSURFACE_DESC _texture_description;
	};
}
