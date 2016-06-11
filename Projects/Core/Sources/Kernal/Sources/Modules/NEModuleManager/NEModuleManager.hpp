//	---------------------------------------------------------------------------------
//	Ŭ������:	NEModuleManager
//	����	:	����� �����ϴ� ������ Ŭ����.
//	����	:	���Ŭ����.	NEModule
//	Ư��	:	��忣���� �⺻������ ���ԵǴ� ���Ŭ������.
//	�˰���:	NEFileSystem�� �˻� ����� ����ؼ� ���(*.dll)�� ã��, ���ο��� �̸� 
//				������ ��Ʈ������Ʈ�� ����� �����´�.
//				�� ��, ���Ӽ��� ����ؼ� ����� ���ġ�Ѵ�.
//				���� ����� �ܺο��� ���� �� �� �ֵ��� �������̽��� �����Ѵ�.
//	�����:	getLength()	:	����� ����
//				getModule(type_id)
//					:	����� ���̵� ���ؼ� �����Ѵ�. �������� �ʴ´�. ����� ���̵�
//						(fetch �Ǵ� ����)�� ȯ�濡 ���� �޶����� �����̴�.
//						�׺��ٴ� ����� ���ؼ� ����� �������� ����� �����Ѵ�.
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NENode/NENode.hpp"
#include "../NEModuleSet/NEModuleSet.hpp"
#include "../NEModuleList/NEModuleList.hpp" // innerclass���� ����ϱ� ���ؼ�

namespace NE
{
	class NE_DLL NEModuleManager : public NEModule
	{
		NE_DECLARE_CLASS(NEModuleManager, NEModule)

	public:
		friend class Kernal;

		//	����Ŭ����:
	public:
#include "innerclass/DLLHeader/DLLHeader.hpp"
#include "innerclass/ErrorCode/ErrorCode.hpp"

		//	typedef:
		typedef class NE_DLL NETList<This::DLLHeader> DLLHeaderList;

		//	������:
	public:
		NEModuleManager();
		NEModuleManager(const This& source);

		//	������ �����ε�:
	public:
		const This& operator=(const This& source);
		bool operator==(const This& source) const;
		bool operator!=(const This& source) const;

		//	�Ҹ���:
	public:
		virtual ~NEModuleManager();

		//	������:
	public:
		const NEModule& getModule(const NEExportable::Identifier& identifer) const;
		const DLLHeaderList& getDllHeaderSet() const;
		const NEModuleSet& getModuleSet() const;
		
		//	�������̽�:
		//		���:
		//			NEExportable:
	public:
		virtual type_result initialize();
		virtual const NEExportable::ModuleHeader& getHeader() const;
		virtual LPCTSTR getErrorMessage(type_errorcode errorcode) const;
		//			NEUnit:
	private:
		virtual type_result execute();
		//			NEOject:
	public:
		virtual type_result isValid() const;
		virtual void release();

		//	�����Լ�:
	private:
		const This& _assign(const This& source);
		void _release();
		void _linkModule();
		void _pushModuleSet(NEModuleList& buffer);
		void _resorting(NEModuleList& source);
		type_index _searchModuleIndexWithDependencies(NEModule& module, NEModuleList& searching_target);
		void _filterNestedName(NEModuleList& source);
		void _linkDLL();
		void _pushDLLPathToDLLHeaderSet();
		bool _linkDLLsUsingInputedPath();
		type_result _pushDLLHeader(This::DLLHeader& header);
		void _reportErrorsIfThereAreModulesNotFeched();

		//	�������:		
	protected:		
		DLLHeaderList _dllheaderset;
		NEModuleSet _moduleset;
	};

	template 
	class NE_DLL NETList<NEModuleManager::DLLHeader, false>;
}