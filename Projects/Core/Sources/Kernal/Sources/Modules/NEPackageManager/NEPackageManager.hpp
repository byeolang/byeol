//	---------------------------------------------------------------------------------
//	Ŭ������:	NEPackageManager
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
#include "../NEPackageSet/NEPackageSet.hpp"

namespace NE
{
	class NE_DLL NEPackageManager : public NEModule
	{
		NE_DECLARE_CLASS(NEPackageManager, NEModule)

	public:
		friend class Kernal;
		friend class Editor;

		//	����Ŭ����:
	public:
#include "innerclass/ErrorCode/ErrorCode.hpp"

		//	������:
	public:
		NEPackageManager();
		NEPackageManager(const This& source);

		//	������ �����ε�:
	public:
		const This& operator=(const This& source);
		bool operator==(const This& source) const;
		bool operator!=(const This& source) const;

		//	�Ҹ���:
	public:
		virtual ~NEPackageManager();

		//	������:
	public:
		const NEPackage& find(const NEIdentifier& package_identifier) const;
		const NEPackageSet& getPackages() const;

		//	�������̽�:
		//		���:
		//			NEExportable:
	public:
		virtual type_result initialize();
		//			NEUnit:
	private:
		virtual type_result execute();
		//			NEOject:
	public:
		virtual type_result isValid() const;
		virtual void release();

		//	�����Լ�:
	private:
		type_result _initializeBuiltIns();
		const This& _assign(const This& source);
		void _release();
		type_result _fetchPackages(NEPackageList& candidates);
		type_result _enrollPackages(NEPackageList& candidates);
		type_result _removeDuplicated(NEPackageList& candidates);		
		type_result _linkPackageBinaries(NEPackageList& candidates);
		type_result _listupPackageBinaries(NEPackageList& candidates);

		type_result _bridgePackageInterface(NEPackage& to_be_fetched);
		type_result _removePackagesFailedToBridge(NEPackageList& candidates);
		type_result _ownClasses(NEPackage& package);

		//	�������:		
	private:		
		NEPackageSet _packages;
	};
}