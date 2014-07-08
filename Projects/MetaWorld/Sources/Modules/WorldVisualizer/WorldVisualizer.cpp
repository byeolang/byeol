#include "WorldVisualizer.hpp"

namespace MetaWorld
{
	type_result WorldVisualizer::execute()
	{
		NENodeCodeSet	&se = Editor::getInstance().getScriptEditor().getScriptNodes(),
						&nm = Kernal::getInstance().getNodeManager().getRootNodes();
		

		return _searchNodeSet(se, nm);
	}

	DX9Graphics::Model& WorldVisualizer::_castFrom(NEModule& module)
	{
		using namespace DX9Graphics;
	
		if(DX9Graphics::Model::getScriptCodeSet().find(module.getScriptCode()) != NE_INDEX_ERROR)
			return static_cast<Model&>(module);

		Model* nullpoint = 0;
		return *nullpoint;
	}

	type_result WorldVisualizer::_searchModuleSet(NEModuleCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder)
	{
		return 0;
	}

	type_result WorldVisualizer::_searchKeySet(NEKeyCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder)
	{
		return 0;
	}

	type_result WorldVisualizer::_searchNodeSet(NENodeCodeSet& sources_in_editor, NENodeCodeSet& targets_in_noder)
	{
		//	�˰���:
		//		����:
		//			ScriptEditor�� ��尣�� ���������� �״�� ������ NodeManager
		//			�� �߰��Ѵ�.
		//			�̶�, 
		//				1 - NodeManager���� 0��Ű���� ������ ���A 1���� ��� push�ϸ�, 
		//				2 - ���A�� AnimatedModel, Model�� 1�� �̻� ������ �־�� �ϸ�,
		//				3 - 1��Ű�� ���A�� ��� ��ũ��Ʈ�ڵ����� �ٿ��ֱ� �Ѵ�.
		//			�� ��ũ��Ʈ�ڵ�� �Ϲ������� ���A�ȿ� �ִ� �� ����Ѵ�.
		//			�׷��� NodeManager�� �߰��� ������ ScriptEditor�� ���� 
		//			��ũ��Ʈ���� "�����Դ°�"�� �� �� �ֵ��� "������"�� �����Ѵ�.
		//
		//		����:
		//			1	- ��� push�� ���A�� ã�´�.
		//			2	- sources�� Ž���ϴµ�, ������ ��带 source�� ����.
		//				sources�� �ִ� ��� ��带 ã�µ�,
		//			3	- source�� ScriptCode�� ���Ѵ�. 1��Ű�� �����ϱ� ���ؼ���.
		//			4	- 1��Ű���� ã�� ��� Ű�� �� ���� �����Ѵ�. 
		//				(= _updateKeyToGivenScriptCode)
		//			5	- ���A�� targets�� push �Ѵ�.
		//			6	- source�ȿ� �� ��尡 �ִ��� NodeCodeSetKey�� Ž���Ѵ�.
		//				(= _getInnerContainer)
		//			7	- �ִٸ�,
		//			7-1	- ���ο� sources�� �ٽ� ����.
		//			7-2	- ���ٸ�, 2������ ���ư���.
		//
		//		�ٽ� �ĺ��� ����:
		//			sources_in_editor	- Ž���� Source. ScriptEditor�� ��ġ�ϰ� ������, ��
		//								������ ������ �ȴ�. (const)
		//			source				- Editor �ȿ� �ִ� ��� 1��.
		//			targets_in_noder	- Ž���� Source�� �߰��� ���.
		//			recruit				- ��� push�� ���. 0��Ű���� 1���� �����´�.		


		//	pre:		
		NENodeSelector& sel = getVisualNodeSelector();
		NENode& recruit = sel.getNode();
		if( ! &recruit)
		{
			ALERT_ERROR("0��Ű�� ��尡 �����ϴ�.");
			return RESULT_TYPE_ERROR;
		}
		sel.initializeReferingPoint();	//	0��Ű���� ��� 1���� targets_in_noder�� push �Ѵ�.
		

		//	main:
		for(int n=0; n < sources_in_editor.getSize() ;n++)
		{
			NENode& source = sources_in_editor[n];
			//	ScriptCode �Ҵ�:
			//_updateKeyToGivenScriptCode(source.getScriptCode());
			//	����:
			//		resize:
			if(targets_in_noder.getSize() == targets_in_noder.getLength())
				targets_in_noder.resize(targets_in_noder.getSize() + 1);
			//		insertion:
			//targets_in_noder.
			//	��� Ž��:
			//type_index where_new_one_was_pushed = targets_in_noder.push();
			//if(NENodeCodeSet& is_this_new_push_to = _getInnerContainer(nodeset[where_new_one_was_pushed]))
		}
		return 0;
	}

	NENodeCodeSet& WorldVisualizer::_getInnerContainer(NENode& target)
	{
		NENodeCodeSet* nullptr = 0;
		return *nullptr;
	}

	type_result WorldVisualizer::_updateKeyToGivenScriptCode(NENode& to_be_push, type_code scriptcode)
	{
		return 0;
	}
}