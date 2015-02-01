#pragma once

#include "../DockableResource/DockableResource.hpp"

namespace DX9Graphics
{
	class Texture;
	class TabledTexture;

	class NE_DLL Sprite : public DockableResource
	{
	public:
		typedef Sprite ThisClass;
		typedef DockableResource SuperClass;

	public:
		NETArgument<NEFloatKey>	arg_bounding_sphere_radius;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_bounding_sphere_radius);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			return arg_bounding_sphere_radius.setPurposeLimitation(NEArgumentBase::READ_BY);			
		}

	public:
		Sprite()
			: SuperClass(), _sprite(0)
		{

		}
		~Sprite()
		{
			ThisClass::releaseResource();
		}

	public:
		LPD3DXSPRITE getSprite() { return _sprite; }
		const LPD3DXSPRITE getSprite() const { return _sprite; }

	public:
		virtual type_result dock(Model& model);
		virtual type_result initializeResource()
		{
			_initializeSprite();

			return SuperClass::initializeResource();
		}
		virtual type_result releaseResource()
		{
			if (_sprite)
				_sprite->Release();
			_sprite = 0;

			return SuperClass::releaseResource();
		}
		virtual type_result execute()
		{
			if (!_sprite)
				initializeResource();

			return RESULT_SUCCESS;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if(_header.isValid() != RESULT_NOTHING)
			{
				_header.getName()		= "Sprite";
				_header.getDeveloper()	= "kniz";
				_header.setRevision(1);
				_header.getComment() = 
					"3D������ �׸�(Texture)�� ������ �簢�� ����Դϴ�.\n�浹�˻綧 ���� BoundingSphere�� �������� ������ �� �ֽ��ϴ�.(�ڵ����� ������ �ʽ��ϴ�)";
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2013-08-10";

				NETStringSet& args = _header.getArgumentsComments();
				args.resize(1);
				args.push("BoundingSphere Radius\n�浹�˻綧 ���� ������ ���� �������Դϴ�. ũ�� Ŭ���� �浹 ���ΰ� �ΰ������ϴ�.");
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

			ThisClass::releaseResource();
		}

	private:
		type_result _initializeSprite();
		type_result _renderSprite(Model& model, Texture& texture, LPDIRECT3DDEVICE9 device);

	private:
		LPD3DXSPRITE _sprite;

	public:
		static const NECodeSet& getModuleScriptCodes()
		{
			NEModuleManager& moduler = Kernal::getInstance().getModuleManager();
			NECodeType type(NECodeType::MODULE_SCRIPT);
			static NECodeSet codeset(type);
			if (codeset.getLength() <= 0)
			{				
				codeset.create(1);
				codeset.push(NEExportable::Identifier("Sprite"));
			}

			return codeset;
		}
	};
}