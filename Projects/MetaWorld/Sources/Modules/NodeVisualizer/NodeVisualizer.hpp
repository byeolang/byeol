#pragma once

#include "../../Includes/Includes.hpp"

namespace MetaWorld
{
	class NE_DLL NodeVisualizer : public NEModule
	{
	public:
		typedef NodeVisualizer ThisClass;
		typedef NEModule SuperClass;

	public:
		const NENodeSelector& getSources() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("0��Ű�� NODE_SELECTOR���� �մϴ�.");
				NENodeSelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getSources()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getSources());
		}
		const NENodeSelector& getAvartarWhenSourceIsMostAncestor() const
		{
			const NEKey& key = getKeySet()[1];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("1��Ű�� NODE_SELECTOR���� �մϴ�.");
				NENodeSelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getAvartarWhenSourceIsMostAncestor()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getAvartarWhenSourceIsMostAncestor());
		}
		const NENodeSelector& getAvartarWhenSourceIsNode() const
		{
			const NEKey& key = getKeySet()[2];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("2��Ű�� NODE_SELECTOR���� �մϴ�.");
				NENodeSelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getAvartarWhenSourceIsNode()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getAvartarWhenSourceIsNode());
		}
		const NENodeSelector& getAvartarWhenSourceIsModule() const
		{
			const NEKey& key = getKeySet()[3];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("3��Ű�� NODE_SELECTOR���� �մϴ�.");
				NENodeSelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getAvartarWhenSourceIsModule()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getAvartarWhenSourceIsModule());
		}
		const NENodeSelector& getAvartarWhenSourceIsKey() const
		{
			const NEKey& key = getKeySet()[4];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("4��Ű�� NODE_SELECTOR���� �մϴ�.");
				NENodeSelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getAvartarWhenSourceIsKey()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getAvartarWhenSourceIsKey());
		}
		const type_float& getDeltaXPerDepth() const
		{
			const NEKey& key = getKeySet()[5];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("5��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaXPerDepth()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaXPerDepth());
		}
		const type_float& getDeltaYPerDepth() const
		{
			const NEKey& key = getKeySet()[6];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("6��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaYPerDepth()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaYPerDepth());
		}
		const type_float& getDeltaZPerDepth() const
		{
			const NEKey& key = getKeySet()[7];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("7��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaZPerDepth()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaZPerDepth());
		}
		const type_float& getDeltaXPerIndex() const
		{
			const NEKey& key = getKeySet()[8];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("8��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaXPerIndex()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaXPerIndex());
		}
		const type_float& getDeltaYPerIndex() const
		{
			const NEKey& key = getKeySet()[9];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("9��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaYPerIndex()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaYPerIndex());
		}
		const type_float& getDeltaZPerIndex() const
		{
			const NEKey& key = getKeySet()[10];
			if( ! &key || ! key.isSubClassOf(NEType::NEFLOAT_KEY))
			{
				ALERT_ERROR("10��Ű�� NEFLOAT_KEY���� �մϴ�.");
				type_float* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEFloatKey&>(key).getValue();
		}
		type_float& getDeltaZPerIndex()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_float&>(casted->getDeltaZPerIndex());
		}

	public:
		virtual type_result initialize()
		{
			NEKeyCodeSet& ks = getKeySet();
			ks.create(11);
			NENodeSelector s;
			s.setManager(NEType::NESCRIPT_EDITOR);
			ks.push(s);					//	0:	�ð�ȭ �����͸� ���� �ֻ��� ���������.
			s.setManager(NEType::NESCRIPT_MANAGER);
			ks.push(s);	//	1:	�ֻ��� ��带 �����ͷ� ���鶧 ����� ���� ����
			ks.push(s);	//	2:	��带 �����ͷ� �����Ҷ� ����� �������
			ks.push(s);	//	3:	��⸦ �����ͷ� �����Ҷ� ����� �������
			ks.push(s);	//	4:	Ű�� �����ͷ� �����Ҷ� ����� �������
			ks.push(NEFloatKey(0));		//	5:	�ϳ��� ���̴� X��ȭ
			ks.push(NEFloatKey(0));		//	6:	�ϳ��� ���̴� Y��ȭ
			ks.push(NEFloatKey(10));	//	7:	�ϳ��� ���̴� Z��ȭ
			ks.push(NEFloatKey(0));		//	8:	�ϳ��� �ε����� X��ȭ
			ks.push(NEFloatKey(10));	//	9:	�ϳ��� �ε����� Y��ȭ
			ks.push(NEFloatKey(0));		//	10:	�ϳ��� �ε����� Z��ȭ
			return RESULT_SUCCESS;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.getName().getSize() <= 0)
			{
				_instance.getName() = "NodeVisualizer";
				_instance.setRevision(1);
				_instance.getDeveloper() = "kniz";
				_instance.getComment() = "0: �ð�ȭ�����͸� ���� ���(������嵵 Ž���Ѵ�)\n1: �ֻ��� ����� �ƹ�Ÿ\n2:����� �ƹ�Ÿ\n3:����� �ƹ�Ÿ\n4:Ű�� �ƹ�Ÿ\n5:���̴� X��ȭ\n6:���̴� Y��ȭ\n7:���̴� Z��ȭ\n8:�ε����� X��ȭ:\n9:�ε����� Y��ȭ\n10:�ε����� Z��ȭ";
			}

			return _instance;
		}

	public:
		virtual type_result execute()
		{
			using namespace NE;
			//	pre:
			//		�ֻ��� ����� �ƹ�Ÿ�� �����ϱ�:
			NENodeSelector& srcs_sel = getSources();

			//	main:
			//		���� Ư�� ��尡 �����Ǿ��ٸ�:
			//			�� ��� �ȿ��� �ǽ�:
			if(srcs_sel.getNodeType() != NECodeType::ALL)
			{	//			��Ʈ ��� ���� ����:
				int length = 0;
				while(&srcs_sel.getNode())
					length++;
				
				NENode& avartar_of_soma/*StubOfMostAncestor*/ = _createAvartar(length);

				type_int index = 0;
				while(NENode* src = &srcs_sel.getNode())
					_searchInDepth(*src, 0, index++, avartar_of_soma);
			}
			else	//		��Ʈ ��� ���� ����:
			{	
				const NENodeCodeSet& scripts = Editor::getInstance().getScriptEditor().getScriptNodes();
				
				NENode& avartar_of_soma = _createAvartar(scripts.getLength());

				for(int n=0; n < scripts.getLength() ;n++)
					_searchInDepth(scripts[n], 0, n, avartar_of_soma);
			}
			
			return RESULT_SUCCESS;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	private:
		NENode& _createAvartar(type_int length);
		NENode& _createAvartar(const NENode& source, NENode& parent_of_avartar, type_int index_between_siblings);
		NENode& _createAvartar(const NEModule& source, NENode& parent_of_avartar, type_int index_between_siblings);
		NENode& _createAvartar(const NEKey& source, NENode& parent_of_avartar, type_int index_between_siblings);
		type_result _searchInDepth(const NENode& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar);
		type_result _searchInDepth(const NEModule& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar);
		type_result _searchInDepth(const NEKey& target, type_int depth, type_int index_between_siblings, NENode& parent_of_avartar);
		template <typename TYPE_TO_FIND>
		TYPE_TO_FIND& _searchIn(NENode& target);
		NENode& _getSourceAvatarOf(NENodeSelector& selector);
		type_result _onAvatarPushed(NENode& avatar, type_int length_to_create);
		type_result _onAvatarPushed(NENode& avatar, const NENode& source, type_index index_between_siblings);
		type_result _onAvatarPushed(NENode& avatar, const NEModule& source, type_index index_between_siblings);
		type_result _onAvatarPushed(NENode& avatar, const NEKey& source, type_index index_between_siblings);
		type_result _updateModelBy(DX9Graphics::Model& model, type_index index_between_siblings)
		{
			model.getTranslationX() = getDeltaXPerDepth() + getDeltaXPerIndex() * index_between_siblings;
			model.getTranslationY() = getDeltaYPerDepth() + getDeltaYPerIndex() * index_between_siblings;
			model.getTranslationZ() = getDeltaZPerDepth() + getDeltaZPerIndex() * index_between_siblings;

			return RESULT_SUCCESS;
		}
	};
}