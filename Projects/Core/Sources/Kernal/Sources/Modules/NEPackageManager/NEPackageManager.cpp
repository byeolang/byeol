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



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� ���ĺ��ڷ� ����� �˻��Ѵ�.
	//	�˰���:	�ּ�å�� ����å�� ���ؼ�
	//					:	�ּ�å�̶� ����̸�, �����ڿ� ����Ƚ������ ��ġ�ϴ� �� ���ϸ�,
	//						�Ϻ��ϰ� ���� ������� �����ϴ� ���� ���Ѵ�. 
	//						�˻� �켱���� 1 ������.
	//						����å�� �˻� �켱���� 2������, �ּ�å�� �������� ������ ����
	//						�Ѵ�.
	//						������ "����̸�", "������"�� ������ ������ �ٸ� ��츦 ���Ѵ�.
	//						�밳, �̶��� ����Ƚ���� "������ ����Ƚ������ ���� ����"�̸�,
	//						target�� ȣȯ��(compatibilities)�� source�� ������ ���Ե� ��츦
	//						���Ѵ�.
	//	��������:	
	//	�޸�	:	�ĺ��ڶ�, "�̸�", "������ȣ", "������"�� ���Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEPackage NE_DLL &NEPackageManager::find(const NEIdentifier& identifier) const
	{
		//	pre:
		NEPackage* null_pointer = NE_NULL;
		//		stand by variables:
		//			Basic identifier info:
		type_int	MAX = 199999999,
					//	if specified revision/version count was set to default(means that not designated),
					//	we just get package with has the most highest value of that one.
					rev = identifier.getRevisionCount() > 0 ? identifier.getRevisionCount() : MAX,
					ver = identifier.getVersionCount() > 0 ? identifier.getVersionCount() : MAX;
		//			Fit:
		struct Fit
		{
			type_index	n;
			type_uint	revision_delta;	//	delta to user's specified one. the less the better.
			type_uint	version_delta;
		};
		Fit to_return = {NE_INDEX_ERROR, MAX, MAX};
		

		//	main:		
		//		finding:
		for(type_index n=0; n < _packages.getLength() ;n++)
		{
			const NEIdentifier& e = _packages[n];
			if( ! identifier.isKindOf(e)) continue;
			//	���� ������ ����� �߰��� ���:
			//		����Ƚ������ ��ġ�ϸ�:	All right. These are fully compatible.
			type_int	rev_delta = abs(e.getRevisionCount() - rev),
						ver_delta = abs(e.getVersionCount() - ver);
			if( ! rev_delta && ! ver_delta)
				//	no need to find anymore.
				return e;

			if(rev_delta <= to_return.revision_delta)
			{
				//	compatibled:
				if(ver_delta < to_return.version_delta)
				{
					to_return.n = n;
					to_return.revision_delta = rev_delta;
					to_return.version_delta = ver_delta;
				}
			}
		}



		//	post:	��� Ž���� �ǽ������� �ּ�å�� ã�� ���ߴ�.
		//			����å���� ���ŵ� �ε����� ����Ű�� ����� ��������.
		if(to_return.n == NE_INDEX_ERROR)
		{
			//	����å�� ã�� ���ߴٸ�:	���� ����å���� ã�� ���ߴٸ� ó���� second_fit_index�� �־�����
			//							���̸���� �������� ���̴�.		
			KERNAL_ERROR("E201011C44 : ��ġ�ϴ� ��� �˻� ����\n�־��� ����� �̸�, �����ڿ� ��ġ�ϴ� ����� �����ϴ�. ���Ŵ����� ���̸���� ��ȯ�� ���Դϴ�.\nã������ �ĺ��� :\n\t�̸� : %s\n\t������ : %s\n\tRevisionCount : %d\n\tVersionCount : %d", identifier.getName().toCharPointer(), identifier.getAuthor().toCharPointer(), identifier.getRevisionCount(), identifier.getVersionCount())
		
			return *null_pointer;
		}
		
		//		����å�� ã�Ҵٸ�:		
		const NEPackage& found = _packages[to_return.n];			
		KERNEL_WARNING(" : We was supposed to provide NEPackage for your request, but couldn't find proper one. Instead of, we just prepare the alternative one for you. Its information is next following.\n\tName\t: %s\n\tAuthor\t: %s\n\tRevisionCount : %d\n\tVersionCount : %d\n\tPath\t: %s", found.getName().toCharPointer(), found.getAuthor().toCharPointer(), found.getRevisionCount(), found.getVersionCount(), found.getPath().toCharPointer());
		
		return found;
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


	type_result NEPackageManager::_initializeBuiltIns()
	{
		if(_packages.getLength() > 0) return RESULT_SUCCESS | RESULT_ABORT_ACTION;

		//	create Kernel Package manually:
		NEPackage kernel_package;
		kernel_package....;

		return RESULT_SUCCESS;
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
		_initializeBuiltIns();
		//		��� Fetch:
		NEPackageList candidates;

		_fetchPackageBinaries(candidates);
		_fetchPackages(candidates);
		

		//	post:
		for(int n=0; n < _package.getLength() ;n++)
			_ownClasses(_package[n]);

		return RESULT_SUCCESS;
	}


	type_result NEPackageManager::_ownClasses(NEPackage& package)
	{
		NEClassManager& classer = Kernal::getInstance().getClassManager();
		NEClassBaseSet& components = package.getComponents();

		for(int n=0; n < components.getLength() ;n++)
		{
			//	NEPackage instances have prior to exist more than classes which were imported from them.
			//	So don't need to worry about assigning pointer of package directly in this code.
			components[0].setPackage(package);
			classer.enroll(package);
		}

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

		return *this;
	}


	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEPackageManager::_release()
	{
		_package.release();		
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
		for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext())
		{
			NEPackage& package = e->getValue();

			NEClassBaseList tray;
			package.getEntryPoint()(tray, package); // entrypoint�κ��� NETList�� �Ѿ�
			
			//	TODO: make these far generic. likely "Array = List;"			
			NEClassBaseSet& components = package.getComponents();
			components.create(tray);
			while(tray.getLength() > 0)
				components.push(tray[0]);
		}	
		
		
		//	post:
		_removeDuplicated(candidates);	
		return _enrollPackages(candidates);
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���������� �������� ����ؼ� ���ĵ� ��⸮��Ʈ�κ��� ����� �����Ѵ�.
	//	��������:
	//	�޸�	:	push�Ǵ� ������� ����� scriptcode�� �ΰ��Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�	
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_enrollPackages(NEPackageList& candidates)
	{	
		//	pre:
		if(	NEModuleList bucket.getLength() <= 0) return;



		//	main: candidates�� ��� ����� ���������� _packages�� �ִ´�		
		//		������ ���۰��� ����:
		//			�� 1�� ���ϴ°�:
		//				���Ŵ����� �ڱ� �ڽ��� �ּҰ��� �ֱ� ���ؼ���.
		//				�̶� ������ ����, ���ϳ��� �ν��Ͻ��� ��������� �ʰ� �ּҰ��� ���̷�Ʈ
		//				�� ���ٴ� ���̴�.
		_packages.create(candidates.getLength());

		//		candidates�κ����� ����:
		typedef NEPackageList::Iterator Iterator;
		for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext())		
		{
			NEPackage& package = e->getValue();
			_packages.push(package);	//	���ο��� Module.id, _onArgumentFetched, _onFetchModule�� ���� ȣ�� ��.

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
	type_result NEPackageManager::_removeDuplicated(NEPackageList& candidates)
	{
		//	main:
		typedef NEPackageList::Iterator Iterator;
		int source_index = 0;
		for(Iterator* source_e=candidates.getIterator(0)		; 
			source_e									;
			source_e=source_e->getNext(), source_index++)
		{		
			//	���1 ����:
			const NEIdentifier& source = source_e->getValue();

			//	���1�� ��:	���1�� ���� ����� �ִ��� Ȯ���Ѵ�
			//	also use index for iterating target:
			//		we'll delete an element of list when we found it duplicated.
			//		in order to do that, we can't use Iterator* only.
			int target_index = 0;
			for(Iterator* target_e=candidates.getIterator(source_index+1)	;
				target_e											;
				target_e=target_e->getNext(), target_index++		)
			{
				//	���2 ����:
				const NEIdentifier& target = target_e->getValue();
				if(source != target) continue;
				//	removal duplicated:
				candidates.remove(target_index); 
				
				KERNAL_WARNING("W201011C41 : ������ ��� ���� ����\n���� �ĺ��ڸ� ���� ����� 2�� �̻� �����մϴ�.\n���Ŵ����� ���߿� �ĺ��� ����� ���� ���� ���Դϴ�.\n������ ����� �ĺ���:\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d\n\tVersionCount : %d", target.getName().toCharPointer(), target.getAuthor().toCharPointer(), target.getRevisionCount(), target.getVersionCount())

				//	balance index and iterator*:
				//		for the later increasing stage of 'for' statement.
				//		we've just removed element of which indicating of target_e iterator.
				//		So because currently, target__index is designating new element's index.
				//		we should not to increase target_index + 1.	Now, when we reach to increase
				//		statement of 'for', target_index will be compensated properly.
				target_index--;	
				target_e = candidates.getIterator(target_index);	//	TODO: this consumes unnecessary O(n) searching job.
			}
		}

		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ������ �����ϴ� DLL ������ �˻��Ѵ�.
	//	��������:
	//	�޸�	:	NEFileSystem�� �˻������ ����Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_linkPackageBinaries(NEPackageList& candidates)
	{
		//	main:
		_listupPackageBinaries(candidates);
		//		����:	
		bool has_changed = false;
		do
		{
			has changed = false;
			/*
				_linkDLLsUsingInputedPath()�� �������� �����ҽÿ��� true�� ��ȯ�Ѵ�
			*/
			typedef NEPackageList::Iterator Iterator;
			for(Iterator* e=candidates.getIterator(0); e ;e=e->getNext())
			{
				NEPackage& to_be_fetched = e->getValue();

				if( ! to_be_fetched.isLoaded())
					if(_bridgePackageInterface(to_be_fetched) == RESULT_SUCCESS)
						//	�ε忡 �����ߴٸ�:	�������� �����Ѱ��� �ȴ�
						has_changed = true;

			}
		} while(has_changed);



		//	post:
		return _reportErrorsIfThereAreModulesNotFeched();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�˻��� DLL ������ ���μ����� �ø���, ��Ʈ������Ʈ�� �ּҸ� �����´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEPackageManager::_bridgePackageInterface(NEPackage& to_be_fetched)
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
	
	type_result NEPackageManager::_listupPackageBinaries(NEPackageList& candidates)
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
		const NEHeader& header = getClassStatically().getIdentifier();
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

	type_result NEPackageManager::_removePackagesFailedToBridge(NEPackageList& candidates)
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
}