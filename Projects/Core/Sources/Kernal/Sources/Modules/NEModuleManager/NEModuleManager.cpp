#include "NEModuleManager.hpp"
#include "../Kernal/Kernal.hpp"
#include "define/define.hpp"

namespace NE
{
	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEModuleManager::NEModuleManager()
		: NEModule()
	{
		_release();
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEModuleManager::NEModuleManager(const NEModuleManager& source)
		: NEModule(source)
	{
		_assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NE_DLL NEModuleManager::~NEModuleManager()
	{

	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEModuleManager NE_DLL &NEModuleManager::operator=(const NEModuleManager& source)
	{		
		NEModule::operator=(source);

		return _assign(source);
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEModuleManager::operator==(const NEModuleManager& source) const
	{
		if(this == &source) return true;
		if(NEModule::operator==(source) == false) return false;
		if(_dllheaderset != source._dllheaderset) return false;
		if(_moduleset != source._moduleset) return false;
		
		return true;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	bool NE_DLL NEModuleManager::operator!=(const NEModuleManager& source) const
	{
		return !(operator==(source));
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
	const NEModule NE_DLL &NEModuleManager::getModule(const NEExportable::Identifier& identifer) const
	{
		//	pre:
		NEModule* null_pointer = NE_NULL;


		//	main:
		type_index	second_fit_index = -1; // �̸��� ����å. ���� ����å���� ���ٸ� -1�� �����ȴ�.
		int	second_fit_revision = 0; // ����å�� ����Ƚ��

		for(type_index n=0; n < _moduleset.getLength() ;n++)
		{
			const NEExportable::ModuleHeader& header = _moduleset.getElement(n).getHeader();

			//	���� �����ڿ� �̸��� ���ٸ�:
			if(identifer.isSameModule(header))
			{
				//	���� ������ ����� �߰��� ���:
				//		����Ƚ������ ��ġ�ϸ�:
				if(header.getRevision() == identifer.getRevision())
				{
					//		�ּ�å �߰�:	���̻� �˻��� ������ ����.
					return _moduleset.getElement(n);
				}
				else //	����å ����:
				{
					//		���� �ֽ� ������� �Ǵ�:	���� �˰��ִ� ����å�� ����Ƚ������ ū��?
					if(header.getRevision() > second_fit_revision)
					{
						//		ȣȯ�� �Ǵ�:	ȣȯ�� ������ �������� Ȯ���� �ʿ伺�� �����.
						const NEIntSet& compatibilities = header.getRevisionCompatibilities();

						for(type_index iterator=0; iterator < compatibilities.getLength() ;iterator++)
						{
							//		���� Ÿ���� ȣȯ ������ ����Ƚ����, ���ڷ� �־��� ����� ����Ƚ���� ���Եȴٸ�,					
							if(identifer.getRevision() == compatibilities.getElement(iterator))
							{
								//		Ÿ���� ���� ���� ������ ���ɼ��� ũ��.
								//		����å ����:	����å�� ������ �ʿ䰡 �ִ�.
								second_fit_revision = header.getRevision();
								second_fit_index = n;
							}
						}
					}
				}
			}
		}



		//	post:	��� Ž���� �ǽ������� �ּ�å�� ã�� ���ߴ�.
		//			����å���� ���ŵ� �ε����� ����Ű�� ����� ��������.
		//		����å�� ã�Ҵٸ�:
		if(second_fit_index != -1)
		{
			KERNAL_WARNING("W201011C45 : ����� ��⸸ �˻���\n�־��� ����� �̸�, ������, ������ȣ���� ��ġ�ϴ� ����� �������ϴ�.\n�����ڿ� �̸��� ���� ������ ����, ������ ���� ���̳�, ������ȣ�� �ٸ� ����� ã�ҽ��ϴ�.\n��� �Ŵ����� �� ����� ��ȯ�� ���Դϴ�.\nã������ �ĺ��� :\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d\n��� ã�� ����� ������ȣ : %d", identifer.getName().toCharPointer(), identifer.getDeveloper().toCharPointer(), identifer.getRevision(), _moduleset.getElement(second_fit_index).getHeader().getRevision())
			return _moduleset.getElement(second_fit_index);
		}
		else//	����å�� ã�� ���ߴٸ�:	���� ����å���� ã�� ���ߴٸ� ó���� second_fit_index�� �־�����
			//							���̸���� �������� ���̴�.
		{		
			KERNAL_ERROR("E201011C44 : ��ġ�ϴ� ��� �˻� ����\n�־��� ����� �̸�, �����ڿ� ��ġ�ϴ� ����� �����ϴ�. ���Ŵ����� ���̸���� ��ȯ�� ���Դϴ�.\nã������ �ĺ��� :\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d", identifer.getName().toCharPointer(), identifer.getDeveloper().toCharPointer(), identifer.getRevision())
			return *null_pointer;
		}
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-16	������	�ۼ�
	//	---------------------------------------------------------------------------------
	const NEModuleManager::DLLHeaderList NE_DLL &NEModuleManager::getDllHeaderSet() const
	{	
		return _dllheaderset;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-16	������	�ۼ�
	//	---------------------------------------------------------------------------------
	const NEModuleSet NE_DLL &NEModuleManager::getModuleSet() const
	{	
		return _moduleset;
	}



	//	---------------------------------------------------------------------------------
	//	����	:
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	//	��� ���Ϸκ��� ����� �����´�
	type_result NE_DLL NEModuleManager::execute()
	{		
		return RESULT_SUCCESS;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEType::Type NE_DLL NEModuleManager::getType() const
	{
		return NEType::NEMODULE_MANAGER;
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	NEObject NE_DLL &NEModuleManager::clone() const
	{		
		return *(new NEModuleManager(*this));
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NE_DLL NEModuleManager::isValid() const
	{						
		type_result result = NEModule::isValid();
		if(NEResult::hasError(result)) return result | NEMODULE_IS_NOT_VALID;
		result = _dllheaderset.isValid();
		if(NEResult::hasError(result)) return result | DLL_HEADERSET_IS_NOT_VALID;
		result = _moduleset.isValid();
		if(NEResult::hasError(result)) return result | MODULESET_IS_NOT_VALID;
		
		return RESULT_SUCCESS;
	}

	type_result NE_DLL NEModuleManager::initialize()
	{
		//	pre:		
		//		�ʱ�ȭ:
		_release(); // release()�� �ϸ� �ȵȴ�.

		

		//	main:
		//		��� Fetch:
		_linkDLL();
		_linkModule();
		
		return RESULT_SUCCESS;
	}

	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NE_DLL NEModuleManager::release()
	{
		//	����Լ� �ʱ�ȭ
		NEModule::release();

		return _release();
	}		



	//	---------------------------------------------------------------------------------
	//	����	:	����� ����� ��������. ����� Ŭ������ �̹� ���ǵǾ �����ϵȴ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEExportable::ModuleHeader NE_DLL &NEModuleManager::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			//	���� �ʱ�ȭ�� �ȵǾ���
			_header.getName() = _T(_NAME );
			_header.getDeveloper() = _T(_DEVELOPER);;
			_header.setRevision(_REVISION);
			_header.getReleaseDate() = _T(_DATE);
			_header.getVersion()  = _T(_VERSION);
			_header.getComment() = _T(_COMMENT);
			_header.setMaxErrorCodeCount(NEModuleManager::END_OF_ERROR_CODE - 1);
		}

		return _header;
	}	



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-03	������	�ۼ�
	//	---------------------------------------------------------------------------------
	LPCTSTR NE_DLL NEModuleManager::getErrorMessage(type_errorcode errorcode) const
	{
		switch(errorcode)
		{
 		case NEMODULE_IS_NOT_VALID:			return _T("�߸��� ���");
 		case DLL_HEADERSET_IS_NOT_VALID:	return _T("�߸��� DLL �����");
 		case MODULESET_IS_NOT_VALID:		return _T("�߸��� ����");
		case INVALID_DLL_PATH:				return _T("DLL ��ΰ� �߸��Ǿ���");
		case DLL_PATH_IS_NULLED:			return _T("DLL ��ΰ� NULL�̴�");
		case FAILED_TO_ACCESS_DLL:			return _T("DLL�� �������� ���ߴ�");
		case FAILED_TO_ACCESS_ENTRY_POINT:	return _T("DLL���� ���������� EntryPoint�� �������� ���ߴ�");
		default:
			return _T("�˼� ���� ����");
		}
	}



	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-08-08	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	const NEModuleManager& NEModuleManager::_assign(const NEModuleManager& source)
	{
		if(this == &source) return *this;

		_dllheaderset = source._dllheaderset;
		_moduleset = source._moduleset;

		return *this;
	}


	//	---------------------------------------------------------------------------------
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_release()
	{
		_moduleset.create(1); // �ڱ��ڽ��� ��� ���ؼ� 1�� �����Ѵ�.
		_dllheaderset.release();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	����� DLL ���Ϸκ���, ���� ��Ʈ������Ʈ�� �����ؼ� ����� �����´�.
	//	��������:
	//	�޸�	:	������ ����� �������� ����ؼ� �� �����Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_linkModule()
	{
		//	���:	
		//		������:	
		//				��⿡�� �������̶�� �� �����ϴµ�,
		//				�̴� �ʱ�ȭ�ÿ�, �ٸ� ����� ���� �ʱ�ȭ�Ǿ��־� ���� �ǹ��Ѵ�.
		//	
		//				���� ���, �������� DC�� �� �����ϴ� ����� ���� ���,
		//				�� ����� �����츦 �����ϴ� ����� ���� �ʱ�ȭ�� �Ǿ�߸� �ʱ�ȭ�� ������ ���̴�.
		//				���� �Ҹ��� ��쵵, ������ ����� ���� �Ҹ�Ǿ�� �ȵȴ�.
		//
		//		������ ������:
		//				�̷��� �������� ������ �ľ�, �ʱ�ȭ�� �Ͼ�� ���� ������,
		//				��ġ�ϴ� ���� ���Ѵ�.
		//				�ʱ�ȭ�� 0������ �Ͼ�Ƿ�, �������� ���� ����� ���� ���ڸ��� ��ġ�ϰ� �ȴ�.
		//				�Ҹ��� ���ڸ����� �Ͼ�� �ȴ�.


		//	main:
		//		�ӽ� ���� ����:
		NEModuleList bucket; // ���߿� _moduleset�� ���� �Ѱ��� ����Ʈ. DLL ��Ʈ�� ����Ʈ�κ��� ���⿡ ����� ä��� �ȴ�.			
		//		���ۿ� ������ ����:
		for(type_index n=0; n < _dllheaderset.getLength() ;n++)
			bucket.push( _dllheaderset[n].getEntryPoint()() ); // entrypoint�κ��� NEListTemplate�� �Ѿ�´�.
		//		�ߺ� ���� ���͸�:
		_filterNestedName(bucket);
		//		������ ������:
		_resorting(bucket);
		//		���¿� ������ ����:	���ĵ� �ӽù��۸� �����Ѵ�
		_pushModuleSet(bucket);
	}



	//	---------------------------------------------------------------------------------
	//	����	:	���������� �������� ����ؼ� ���ĵ� ��⸮��Ʈ�κ��� ����� �����Ѵ�.
	//	��������:
	//	�޸�	:	push�Ǵ� ������� ����� scriptcode�� �ΰ��Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�	
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_pushModuleSet(NEModuleList& buffer)
	{	
		//	pre:
		if(buffer.getLength() <= 0) return;



		//	main: buffer�� ��� ����� ���������� _moduleset�� �ִ´�		
		//		������ ���۰��� ����:
		//			�� 1�� ���ϴ°�:
		//				���Ŵ����� �ڱ� �ڽ��� �ּҰ��� �ֱ� ���ؼ���.
		//				�̶� ������ ����, ���ϳ��� �ν��Ͻ��� ��������� �ʰ� �ּҰ��� ���̷�Ʈ
		//				�� ���ٴ� ���̴�.
		_moduleset.create(buffer.getLength());

		//		buffer�κ����� ����:
		for(NEModuleList::Iterator* iterator = buffer.getIterator(0); iterator != NE_NULL ;iterator = iterator->getNext())		
		{
			NEModule& target = iterator->getValue();
			_moduleset.push(target);	//	���ο��� Module.id, _onArgumentFetched, _onModuleFetched�� ���� ȣ�� ��.

			KERNAL_INFORMATION(" ��� �߰���\n����: %s", target.getHeader().getName().toCharPointer());
		}
	}



	//	---------------------------------------------------------------------------------
	//	����	:	fetch�� ����� �������� ����ؼ� �������Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_resorting(NEModuleList& source) // const�� �ƴԿ� �ָ��϶�
	{		
		//	main:
		//		������ ��� ����:	dependencies�� ���� �ִ� ����� �и���Ų��.
		//			������ ��� ���� ����:	
		NEModuleList extracted_dependency_module;

		NEModuleList::Iterator* iterator = source.getIterator(0);
		for(int n=0; n < source.getLength() ;n++)
		{
			//	���� �˻�:
			if(iterator == NE_NULL)
			{
				KERNAL_ERROR("E201011C54 : ��� ������ ���̿� ���� �������� ����ġ\n���ۿ� ��� ����� �������ϴ� ��, ���̿� �������� ����ġ�� ���ؼ�, �۾��� ������ �ϼ����� ���߽��ϴ�.")
				break;
			}

			//	������ �ľ�:
			//		��� ������:	������������ ����� �ִ�
			NEModule& target = iterator->getValue();
			const NEExportable::ModuleHeader& header = target.getHeader();
			//		����1:	�������� �ִٸ�
			if(header.getModuleDependencies().getLength() > 0)
			{
				//		����:	source�κ��� �и��ؼ� extracted�� �����Ѵ�
				//			������ ���ۿ� �߰�:
				extracted_dependency_module.push(target); // ���ο��� clone ȣ��
				//			�־��� ��Ͽ��� ����:
				source.remove(n); // iterator = iterator->getNext(); �� ���� �ʴ´�.
			}
			else
				iterator = iterator->getNext();
		} 


		//		������ ��� ������:	�и��� ����� �������� ����ؼ� ���ġ�Ѵ�.
		while(extracted_dependency_module.getLength() > 0)
		{	//	���:
			//		���� ���� Ž���� ���� ���� ����:	
			//			���� ������ ���� ���� �����ϱ� ���� ����. 
			//			����, ���� ������ ���ٸ�, ������ ����� ���� ���̰� ��ȭ ���� ���̴�.
			//		
			//		�� �̷��� �˻纯���� �δ°�:
			//			�ѹٲ� for ���� ���Ҵٰ� �ؼ�, ��� ������ ����� push�Ǵ°� �ƴϳ� �����̴�.
			//			���δ� extracted_module �߿��� ���� ����� ���� ��⿡ �������� �������� �ִ�. 
			//			�׷��� ������ ��ȭ�� ���������� ����ؼ� extracted_module�� �˻��Ѵ�.

			type_index infinite_loop_checker = extracted_dependency_module.getLength();


			//	������:
			for(type_index n=0; n < extracted_dependency_module.getLength() ;n++)
			{
				//	�ε��� ã��:	�������� �����ϴ� �ּ� �ε����� �����´�
				type_index index = _searchModuleIndexWithDependencies(extracted_dependency_module[n], source); // ������ index�� getLengthLastIndex + 1�� ��쿡�� insert ���ο��� push�� ����ȴ�.

				//	����1:	�ε����� ã�Ҵٸ�
				if(index > NE_INDEX_ERROR)
				{
					//	�������ۿ� ����:
					source.insert(index, extracted_dependency_module[n]);
					//	������ ��� ���ۿ��� ����:
					extracted_dependency_module.remove(n);						
					//	���̺���(= infinite_loop_check )�� �������̰� ��߳��� �Ǿ���.						
				}
			}

			//	����2:	���̺����� ��ȭ�� ���ٸ� (= ��� ����� �f������� ���̻� �������� �����Ҽ� ���� )
			if(extracted_dependency_module.getLength() == infinite_loop_checker)
			{	
				//	����:	�и�, ����������� �����ϳ� �˼����� ������ �� �̻� �������� ������ ���� ����									
				KERNAL_ERROR("E201011C43 : �Ϻ� ������ ����� �б� ����\n�Ϻ� ����� �������� ����Ͽ� ��ġ���� ���߽��ϴ�.\n���Ŵ����� ��ġ���� ���� ������ ������ ���Դϴ�.\n���ܵ� �������� ����� ���� : %d", extracted_dependency_module.getLength())
					break;
			}							
		}
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�־��� ����� �������� ����ؼ� searching_target�� �ּ��� ��� �ε���
	//				���ķ� �ڸ���ƾ� �ϴ��� �����Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_index NEModuleManager::_searchModuleIndexWithDependencies(NEModule& module, NEModuleList& searching_target)
	{	//	���:
		//		������ ����:
		//			NETString header::module_dependencies[]�� ���·� ����� �ִ�.
		//			�� �Լ������� �������� ������ų������ �ش� ����(= NETString )�� �ϳ��� ������̴�.
		//			�׷��� ��� ������ ������ ������� ������ ������ �ε����� �Ǵ��Ѵ�.



		//	main:
		//		��� ����:	����� ������ �������� ����ִ�
		NETStringSet dependency_checklist = module.getHeader().getModuleDependencies(); // ����� ����κ��� ����ȴ�. ���Ӽ��� ������Ű�� �ε����� ã�Եɶ����� ũ�Ⱑ �ϳ��� �پ���, �� üũ����Ʈ�� ũ�Ⱑ 0�� �Ǹ� ���������� ������ �ε����� �ȴ�.

		//		������ �ε��� �˻�:
		for(type_index index=0; index < searching_target.getLength() ;index++)
		{	//		��� �̸� ����:	�������� �ִٸ�, dependency_checklist ��򰡿� ���� ���̴�
			const NETString& target_name = searching_target[index].getHeader().getName();

			//		������ ���� �Ǵ�:
			for(type_index dependency_index=0; dependency_index < dependency_checklist.getLength() ;dependency_index++)
			{	//	����1:	�������� �ִٸ�
				if(dependency_checklist[dependency_index] == target_name)
				{	//	��ġ�ϴ� ������ ����:	������ ����������(dependency_checklist)���� ��ġ�ϴ� �������� �����Ѵ�
					dependency_checklist.remove(dependency_index);
					dependency_index--;
				}
			}

			//	����2:	���� �ε���(count)���� ��� �������� ������°�
			if(dependency_checklist.getLength() <= 0) // ��� ���Ӽ��� �����ϴ� �ּ� �ε����� ã�Ҵ�.
			{
				//	�˻�����:
				return index + 1; // index + 1���� �־���Ѵ�. index���� ������, index�ڸ��� �ִ��� index + 1�� �з����� �Ǵϱ�.
			}
		}



		//	post:
		//		�˻�����:	������ �ε����� ã�� ���ߴ�
#ifdef DEVELOPER
		const NEExportable::Identifier& identifier = module.getHeader();
		KERNAL_ERROR("E201011C42 : ����� �������� ����� ��ġ �Ǵ� ����\n�־��� ����� �������� ����Ͽ� ������ ��ġ(�ּ��� �ε���)�� �˻�������, �����߽��ϴ�.\n����� �ĺ��� :\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d", identifier.getName().toCharPointer(), identifier.getDeveloper().toCharPointer(), identifier.getRevision())
#endif
			return NE_INDEX_ERROR;
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�߰��Ϸ��� ����� �̹� �����ϴ� ����̸��� ���� �ִٸ�, ���� �ֽŹ���
	//				�� ������ �Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_filterNestedName(NEModuleList& source)
	{
		//	main:
		for(type_index n=0; n < source.getLength() ;n++)
		{		
			//	���1 ����:
			const NEExportable::ModuleHeader& source_header = source[n].getHeader();

			//	���1�� ��:	���1�� ���� ����� �ִ��� Ȯ���Ѵ�
			for(type_index target_index = n+1; target_index < source.getLength() ; target_index++)
			{
				//	���2 ����:
				const NEExportable::ModuleHeader& target_header = source[target_index].getHeader();

				//	����1:	������ ���� ����� �߰��ߴٸ�
				if(source_header == target_header)
				{	
					//	�߰� ���� �ʴ´�:
					source.remove(target_index); 
#ifdef DEVELOPER
					const NEExportable::Identifier& identifier = target_header;
					KERNAL_WARNING("W201011C41 : ������ ��� ���� ����\n���� �ĺ��ڸ� ���� ����� 2�� �̻� �����մϴ�.\n���Ŵ����� ���߿� �ĺ��� ����� ���� ���� ���Դϴ�.\n������ ����� �ĺ���:\n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d", identifier.getName().toCharPointer(), identifier.getDeveloper().toCharPointer(), identifier.getRevision())
#endif
						break;
				}
			}
		}

	}



	//	---------------------------------------------------------------------------------
	//	����	:	������ ������ �����ϴ� DLL ������ �˻��Ѵ�.
	//	��������:
	//	�޸�	:	NEFileSystem�� �˻������ ����Ѵ�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	void NEModuleManager::_linkDLL()
	{			
		//	pre:
		_pushDLLPathToDLLHeaderSet();



		//	main:
		//		�˻� �غ�:	
		/*
			�������� �����Ѵٸ� ���� load �����ߴ� DLL�� �ٽ� �ε� ������ ���� �𸥴�.
		*/
		int index = 0;
		//		����:	
		while(true)
		{
			/*
				_linkDLLsUsingInputedPath()�� �������� �����ҽÿ��� true�� ��ȯ�Ѵ�
			*/
			bool has_changed = _linkDLLsUsingInputedPath();
			if(! has_changed)
				break;
		}



		//	post:
		_reportErrorsIfThereAreModulesNotFeched();
	}



	//	---------------------------------------------------------------------------------
	//	����	:	�˻��� DLL ������ ���μ����� �ø���, ��Ʈ������Ʈ�� �ּҸ� �����´�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	type_result NEModuleManager::_pushDLLHeader(NEModuleManager::DLLHeader& header)
	{	//	���:
		//		�� push�� �ϰ� �����ڸ� �������°�:
		//			push�ϴ� �������� �Ҹ��ڿ� ��������ڰ� ȣ��Ǳ� �����̴�.
		//			Header�� �Ҹ��ڴ� FreeLibrary�� ȣ���ؼ� ������ DLL�� �Ҹ��Ų��.
		//			�̷� ������带 ���̱� ���ؼ�, �̸� ���� ����ְ� �ܺο��� �����Ѵ�


		//	pre:
		//		�Ķ���� �˻�:
		if(! &header) return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION;
		if( ! &(header.getPath())) return RESULT_TYPE_ERROR | RESULT_ABORT_ACTION | DLL_PATH_IS_NULLED;
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
		header.getInstance() = LoadLibrary(header.getPath().toCharPointer());
		/*
			�ٽ� ���� �޼����� Ȱ��ȭ �Ѵ�.
		*/
		SetErrorMode(NE_DEFAULT);

		//		����1:	DLL�� �� �о��ٸ�		
		if( ! header.getInstance())
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
		header.getEntryPoint() = reinterpret_cast<EntryPoint>(GetProcAddress(header.getInstance(), _DLL_ENTRYPOINT_STRING));

		//			����1-1:	�������� �������� ���ߴٸ�
		if( ! header.getEntryPoint())
		{	//			���� ����:	�ε�� ���������� ��Ʈ������Ʈ ��Ͽ��� ����
			KERNAL_ERROR("E201011C39 : ���� ��Ʈ������Ʈ ���� ����\nDLL Ȯ�� �� ���� DLL ���ῡ�� ���������� ���� ��Ʈ�� ����Ʈ�� �������� ���߽��ϴ�.\n�ش� ��� �����ڰ� �߸��� ��Ʈ������Ʈ�� ����ϰ� ���� ���ɼ��� �ֽ��ϴ�.\n���Ŵ����� �� DLL�� ������ ���Դϴ�.\nǥ�� ��Ʈ������Ʈ : %s\n���ϸ� : %s", _T(_DLL_ENTRYPOINT_STRING), header.getPath().toCharPointer())

			return RESULT_TYPE_ERROR | RESULT_ACCESS_DENIED;
		}

		return RESULT_SUCCESS;
	}
	
	void NEModuleManager::_pushDLLPathToDLLHeaderSet()
	{
		//	pre:
		//		Ÿ����:
		NEINIManager& ini = Kernal::getInstance().getINIManager();
		//		���� �غ�:
		NETString path = Kernal::getInstance().getSettings().getModuleDirectory() + _T("*.dll");



		//	main:
		//		�˻� �غ�:
		NEFileSystem filesystem(path.toCharPointer(), true); //	�׽�Ʈ�ϱ� �ϴ� Ǯ ��θ� �����Ѵ�. ��������� "Modules/*.dll" �� �ϸ� ��
		//		����:
		
		while(true)
		{
			filesystem.findFile();
			const NEFileSystem::TaskUnit& task = filesystem.getLastElement();
			if( ! &task) break;

			//		�б� ���� flag:
			//			���� ����:
			NETString value;
			//			INI�κ��� �� ������:
			type_result result = ini.readModuleIniString(getHeader(), task.getFilePath(), value);
			//		�ε� ���� ���� �˻�:
			//			���� ������ �߻��ߴٸ�:
			if(NEResult::hasError(result))		
				ini.writeModuleIniString(getHeader(), task.getFilePath(), NETString(true));
			else	//	������ ���ٸ�:
				//			������ ���� false(�ε� ����) �̶��:
				if(value.toBoolean() == false)
				{
					KERNAL_INFORMATION("I201011C53 : DLL ���� ���� ���\nINI�� ���ؼ� �߰��� DLL�� �ε� ���� �ʱ�� �߽��ϴ�.\n�ε��ϰ� �ʹٸ� INI�� Ű���� false���� true�� �ٲٽʽÿ�.\n��ҵ� DLL ���ϸ� : %s", task.getFilePath())

					continue;
				}

			NEModuleManager::DLLHeader header;
			header.getPath() = task.getFilePath();
			_dllheaderset.push(header);
		}
	}

	/*
		�������� �����Ѵٸ� true�� ��ȯ�Ѵ�
	*/
	bool NEModuleManager::_linkDLLsUsingInputedPath()
	{
		bool has_changed = false;

		for(int n=0; n < _dllheaderset.getLength() ;n++)
		{
			NEModuleManager::DLLHeader& header = _dllheaderset[n];

			if( ! header.isLoaded())
				if(_pushDLLHeader(header) == RESULT_SUCCESS)
					//	�ε忡 �����ߴٸ�:	�������� �����Ѱ��� �ȴ�
					has_changed = true;

		}

		return has_changed;
	}

	void NEModuleManager::_reportErrorsIfThereAreModulesNotFeched()
	{
		for(int n=0; n < _dllheaderset.getLength() ;)
		{
			NEModuleManager::DLLHeader& header = _dllheaderset[n];

			if( ! header.isLoaded())
			{
#ifdef _DEBUG
				KERNAL_ERROR("E201011C40 : DLL ���� ���� ����\nDLL�� ã������ ���� ���ῡ �����߽��ϴ�.\n�ùٸ� DLL ������ �ƴϰų�, ������ / ����� ��尡 �ٸ� ��� �� �޼����� �߻��� �� �ֽ��ϴ�.\n�ٽ� Ȯ�� �غ��ʽÿ�.\nKernal ���� ��� : Debug\n���ϸ� : %s", header.getPath().toCharPointer())
#else
				KERNAL_ERROR("E201011C40 : DLL ���� ���� ����\nDLL�� ã������ ���� ���ῡ �����߽��ϴ�.\n�ùٸ� DLL ������ �ƴϰų�, ������ / ����� ��尡 �ٸ� ��� �� �޼����� �߻��� �� �ֽ��ϴ�.\n�ٽ� Ȯ�� �غ��ʽÿ�.\nKernal ���� ��� : Release\n���ϸ� : %s", header.getPath().toCharPointer())
#endif		
				_dllheaderset.remove(n);
			}
			else
				n++;
		}
	}
}