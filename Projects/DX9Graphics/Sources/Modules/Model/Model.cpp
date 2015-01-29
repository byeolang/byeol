#include "Model.hpp"
#include "../DX9/DX9.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Texture/Texture.hpp"
#include "../TabledTexture/TabledTexture.hpp"
#include "../SpriteTexter/SpriteTexter.hpp"
#include "../AnimatedModel/AnimatedModel.hpp"

namespace DX9Graphics
{
	SpriteTexter& Model::getTexter()
	{
		return DX9::cast<SpriteTexter>(arg_texter_binder.getValue().getModule());
	}
	Texture& Model::getTexture()
	{
		return DX9::cast<Texture>(arg_texture_binder.getValue().getModule());
	}
	Sprite& Model::getModeling()
	{
		return DX9::cast<Sprite>(arg_modeling_binder.getValue().getModule());
	}
	type_result Model::_onExecute()
	{
		//	pre:			
		if(!isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;


		//	main:
		//		ParentWorldMatrix �Ҵ�ޱ�:
		_parent_world = _getGlobalParentWorld();
		_updateModelMatrix();
		_updateWorldMatrix();


		//	post:
		//		�ڽ� ���� ����:
		_executeChildren();
		_getGlobalParentWorld() = _parent_world;
		return RESULT_SUCCESS;
	}

	const NEExportable::ModuleHeader& Model::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName()		= "Model";
			_header.getDeveloper()	= "kniz";
			_header.setRevision(1);
			_header.getComment() = "3���� ������ ���ε��� Resource���� ��� �����ν� ǥ���ϴ� ����Դϴ�.\n��� �׸��� ��Ÿ���� �ؽ��ĸ� ��ü�� ��Ÿ���� �𵨸��� ������ ������ ��ǥ�� ����, ũ��� ����մϴ�.";
			_header.getVersion() = "0.0.1a";
			_header.getReleaseDate() = "2013-08-08";

			NETStringSet& args = _header.getArgumentsComments();
			args = SuperClass::getHeader().getArgumentsComments();
			args.resize(args.getLength() + 4);
			args.push("Modeling\nSprite�� ���� �𵨸� ����� �����մϴ�.");
			args.push("Texture\nSprite�� ������ �׸� �ؽ��� ����� �����մϴ�.");
			args.push("Texter\nSprite�� �׸��� ������ ���������� ������ SpriteTexter ��⸦ �����մϴ�.");
			args.push("Children\n���� �������� ���Դϴ�.\n�ڽ� ���� �θ� ���� ��ǥ,ȸ��,ũ�⸦ �������� ����ϴ� ��밪 �Դϴ�.");
		}

		return _header;
	}

	type_result Model::render()
	{
		if( ! isEnable()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;		
		if( ! arg_modeling_binder.isEnable()||
			! arg_texture_binder.isEnable()	) 
			return RESULT_SUCCESS | RESULT_ABORT_ACTION;

		DockableResource& modeling = DX9::cast<DockableResource>(arg_modeling_binder.getValue().getModule());
		Texture& texture = DX9::cast<Texture>(arg_texture_binder.getValue().getModule());	

		texture.dock(*this);
		_updateRenderState();
		return modeling.dock(*this);
	}

	const NECodeSet& Model::getModuleScriptCodes()
	{
		NECodeType type(NECodeType::MODULE_SCRIPT);
		static NECodeSet instance(type);

		if(instance.getSize() <= 0)
		{
			instance.create(1);
			instance.push(NEExportable::Identifier("AnimatedModel.kniz"));
		}

		return instance;
	}

	type_result Model::_updateRenderState()
	{
		DX9& dx9 = DX9::getInstancedDX();
		LPDIRECT3DDEVICE9 dev = &dx9 ? dx9.getDevice() : 0;
		if( ! dev)
			return ALERT_WARNING("DX9 ���ε� ���з� RenderState ���� ����");


		//	main:
		type_byte	src = arg_source_blend.getValue(),
					dest = arg_dest_blend.getValue();
		if(src)
			dev->SetRenderState(D3DRS_SRCBLEND, src);
		if(dest)
			dev->SetRenderState(D3DRS_DESTBLEND, dest);

		return RESULT_SUCCESS;
	}
}