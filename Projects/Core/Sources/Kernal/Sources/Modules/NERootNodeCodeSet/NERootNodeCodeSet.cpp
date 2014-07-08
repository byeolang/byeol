#include "NERootNodeCodeSet.hpp"
#include "../Kernal/Kernal.hpp"
#include "../NEIndexedNodeSet/NEIndexedNodeSet.hpp"

namespace NE
{
	NERootNodeCodeSet::NERootNodeCodeSet(NEEnlistableManager& manager)
		: SuperClass()
	{
		_manager = &manager;
		//	enlist:
		//		�� enlist�� true�� �ϴ°�:
		//			�����ڿ��� manager�κ��� �ּҸ� �Ѱܹ��� ���� RootNodeCodeSet
		//			���� �ٷ� �� �ִ� ������ ����� enlist �̱� �����̴�.
		//			���� NERootNodeCodeSet�� NodeCodeSet���ν� ���Ŀ� enlist �Ѵٰ�
		//			�Ѵٸ�, �װ��� NENodeCodeSet���ν� enlist�� ó���ϰ� �ɰ��̴�.
		_is_enlisted = true;
	}
	NERootNodeCodeSet::NERootNodeCodeSet(type_index size)
		: SuperClass(size)
	{
		
	}
	NERootNodeCodeSet::NERootNodeCodeSet(const NERootNodeCodeSet& source)
		: SuperClass(source)
	{
		
	}
	NERootNodeCodeSet::~NERootNodeCodeSet()
	{
		
	}

	const NERootNodeCodeSet& NERootNodeCodeSet::operator+=(const NERootNodeCodeSet& source)
	{
		if(this == &source)
		{
			NERootNodeCodeSet copyied(*this);
			resize(_length + copyied._length);
			push(copyied);
		}
		else 
		{
			resize(_length + source._length);
			push(source);
		}

		return *this;
	} 

	NERootNodeCodeSet NERootNodeCodeSet::operator+(const NERootNodeCodeSet& source) const
	{
		ThisClass buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}
	NEObject& NERootNodeCodeSet::clone() const
	{
		return *(new ThisClass(*this));
	}
	NEType::Type NERootNodeCodeSet::getType() const
	{
		return NEType::NEROOT_NODE_CODESET;
	}

	type_result NERootNodeCodeSet::_onEnlisted(NENode& node)
	{
		if( ! &getManager()) return RESULT_TYPE_ERROR;
		if( ! &node) return RESULT_TYPE_ERROR;

		node._is_rootnode = true;
		return SuperClass::_onEnlisted(node);
	}
}