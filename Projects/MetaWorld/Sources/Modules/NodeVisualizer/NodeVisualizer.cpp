#include "NodeVisualizer.hpp"
#include "../PanelBinder/PanelBinder.hpp"
#include "../NodeBinder/NodeBinder.hpp"
#include "../KeyBinder/KeyBinder.hpp"

namespace MetaWorld
{

	NENode& NodeVisualizer::_createAvartar(type_int length)
	{
		NENodeManager& noder = Kernal::getInstance().getNodeManager();	
		const NENode& source = _getSourceAvatarOf(getAvartarWhenSourceIsMostAncestor());

		NENode& avartar = noder.getRootNodes()[noder.getRootNodes().push(source)];	//	IndexedArrayTemplate::push/insert�� retVal�� ���Ե� index�� ��ȯ�Ѵ�.
		_onAvatarPushed(avartar, length);
		return avartar;
	}

	NENode& NodeVisualizer::_createAvartar(const NENode& source, NENode& parent_of_avartar, type_int index_between_siblings)
	{
		typedef DX9Graphics::Model Model;
		Model& connection = _searchIn<DX9Graphics::Model>(parent_of_avartar);
		if( ! &connection) 
		{	
			ALERT_ERROR("parent_of_avartar�� DX9Graphics::Model Ŭ������ ����.");
			__asm int 3;
		}
		NENode& avatar_source = _getSourceAvatarOf(getAvartarWhenSourceIsNode());
		NENodeCodeSet& ns = connection.getSubParticles();
		
		//	post:
		NENode& avatar = ns[ns.push(avatar_source)];
		_onAvatarPushed(avatar, source, index_between_siblings);
		return avatar;
	}

	NENode& NodeVisualizer::_createAvartar(const NEModule& source, NENode& parent_of_avartar, type_int index_between_siblings)
	{
		typedef DX9Graphics::Model Model;
		Model& connection = _searchIn<DX9Graphics::Model>(parent_of_avartar);
		if( ! &connection) 
		{	
			ALERT_ERROR("parent_of_avartar�� DX9Graphics::Model Ŭ������ ����.");
			__asm int 3;
		}
		NENode& avatar_source = _getSourceAvatarOf(getAvartarWhenSourceIsModule());
		NENodeCodeSet& ns = connection.getSubParticles();

		//	post:
		NENode& avatar = ns[ns.push(avatar_source)];
		_onAvatarPushed(avatar, source, index_between_siblings);
		return avatar;
	}

	NENode& NodeVisualizer::_createAvartar(const NEKey& source, NENode& parent_of_avartar, type_int index_between_siblings)
	{
		typedef DX9Graphics::Model Model;
		Model& connection = _searchIn<DX9Graphics::Model>(parent_of_avartar);
		if( ! &connection) 
		{	
			ALERT_ERROR("parent_of_avartar�� DX9Graphics::Model Ŭ������ ����.");
			__asm int 3;
		}
		NENode& avatar_source = _getSourceAvatarOf(getAvartarWhenSourceIsKey());
		NENodeCodeSet& ns = connection.getSubParticles();		

		//	post:
		NENode& avatar = ns[ns.push(avatar_source)];
		_onAvatarPushed(avatar, source, index_between_siblings);
		return avatar;
	}

	type_result NodeVisualizer::_searchInDepth(const NENode& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar)
	{
		//	main:
		//		�ϴ� �� �ڽ��� �����:
		NENode& my_avartar = _createAvartar(target, parent_of_avartar, index_between_siblings);
		//		���� ����� Ž��:
		const NEModuleCodeSet& modules = target.getModuleSet();
		for(int n=0; n < modules.getLength() ;n++)
			_searchInDepth(modules[n], depth+1, n, my_avartar);

		return RESULT_SUCCESS;
	}

	type_result NodeVisualizer::_searchInDepth(const NEModule& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar)
	{
		//	main:
		//		�� �ƹ�Ÿ�� ���� �����:
		NENode& my_avartar = _createAvartar(target, parent_of_avartar, index_between_siblings);
		//		���� Ű�� Ž��:
		const NEKeyCodeSet& keys = target.getKeySet();
		for(int n=0; n < keys.getLength() ;n++)
			_searchInDepth(keys[n], depth+1, n, my_avartar);

		return RESULT_SUCCESS;
	}

	type_result NodeVisualizer::_searchInDepth(const NEKey& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar)
	{
		//	main:
		//		�� �ƹ�Ÿ�� ���� �����:
		NENode& my_avartar = _createAvartar(target, parent_of_avartar, index_between_siblings);
		if(target.isSubClassOf(NEType::NENODE_CODESET_KEY))
		{
			const NENodeCodeSet& container = static_cast<const NENodeCodeSetKey&>(target).getValue();
			for(int n=0; n < container.getLength() ;n++)
				_searchInDepth(container[n], depth+1, n, my_avartar);
		}
		else if(target.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
		{
			const NEModuleCodeSet& container = static_cast<const NEModuleCodeSetKey&>(target).getValue();
			for(int n=0; n < container.getLength() ;n++)
				_searchInDepth(container[n], depth+1, n, my_avartar);
		}	

		return RESULT_SUCCESS;
	}

	template<typename TYPE_TO_FIND>
	TYPE_TO_FIND& NodeVisualizer::_searchIn(NENode& target)
	{
		const NECodeSet& cs = TYPE_TO_FIND::getScriptCodeSet();
		NEModuleCodeSet& ms = target.getModuleSet();

		for(int n=0; n < ms.getLength() ;n++)
			if(cs.find(ms[n].getScriptCode()) != NE_INDEX_ERROR)
				return static_cast<TYPE_TO_FIND&>(ms[n]);

		TYPE_TO_FIND* nullptr = 0;
		return *nullptr;
	}

	NENode& NodeVisualizer::_getSourceAvatarOf(NENodeSelector& selector)
	{
		NENode& to_return = selector.getNode();
		selector.initializeReferingPoint();
		
		return to_return;
	}

	type_result NodeVisualizer::_onAvatarPushed(NENode& avatar, const NEModule& source, type_index index_between_siblings)
	{
		//	����� �ƹ�Ÿ�� �����ɶ�:
		PanelBinder& binder = _searchIn<PanelBinder>(avatar);
		if( ! &binder)
		{
			ALERT_ERROR("�����Ϸ��� ���ƹ�Ÿ�� PanelBinder�� �����ϴ�.");
			return RESULT_TYPE_ERROR;
		}

		//	main:		
		//		������ ���� ����� ����ŭ, �ƹ�Ÿ�� ������ ũ�⸦ �̸�Ȯ��:
		DX9Graphics::Model& model = _searchIn<DX9Graphics::Model>(avatar);
		if( ! &model)
			ALERT_ERROR("�ƹ�Ÿ�� DX9Graphics::Model �� �����ϴ�. �������� ������ �����߽��ϴ�.")
		else
		{
			model.getSubParticles().create(source.getKeySet().getLength());
			_updateModelBy(model, index_between_siblings);
		}
		//		���� ���� ���ε�:
		NEModuleSelector& to_be_binded = binder.getSourceSelector();
		to_be_binded.getBinder().bind(source, NEType::NESCRIPT_EDITOR);
		//		Text�� ����� �̸����� ����:
		using DX9Graphics::SpriteTexter;
		SpriteTexter& texter = _searchIn<SpriteTexter>(avatar);
		texter.getText() = source.getHeader().getName();

		return RESULT_SUCCESS;
	}

	type_result NodeVisualizer::_onAvatarPushed(NENode& avatar, const NENode& source, type_index index_between_siblings)
	{
		//	����� �ƹ�Ÿ�� �����ɶ�:
		//		���� ���� ���ε�:
		NodeBinder& binder = _searchIn<NodeBinder>(avatar);
		if( ! &binder)
		{
			ALERT_ERROR("�����Ϸ��� ���ƹ�Ÿ�� NodeBinder�� �����ϴ�.");
			return RESULT_TYPE_ERROR;
		}

		//	main:
		//		������ ���� ����� ����ŭ, �ƹ�Ÿ�� ������ ũ�⸦ �̸�Ȯ��:
		DX9Graphics::Model& model = _searchIn<DX9Graphics::Model>(avatar);
		if( ! &model)
			ALERT_ERROR("�ƹ�Ÿ�� DX9Graphics::Model �� �����ϴ�. �������� ������ �����߽��ϴ�.")
		else
		{
			model.getSubParticles().create(source.getModuleSet().getLength());
			_updateModelBy(model, index_between_siblings);
		}
		NENodeSelector& to_be_binded = binder.getSourceSelector();
		to_be_binded.getBinder().bind(source, source.getManager().getType());
		//		Text�� "ĳ����"�� ����:
		using DX9Graphics::SpriteTexter;
		SpriteTexter& texter = _searchIn<SpriteTexter>(avatar);
		texter.getText() = "ĳ����";

		return RESULT_SUCCESS;
	}

	type_result NodeVisualizer::_onAvatarPushed(NENode& avatar, const NEKey& source, type_index index_between_siblings)
	{
		//	����� �ƹ�Ÿ�� �����ɶ�:
		//		���� ���� ���ε�:
		DX9Graphics::Model& model = _searchIn<DX9Graphics::Model>(avatar);
		KeyBinder& binder = _searchIn<KeyBinder>(avatar);
		if( ! &binder)
		{
			ALERT_ERROR("�����Ϸ��� ���ƹ�Ÿ�� KeyBinder�� �����ϴ�.");
			return RESULT_TYPE_ERROR;
		}

		//	main:
		if(source.isSubClassOf(NEType::NENODE_CODESET_KEY))
		{
			//		������ ���� ����� ����ŭ, �ƹ�Ÿ�� ������ ũ�⸦ �̸�Ȯ��:
			if( ! &model)
				ALERT_ERROR("�ƹ�Ÿ�� DX9Graphics::Model �� �����ϴ�. �������� ������ �����߽��ϴ�.")
			else
			{
				NENodeCodeSetKey& casted = (NENodeCodeSetKey&) source;
				model.getSubParticles().create(casted.getValue().getLength());
			}
		}
		else if(source.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
		{
			//		������ ���� ����� ����ŭ, �ƹ�Ÿ�� ������ ũ�⸦ �̸�Ȯ��:
			DX9Graphics::Model& model = _searchIn<DX9Graphics::Model>(avatar);
			if( ! &model)
				ALERT_ERROR("�ƹ�Ÿ�� DX9Graphics::Model �� �����ϴ�. �������� ������ �����߽��ϴ�.")
			else
			{
				NEModuleCodeSetKey& casted = (NEModuleCodeSetKey&) source;
				model.getSubParticles().create(casted.getValue().getLength());
			}
		}
		NEKeySelector& to_be_binded = binder.getSourceSelector();
		to_be_binded.getBinder().bind(source, NEType::NESCRIPT_EDITOR);
		//		Text�� "Key�� Ÿ���̸�"�� ����:
		using DX9Graphics::SpriteTexter;
		SpriteTexter& texter = _searchIn<SpriteTexter>(avatar);
		texter.getText() = source.getTypeName();
		_updateModelBy(model, index_between_siblings);
		return RESULT_SUCCESS;
	}

	type_result NodeVisualizer::_onAvatarPushed(NENode& avatar, type_int length_to_create)
	{
		using namespace DX9Graphics;
		//	��Ʈ ����� �ƹ�Ÿ�� �����ɶ�:
		//		��ũ��Ʈ�� ����ŭ �̸� Resize:
		Model& model = _searchIn<Model>(avatar);
		if( ! &model)
			ALERT_ERROR("�ƹ�Ÿ�� DX9Graphics::Model �� �����ϴ�.")
		else
			model.getSubParticles().create(length_to_create);
		//		Text�� "����"�� ����:		
		SpriteTexter& texter = _searchIn<SpriteTexter>(avatar);
		texter.getText() = "����";
		return RESULT_SUCCESS;
	}

}