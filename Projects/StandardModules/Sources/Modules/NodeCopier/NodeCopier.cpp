#include "NodeCopier.hpp"

namespace NE
{
	NE_DLL NodeCopier::NodeCopier()
		: NEModule()
	{

	}

	NE_DLL NodeCopier::NodeCopier(const NodeCopier& source)
		: NEModule(source)
	{

	}

	NE_DLL NodeCopier::~NodeCopier()
	{

	}

	type_result NE_DLL NodeCopier::initialize()
	{
		getKeySet().create(5);
		getKeySet().push(NENodeSelector());	//	������ ���
		getKeySet().push(NEKeySelector());	//	����� NodeSetKey
		getKeySet().push(NEUByteKey());		//	����� ����� �ڵ��� ����
		getKeySet().push(NEIntKey());		//	����� ����� �ڵ��� ��
		getKeySet().push(NEIntKey(-1));		//	NodeSetKey�� ���� index. -1�Ͻ� push

		return RESULT_SUCCESS;
	}

	type_result NE_DLL NodeCopier::execute()
	{
		//	pre:
		type_index given_index = getIndexToInsert();

		while(NENode* nitr = &getWhoSelector().getNode())
		{
			while(NEKey* kitr = &getWhereSelector().getKey())
			{
				if( ! kitr->isSubClassOf(NEType::NENODE_CODESET_KEY))
					break;

				NENodeCodeSet& ns = static_cast<NENodeCodeSetKey&>(*kitr).getValue();
				if(ns.getLength() == ns.getSize())
					ns.resize(ns.getLength() + 1);

				type_index index = (given_index == -1) ? ns.getLength() : given_index;
				ns.insert(index, *nitr);

				NENode& inserted = ns[index];
				type_code code = getCodeValue();
				switch(NECodeType::CodeType(getCodeType()))
				{
				case NECodeType::NAME:		inserted.setNameCode(code);	break;
				case NECodeType::GROUP:
					{
						NEIntSet cs = inserted.getGroupCode();
						if( ! cs.find(code))
						{
							if(cs.getLength() == cs.getSize())
								cs.resize(cs.getLength() + 1);
							cs.push(code);
							inserted.setGroupCode(cs);
						}					
					}					
					break;
				case NECodeType::PRIORITY:	inserted.setPriority(code);	break;
				}
			}
		}

		return RESULT_SUCCESS;
	}

	NEObject NE_DLL &NodeCopier::clone() const
	{
		return *(new NodeCopier(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &NodeCopier::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("NodeCopier");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��. ������ ���\n1��. ������ ���(NodeCodeSetKey�� ����)\n2��. ����� ������ �ڵ�����(2:NAME, 3:GROUP, 4:PRIORITY, �׿�: �������)\n3��. ������ �ڵ� ��\n4��. ������ �ε���(-1�϶� push�� ����)");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-06");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(0);
		}

		return _header;
	}
}
