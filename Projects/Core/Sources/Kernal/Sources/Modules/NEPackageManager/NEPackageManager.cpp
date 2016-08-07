#include "NEPackageManager.hpp"
#include "../Kernal/Kernal.hpp"
#include "define/define.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEPackageManager::NEPackageManager()
		: Super()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEPackageManager::NEPackageManager(const This& source)
		: Super(source)
	{
		_assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEPackageManager::~NEPackageManager()
	{

	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEPackageManager NE_DLL &NEPackageManager::operator=(const This& source)
	{		
		if(this == &source) return *this;

		Super::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEPackageManager::operator==(const This& source) const
	{
		if(this == &source) return true;
		if(Super::operator==(source) == false) return false;
		if(_packages != source._packages) return false;
		if(_classes != source._classes) return false;
		
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEPackageManager::operator!=(const This& source) const
	{
		return !(operator==(source));
	}

namespace
{
	inline type_uint abs(type_int raw)	
	{
		return raw < 0 : -raw : raw;
	}
}


	const NEPackage& NEPackageManager::findPackage(const NETString& developer, NETString& name, type_int interface_revision) const
	{
		return findPackage(NEIdentifier(developer, name, interface_revision));
	}

	const NEClassBase& NEPackageManager::findClass(const NETString& developer, const NETString& name, type_int interface_revision) const
	{
		return findClass(NEIdentifier(developer, name, interface_revision));
	}

	const NEPackage& NEPackageManager::findPackage(const NEIdentifier& identifier) const
	{
		return static_cast<NEPackage&>(_findIdentifier(identifier, _packages));
	}

	const NEClassBase& NEPackageManager::findClass(const NEIdentifier& identifier) const
	{
		return static_cast<NEClassBase&>(_findIdentifier(identifier, _classes));
	}

	type_int _judgeIdentifierScore(const NEHeader& existing, const NEIdentifier& target) const
	{		
		if(	existing.getDeveloper() != target.getDeveloper()	||
			existing.getName() != target.getName()				)	return 0;
		if(existing.getInterfaceRevision() == target.getInterfaceRevision()) return 3;
		if(existing.getCompatibles().find(target.getInterfaceRevision()))
			return 2;

		return 1;
	}

	template <typename T, type_bool useHeap>
	const NEIdentifier& NEPackageManager::_findIdentifier(const NEIdentifier& target, NETArray<T, useHeap>& collector) const
	{
		//	pre:
		//		Fit:
		struct Fit
		{
			NEIdentifier* founded;
			type_int score;
		};
		Fit to_return = {NE_NULL, 0};
		

		//	main:		
		//		finding:
		for(type_index n=0; n < collector.getLength() ;n++)
		{
			const NEHeader& e = collector[n];
			type_int score = _judgeIdentifierScore(e, target);
			if(score < to_return.score) continue;
			if(score == 3) return collector[n];	//	just fit case.

			//	if we met new challenger,
			if(	score > to_return.score								||
				e.getRevisionCount() > to_return.getRevisionCount()	)
			{
				to_return.founded = &e;
				to_return.score = score;
			}
		}


		//	post:	��� Ž���� �ǽ������� �ּ�å�� ã�� ���ߴ�.
		//			����å���� ���ŵ� �ε����� ����Ű�� ����� ��������.
		if( ! to_return.founded)
		{
			//	����å�� ã�� ���ߴٸ�:	���� ����å���� ã�� ���ߴٸ� ó���� second_fit_index�� �־�����
			//							���̸���� �������� ���̴�.		
			KERNAL_ERROR("E201011C44 : ��ġ�ϴ� ��� �˻� ����\n�־��� ����� �̸�, �����ڿ� ��ġ�ϴ� ����� �����ϴ�. ���Ŵ����� ���̸���� ��ȯ�� ���Դϴ�.\nã������ �ĺ��� :\n\t�̸� : %s\n\t������ : %s\n\tRevisionCount : %d\n\tVersionCount : %d", identifier.getName().toCharPointer(), identifier.getAuthor().toCharPointer(), identifier.getRevisionCount(), identifier.getVersionCount())
		
			NEIdentifier* null_pointer = NE_NULL;
			return *null_pointer;
		}		
		//		����å�� ã�Ҵٸ�:		
		KERNAL_WARNING(" : We was supposed to provide NEPackage for your request, but couldn't find proper one. Instead of, we just prepare the alternative one for you. Its information is next following.\n\tName\t: %s\n\tAuthor\t: %s\n\tRevisionCount : %d\n\tVersionCount : %d\n\tPath\t: %s", found.getName().toCharPointer(), found.getAuthor().toCharPointer(), found.getRevisionCount(), found.getVersionCount(), found.getPath().toCharPointer());
		
		return *to_return.founded;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-16	������	�ۼ�
	//	---------------------------------------------------------------------------------
	const NEPackageSet& NEPackageManager::getPackages() const
	{	
		return _packages;
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	��� ���Ϸκ��� ����� �����´�
	type_result NE_DLL NEPackageManager::execute()
	{	
		if(_packages.getLength() > 0) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

		return initialize();
	}


	type_result NEPackageManager::_pushBuiltIns(NEPackageList& candidates)
	{
		//	create Kernel Package manually:
		//NEPackage kernel_package;
		//kernel_package....;		

		return RESULT_SUCCESS; //candidates.push(builtin);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NE_DLL NEPackageManager::isValid() const
	{						
		type_result result = Super::isValid();
		if(NEResult::hasError(result)) return result | NEMODULE_IS_NOT_VALID;
		result = _packages.isValid();
		if(NEResult::hasError(result)) return result | DLL_HEADERSET_IS_NOT_VALID;
		result = _packages.isValid();
		if(NEResult::hasError(result)) return result | MODULESET_IS_NOT_VALID;
		
		return RESULT_SUCCESS;
	}

	type_result NE_DLL NEPackageManager::initialize()
	{
		//	pre:
		//		�ʱ�ȭ:
		_release(); // release()�� �ϸ� �ȵȴ�.


		//	main:
		NEPackageList candidates;
		_pushBuiltIns(candidates);
		//		��� Fetch:
		_linkPackages(candidates);
		_fetchPackages(candidates);
	
		return RESULT_SUCCESS;
	}

	const NEPackageSet& NEPackageManager::getPackages() const
	{
		return _packages;
	}

	const NEClassBaseSet& NEPackageManager::getClasses() const
	{
		return _classes;
	}

	type_result NEPackageManager::_ownClasses(NEClassBaseList& candidates, NEPackage& package)
	{
		int n=0;
		for(NEClassBaseList::Iterator* e=candidates.getIterator(0); e ;e=e->getNext(), n++)
		{
			NEClassBase& klass = e->getValue();
			//	Package instance copying doesn't make the problem:
			//		NEPackage instances have prior to exist more than classes which were imported from them.
			//		So don't need to worry about assigning pointer of package directly in this code.
			//	class enrolling call "Module.onFetched" callback:
			if(NEResult::hasError(_enroll(klass)))
			{
				NEHeader& header = klass.getHeader();
				KERNAL_WARNING(": we've met errors while enrolling fetched class, %s.%s.%d\nSorry, but we've to choose not to load this class.", header.getDeveloper().toCharPointer(), header.getName().toCharPointer, header.getInterfaceRevision());
				candidates.remove(n);
				n--;
				e=candidates.getIterator(n);
				continue;
			}
		}


		//	post:
		//		make it own:
		NEClassBaseSet& classes = package.getClasses();
		classes.create(candidates.getLength());

		for(NEClassBaseList::Iterator* e=candidates.getIterator(0); e ;e=e->getNext())
			classes.push(e->getValue());

		return RESULT_SUCCESS;
	}

	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NE_DLL NEPackageManager::release()
	{
		//	����Լ� �ʱ�ȭ
		Super::release();

		return _release();
	}		


	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-08	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEPackageManager& NEPackageManager::_assign(const This& source)
	{
		if(this == &source) return *this;

		_packages = source._packages;
		_classes = source._classes;

		return *this;
	}


	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEPackageManager::_release()
	{
		_packages.release();
		_classes.release();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	����� DLL ���Ϸκ���, ���� ��Ʈ������Ʈ�� �����ؼ� ����� �����´�.
	//	��������:
	//	�޸�	:	������ ����� �������� ����ؼ� �� �����Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_fetchPackages(NEPackageList& candidates)
	{
		//	���:	
		//		������:	
		//				��⿡�� �������̶�� �� �����ϴµ�,
		//				�̴� �ʱ�ȭ�ÿ�, �ٸ� ����� ���� �ʱ�ȭ�Ǿ��־� ���� �ǹ��Ѵ�.
		//	
		//				���� ���, �������� DC�� �� �����ϴ� ����� ���� ���,
		//				�� ����� �����츦 �����ϴ� ����� ���� �ʱ�ȭ�� �Ǿ�߸� �ʱ�ȭ�� ������ ���̴�.
		//				���� �Ҹ��� ��쵵, ������ ����� ���� �Ҹ�Ǿ�� �ȵȴ�.
		

		//	main:
		//		���ۿ� ������ ����:
		typedef NEPackageList::Iterator Iterator;
		int n=0;
		for(Iterator* e=candidates.getIterator(0), n=0; e ;e=e->getNext(), n++)
		{
			NEPackage& package = e->getValue();
			//	release:
			package.getClasses().release();
			package.NEHeader::release();
			//	enter at entrypoint:
			NEClassBaseList tray;
			if(NEResult::hasError(package.getEntryPoint()(package, tray)))
			{
				KERNAL_WARNING("error happen while loading %s module", package.getName().toCharPointer());
			}
			//	filtering:
			if(_isFiltered(package, candidates))
			{
				candidates.remove(n);
				n--;
				e = candidates.getIterator(n);
				continue;
			}
			//	make it own:
			_ownClasses(tray, package);
		}	
		
		
		//	post:
		return _ownPackages(candidates);
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���������� �������� ����ؼ� ���ĵ� ��⸮��Ʈ�κ��� ����� �����Ѵ�.
	//	��������:
	//	�޸�	:	push�Ǵ� ������� ����� scriptcode�� �ΰ��Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�	
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_ownPackages(NEPackageList& candidates)
	{	
		//	pre:
		if(	NEModuleList bucket.getLength() <= 0) return;



		//	main:
		_packages.create(candidates.getLength());

		//		candidates�κ����� ����:
		typedef NEPackageList::Iterator Iterator;
		for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext())		
		{
			NEPackage& package = e->getValue();
			_packages.push(package);

			KERNAL_INFORMATION("'%s' package was added", package.getName().toCharPointer());
		}

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�߰��Ϸ��� ����� �̹� �����ϴ� ����̸��� ���� �ִٸ�, ���� �ֽŹ���
	//				�� ������ �Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_bool NEPackageManager::_isDuplicated(const NEPackage& fetched, const NEPackageList& candidates) const
	{
		//	main:
		typedef NEPackageList::Iterator Iterator;

		int n=0;
		for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext(), n++)
		{		
			//	���1 ����:
			const NEIdentifier& source = e->getValue();
			//	���1�� ��:	���1�� ���� ����� �ִ��� Ȯ���Ѵ�
			if(source != fetched) continue;
			
			//	removal duplicated:				
			KERNAL_WARNING("W201011C41 : ������ ��� ���� ����\n���� �ĺ��ڸ� ���� ����� 2�� �̻� �����մϴ�.\n���Ŵ����� ���߿� �ĺ��� ����� ���� ���� ���Դϴ�.\n������ ����� �ĺ���:\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d\n\tVersionCount : %d", target.getName().toCharPointer(), target.getAuthor().toCharPointer(), target.getRevisionCount(), target.getVersionCount())
			return true;
		}

		return false;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ������ �����ϴ� DLL ������ �˻��Ѵ�.
	//	��������:
	//	�޸�	:	NEFileSystem�� �˻������ ����Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_linkPackages(NEPackageList& candidates)
	{
		//	main:
		_listupCandidatesToLink(candidates);
		//		����:	
		type_bool has_changed = false;
		do
		{
			has_changed = false;
			/*
				_linkDLLsUsingInputedPath()�� �������� �����ҽÿ��� true�� ��ȯ�Ѵ�
			*/
			typedef NEPackageList::Iterator Iterator;
			for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext())
			{
				NEPackage& to_be_fetched = e->getValue();

				if( ! to_be_fetched.isLoaded())
					if(_linkPackage(to_be_fetched) == RESULT_SUCCESS)
						//	�ε忡 �����ߴٸ�:	�������� �����Ѱ��� �ȴ�
						has_changed = true;

			}
		} while(has_changed);



		//	post:
		return _removePackagesFailedToLink();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�˻��� DLL ������ ���μ����� �ø���, ��Ʈ������Ʈ�� �ּҸ� �����´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_linkPackage(NEPackage& to_be_fetched)
	{	//	���:
		//		�� push�� �ϰ� �����ڸ� �������°�:
		//			push�ϴ� �������� �Ҹ��ڿ� ��������ڰ� ȣ��Ǳ� �����̴�.
		//			to_be_fetched�� �Ҹ��ڴ� FreeLibrary�� ȣ���ؼ� ������ DLL�� �Ҹ��Ų��.
		//			�̷� ������带 ���̱� ���ؼ�, �̸� ���� ����ְ� �ܺο��� �����Ѵ�


		//	pre:
		//		�Ķ���� �˻�:
		if( ! &to_be_fetched) return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		if( ! &(to_be_fetched.getPath())) return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION | DLL_PATH_IS_NULLED;
		//		Ÿ����:
		NEINIManager& ini = Kernal::getInstance().getINIManager();



		//	main:
		//		�⺻���� ���� ����:		
		//			�ν��Ͻ�:
		/*
			LoadLibrary�� ���ο��� �߻��ϴ� ���� �޼����ڽ��� ��� ���� ���ؼ�, 
			SetErrorMode WINAPI�� ����Ѵ�
		*/
		SetErrorMode(SEM_FAILCRITICALERRORS);
		to_be_fetched.setInstance(LoadLibrary(to_be_fetched.getPath().toCharPointer()));
		/*
			�ٽ� ���� �޼����� Ȱ��ȭ �Ѵ�.
		*/
		SetErrorMode(NE_DEFAULT);

		//		����1:	DLL�� �� �о��ٸ�		
		if( ! to_be_fetched.getInstance())
			/*
				������ ������ ������ ���ɼ��� �ִ�. �ϴ� ������ �ǹ��ϴ� FAILED_TO_ACCESS_DLL�� �ܺο� ��ȯ�Ѵ�.
				�ܺο����� FAILED_TO_ACCESS_DLL�� �߽߰�, ���ۿ� ���ܳ��� �ٸ� DLL�� try �Ұ��̴�.
				���Ŵ����� ���������� ������ ���� ��Ȳ��, DLL �ε� sequence�� �ߴ��ϰ� �ȴ�.

					1. ��� DLL�� �ε� �Ϸ�(������ ���̰� 0)
					2. ���� ���۸� 0���� ���������� try������, ������ ���� ��� fail��
			*/
			return RESULT_TYPE_ERROR | RESULT_ACCESS_DENIED | FAILED_TO_ACCESS_DLL;

		//		�ε� ����:
		//			�������� ������:
		to_be_fetched.setEntryPoint(reinterpret_cast<EntryPoint>(GetProcAddress(to_be_fetched.getInstance(), _DLL_ENTRYPOINT_STRING)));

		//			����1-1:	�������� �������� ���ߴٸ�
		if( ! to_be_fetched.getEntryPoint())
		{	//			���� ����:	�ε�� ���������� ��Ʈ������Ʈ ��Ͽ��� ����
			KERNAL_ERROR("E201011C39 : ���� ��Ʈ������Ʈ ���� ����\nDLL Ȯ�� �� ���� DLL ���ῡ�� ���������� ���� ��Ʈ�� ����Ʈ�� �������� ���߽��ϴ�.\n�ش� ��� �����ڰ� �߸��� ��Ʈ������Ʈ�� ����ϰ� ���� ���ɼ��� �ֽ��ϴ�.\n���Ŵ����� �� DLL�� ������ ���Դϴ�.\nǥ�� ��Ʈ������Ʈ : %s\n���ϸ� : %s", _T(_DLL_ENTRYPOINT_STRING), to_be_fetched.getPath().toCharPointer())

			return RESULT_TYPE_ERROR | RESULT_ACCESS_DENIED;
		}

		return RESULT_SUCCESS;
	}
	
	type_result NEPackageManager::_listupCandidatesToExamine(NEPackageList& candidates)
	{
		//	pre:
		//		Ÿ����:
		NEINIManager& ini = Kernal::getInstance().getINIManager();
		//		���� �غ�:
		NETString path = Kernal::getInstance().getSettings().getPackageDirectory() + _T("*.dll");



		//	main:
		//		�˻� �غ�:
		NEFileSystem filesystem(path.toCharPointer(), true); //	�׽�Ʈ�ϱ� �ϴ� Ǯ ��θ� �����Ѵ�. ��������� "Modules/*.dll" �� �ϸ� ��
		//		����:
		const NEHeader& header = getClassStatically().getHeader();
		while(true)
		{
			filesystem.findFile();
			const NEFileSystem::TaskUnit& task = filesystem.getLastElement();
			if( ! &task) break;

			//		�б� ���� flag:
			//			���� ����:
			NETString value;
			//			INI�κ��� �� ������:
			type_result result = ini.readModuleIniString(header, task.getFilePath(), value);
			//		�ε� ���� ���� �˻�:
			//			���� ������ �߻��ߴٸ�:
			if(NEResult::hasError(result))		
				ini.writeModuleIniString(header, task.getFilePath(), NETString(true));
			else	//	������ ���ٸ�:
				//			������ ���� false(�ε� ����) �̶��:
				if( ! value.toBoolean())
				{
					KERNAL_INFORMATION("I201011C53 : DLL ���� ���� ���\nINI�� ���ؼ� �߰��� DLL�� �ε� ���� �ʱ�� �߽��ϴ�.\n�ε��ϰ� �ʹٸ� INI�� Ű���� false���� true�� �ٲٽʽÿ�.\n��ҵ� DLL ���ϸ� : %s", task.getFilePath())

					continue;
				}

			NEPackage new_one;
			new_one.getPath() = task.getFilePath();
			candidates.push(new_one);
		}

		return RESULT_SUCCESS;
	}

	type_result NEPackageManager::_removePackagesFailedToLink(NEPackageList& candidates)
	{
		typedef NEPackageList::Iterator Iterator;

		type_index n=0;
		for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext(), n++)
		{
			NEPackage& package = candidates[n];

			if(package.isLoaded()) continue;			
				
			KERNAL_ERROR("E201011C40 : DLL ���� ���� ����\nDLL�� ã������ ���� ���ῡ �����߽��ϴ�.\n�ùٸ� DLL ������ �ƴϰų�, ������ / ����� ��尡 �ٸ� ��� �� �޼����� �߻��� �� �ֽ��ϴ�.\n�ٽ� Ȯ�� �غ��ʽÿ�.\nKernal ���� ��� : Debug\n���ϸ� : %s", package.getPath().toCharPointer())

			candidates.remove(n);

			//	balance index and iterator*:
			e=candidates.getIterator(n);
			n--;
		}

		return RESULT_SUCCESS;
	}

	type_bool NEPackageManager::_isFiltered(const NEPackage& fetched, const NEPackageList& candidates) const
	{
		//	when we have more requirements to filter, it'll be added at here.
		return _isDuplicated(fetched, candidates);
	}

	type_result NEClassManager::_enroll(const NEClassBase& new_class)
	{
		//	pre:
		//		Acquire static instance:
		static NETClass<NEAdam> root;
		//		exception handlings:
		if( ! &new_class) return KERNAL_WARNING("...");
		if(new_class.isRegistered()) return RESULT_SUCCESS | RESULT_ABORT_ACTION;


		//	main:
		//		enroll parent:
		const NEClassBase& super = new_class.getSuperClass();
		if( ! &super)
			//	if new_class is a NEAdam class, its returend 'super' is a instance of Null.
			return RESULT_SUCCESS;
		//		hee- ha! lets do this reculsively.
		_enroll(super);

		//		enrolls:
		//			supers: we can call this because Superclasses are all enrolled.
		new_class._enrollSuperClasses(super);	//	in this handler, each super classes receive sub classes.


		//	post:
		super._enrollChildClass(new_class);
		return _pushToManaged(new_class);
	}

	type_result NEPackageManager::_pushToManaged(const NEClassBase& new_class)
	{
		//	pre:
		//		resize:
		if(_classes.getLength() >= _classes.getSize())
			_classes.resize(_classes.getSize() * 2);


		//	main:
		type_index n = _classes.push(new_class);
		if (n == NE_INDEX_ERROR)
			return KERNAL_ERROR("...")


			//	post:
			//		give _class id as pushed index:
			//			there is no instance changing after inserted.
			//			(except for whole initializing)
			//			so, when new_class take new class id, it's ne
			//			ver get changed.
			new_class._getId() = n;
		return RESULT_SUCCESS;
	}
}