#pragma once

#include "../../Includes/Includes.hpp"

namespace MetaWorld
{
	class NE_DLL WorldVisualizer : public NEModule
	{
	public:
		typedef WorldVisualizer ThisClass;
		typedef NEModule SuperClass;

	public:
		NENodeSelector& getVisualNodeSelector()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getVisualNodeSelector());
		}
		const NENodeSelector& getVisualNodeSelector() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key || ! key.isSubClassOf(NEType::NENODE_SELECTOR))
			{
				ALERT_ERROR("0��Ű�� NENODE_SELECTOR���� �մϴ�.");
				const NENodeSelector* nullpoint = 0;
				return *nullpoint;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NEKeySelector& getScriptCodeSelector()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NEKeySelector&>(casted->getScriptCodeSelector());
		}
		const NEKeySelector& getScriptCodeSelector() const
		{
			const NEKey& key = getKeySet()[1];
			if( ! &key || ! key.isSubClassOf(NEType::NEKEY_SELECTOR))
			{
				ALERT_ERROR("1��Ű�� NEKEY_SELECTOR���� �մϴ�.");
				const NEKeySelector* nullpoint = 0;
				return *nullpoint;
			}

			return static_cast<const NEKeySelector&>(key);
		}

	public:
		virtual type_result initialize()
		{
			NEKeyCodeSet& cont = getKeySet();
			cont.create(2);
			cont.push(NENodeSelector());	//	0:	��带 ã���� �� �����͸� ã�� ��带 ��������� �����Ѵ�. ��, �̾ȿ��� 1�� �̻��� Model Ȥ�� AnimatedModel�� �־�� �Ѵ�.
			cont.push(NEKeySelector());		//	1:	0�� ��带 �߰��ϱ�����, ScriptEditor�� ���� Script�� ����Ű���� �� Ű�� ������´�.
			return RESULT_SUCCESS;
		}
		virtual type_result execute();
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.getName().getSize() <= 0)
			{
				_instance.getName() = "WorldVisualizer";
				_instance.getDeveloper() = "kniz";
				_instance.setRevision(1);
				_instance.getReleaseDate() = "2013-10-21";
				_instance.getVersion() = "0.0.1a";
				_instance.getComment() = "ScriptEditor�� ��尡 ���ö����� 0���� �ִ� ��带 �������ݴϴ�.\n�����ÿ��� ���������� ����ϴµ�, ������� ScriptEditor�� Node1�ȿ� Node2�� �����ִ� ���¶��, \n �̰��� ���� ���·� NodeManager�� ����� �ݴϴ�. 0��Ű�κ��� ���� ���� �ּ� 1�� �̻��� \nModel/AnimatedModel�� 1��Ű�� ������ IntKey�� ������ �־�� �մϴ�.\n0: �����ϰԵ� Node(ó�� 1�� �ܿ��� ����)\n1: 0�� ��尡 ������ �ִ�, ScriptEditor�� ScriptCode�� ����Ű�� ���� ������ Ű";
			}

			return _instance;
		}
		virtual NEObject& clone() const { return *(new ThisClass(*this)); }

	private:		
		DX9Graphics::Model& _castFrom(NEModule& module);
		type_result _searchModuleSet(NEModuleCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder);
		type_result _searchKeySet(NEKeyCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder);
		type_result _searchNodeSet(NENodeCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder);
		type_result _updateKeyToGivenScriptCode(NENode& to_be_push, type_code scriptcode);
		NENodeCodeSet& _getInnerContainer(NENode& target);		
	};
}