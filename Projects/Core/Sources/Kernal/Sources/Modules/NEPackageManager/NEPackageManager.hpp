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
		NE_DECLARE_MODULE(NEPackageManager, NEModule)

	public:
		friend class Kernal;
		friend class Editor;

		//	������:
	public:
		NEPackageManager();
		NEPackageManager(const This& source);

		//	������ �����ε�:
	public:
		const This& operator=(const This& source);
		type_bool operator==(const This& source) const;
		type_bool operator!=(const This& source) const;

		//	�Ҹ���:
	public:
		virtual ~NEPackageManager();

		//	������:
	public:
		const NEPackage& findPackage(const NETString& developer, NETString& name, type_int interface_revision) const;
		const NEPackage& findPackage(const NEIdentifier& package_identifier) const;
		const NEPackageSet& getPackages() const;
		const NEClassBase& findClass(const NETString& developer, NETString& name, type_int interface_revision) const;
		const NEClassBase& findClass(const NEIdentifier& identifier) const;
		const NEClassBaseSet& getClasses() const;

		//	�������̽�:git
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
		type_result _pushBuiltIns(NEPackageList& candidates);
		const This& _assign(const This& source);
		void _release();
		type_result _fetchPackages(NEPackageList& candidates);
		type_result _ownPackages(NEPackageList& candidates);
		type_bool _isFiltered(const NEPackage& fetched, const NEPackageList& candidates) const;
		type_bool _isDuplicated(const NEPackage& fetched, const NEPackageList& candidates) const;
		type_result _linkPackages(NEPackageList& candidates);
		type_result _listupCandidatesToLink(NEPackageList& candidates);
		type_result _linkPackage(NEPackage& to_be_fetched);
		type_result _removePackagesFailedToLink(NEPackageList& candidates);
		type_result _ownClasses(NEPackage& package);
		type_result _enroll(const NEClassBase& new_class);
		type_result _pushToManaged(const NEClassBase& new_class);
		type_int _judgeIdentifierScore(const NEHeader& existing, const NEIdentifier& target) const;
		template <typename T, type_bool useHeap>
		const NEIdentifier& _findIdentifier(const NEIdentifier& target, NETArray<T, useHeap, T>& collector) const;

		//	�������:		
	private:		
		NEPackageSet _packages;
		NEClassBaseSet _classes;
	};
}