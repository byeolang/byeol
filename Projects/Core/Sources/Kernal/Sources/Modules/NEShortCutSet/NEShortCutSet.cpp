#include "NEShortCutSet.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	NE_DLL NEShortCutSet::NEShortCutSet()
		: SuperClass()
	{

	}

	NE_DLL NEShortCutSet::NEShortCutSet(const NECodeType& type)
		: SuperClass(), _type(type)
	{

	}

	NE_DLL NEShortCutSet::NEShortCutSet(const ThisClass& source)
		: SuperClass(source), _type(source._type)
	{

	}

	NE_DLL NEShortCutSet::~NEShortCutSet()
	{

	}

	NEShortCutSet NE_DLL &NEShortCutSet::operator=(const ThisClass& source)
	{
		if(this == &source) return *this;

		SuperClass::operator=(source);

		_type = source._type;

		return *this;
	}

	bool NE_DLL NEShortCutSet::operator==(const ThisClass& source) const
	{
		if(SuperClass::operator!=(source)) return false;

		return _type == source._type;
	}

	bool NE_DLL NEShortCutSet::operator!=(const ThisClass& source) const
	{
		return ! operator==(source);
	}

	const NECodeType NE_DLL &NEShortCutSet::getCodeType() const
	{
		return _type;
	}

	NEObject NE_DLL &NEShortCutSet::clone() const
	{
		return *(new ThisClass(*this));
	}

	void NE_DLL NEShortCutSet::release()
	{
		SuperClass::release();

		_type.release();
	}

	NEType::Type NE_DLL NEShortCutSet::getType() const
	{
		return NEType::NESHORTCUTSET;
	}

	NEBinaryFileSaver NE_DLL &NEShortCutSet::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);

		return saver << _type;
	}

	NEBinaryFileLoader NE_DLL &NEShortCutSet::serialize(NEBinaryFileLoader& loader)
	{
		SuperClass::serialize(loader);

		return loader >> _type;
	}

	type_result NE_DLL NEShortCutSet::_enlist(NENode& node, type_index real_index)
	{
		const NECodeSet& cs = node.getCodes(getCodeType());
		if( ! &cs)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}


		type_result result = RESULT_SUCCESS;
		for(int n=0; n < cs.getLength(); n++)
			result |= _enlist(node, cs[n], real_index);

		return result;
	}

	type_result NEShortCutSet::_enlist(NENode& node, const NECode& code, type_index real_index)
	{
		if(	code <= NE_HAS_NO_SCRIPTCODE||
			code > getLengthLastIndex()	)
		{
			KERNAL_ERROR(" : ��ũ��Ʈ �ڵ尡 ������ �Ѿ���ϴ�.\n\t�ڵ�: %d", code);
			return RESULT_TYPE_ERROR;
		}
		if(real_index < 0)
		{
			KERNAL_ERROR(" : ���� ����� �ε����� �߸��Ǿ����ϴ�.\n\t���� ��� �ε���: %d", real_index);
			return RESULT_TYPE_ERROR;
		}
		if(code.getCodeType() != _type.getCodeType())
		{
			KERNAL_WARNING(" : ")
			return RESULT_TYPE_WARNING;
		}
		NEShortCut& shortcut = getElement(code.getCode());
		type_index index = shortcut.find(real_index);
		if(index != NE_INDEX_ERROR)
		{
			KERNAL_INFORMATION(" : �̹� enlist�� �Ǿ�����?\n�־��� ���ƿ��� ����� �ε����� ã�ҽ��ϴ�.\n\t�ε���: %d", index);
			return RESULT_SUCCESS | RESULT_ABORT_ACTION;
		}


		//	main:		
		return shortcut.push(real_index);
	}

	type_result NE_DLL NEShortCutSet::_unlist(NENode& node, type_index real_index)
	{
		const NECodeSet& cs = node.getCodes(getCodeType());
		if( ! &cs)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}


		type_result result = RESULT_SUCCESS;
		for(int n=0; n < cs.getLength(); n++)
			result |= _unlist(node, cs[n], real_index);

		return result;
	}

	type_result NEShortCutSet::_unlist(NENode& node, const NECode& code, type_index real_index)
	{
		if(	code <= NE_HAS_NO_SCRIPTCODE||
			code > getLengthLastIndex()	)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}
		NEShortCut& shortcut = getElement(code.getCode());
		type_index index = shortcut.find(real_index);
		if(index <= NE_INDEX_ERROR)
		{
			KERNAL_ERROR(" : ");
			return RESULT_TYPE_ERROR;
		}


		return shortcut.remove(real_index);
	}

}