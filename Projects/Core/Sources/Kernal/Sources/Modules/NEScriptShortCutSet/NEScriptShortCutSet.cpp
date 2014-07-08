#include "NEScriptShortCutSet.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	NEScriptShortCutSet::NEScriptShortCutSet()
		: SuperClass()
	{

	}
	NEScriptShortCutSet::NEScriptShortCutSet(type_index size)
		: SuperClass(size)
	{

	}
	NEScriptShortCutSet::NEScriptShortCutSet(const ThisClass& source)
		: SuperClass(source)
	{

	}

	const NEScriptShortCutSet& NEScriptShortCutSet::operator+=(const ThisClass& source)
	{
		SuperClass::operator+=(source);

		return *this;
	}
	NEScriptShortCutSet NEScriptShortCutSet::operator+(const ThisClass& source) const
	{
		ThisClass buffer(getLength() + source.getLength());
		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}
	
	type_result NEScriptShortCutSet::_enlist(NENode& node, type_index node_real_index)
	{
		type_code scriptcode = node.getScriptCode();
		if(	scriptcode <= NE_HAS_NO_SCRIPTCODE	||
			scriptcode > getLengthLastIndex()	)
		{
			KERNAL_ERROR(" : ��ũ��Ʈ �ڵ尡 ������ �Ѿ���ϴ�.\n\t�ڵ�: %d", scriptcode);
			return RESULT_TYPE_ERROR;
		}
		if(node_real_index < 0)
		{
			KERNAL_ERROR(" : ���� ����� �ε����� �߸��Ǿ����ϴ�.\n\t���� ��� �ε���: %d", node_real_index);
			return RESULT_TYPE_ERROR;
		}
		NEShortCut& shortcut = getElement(scriptcode);
		type_index index = shortcut.find(node_real_index);
		if(index != NE_INDEX_ERROR)
		{
			KERNAL_INFORMATION(" : �̹� enlist�� �Ǿ�����?\n�־��� ���ƿ��� ����� �ε����� ã�ҽ��ϴ�.\n\t�ε���: %d", index);
			return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		}	



		//	main:		
		return shortcut.push(node_real_index);
	}
	type_result NEScriptShortCutSet::_unlist(NENode& node, type_index node_real_index)
	{
		type_code scriptcode = node.getScriptCode();
		if(	scriptcode <= NE_HAS_NO_SCRIPTCODE	||
			scriptcode > getLengthLastIndex()	)
		{

			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}
		NEShortCut& shortcut = getElement(scriptcode);
		type_index index = shortcut.find(node_real_index);
		if(index <= NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}


			
		return shortcut.remove(index);
	}
}