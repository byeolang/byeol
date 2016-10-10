#include "NEModuleArray.hpp"
#include "../Kernal/Kernal.hpp"
#include "../NENodeCodeSet/NENodeCodeSet.hpp"

namespace NE
{
	type_id NEModuleArray::_last_generated_id = 0;

	NEModuleArray::NEModuleArray(NEEnlistableManager& manager)
		: Super(manager)
	{

	}

	NEModuleArray::NEModuleArray(type_count size)
		: Super(size)
	{

	}

	NEModuleArray::NEModuleArray(const This& source)
		: Super(source)
	{

	}

	NEModuleArray::~NEModuleArray()
	{

	}

	const NEModuleArray& NEModuleArray::operator+=(const This& source)
	{
		Super::operator+=(source);

		return *this;
	}

	NEModuleArray NEModuleArray::operator+(const This& source) const
	{
		This buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	type_id NEModuleArray::peekLastGeneratedId() const
	{
		return _last_generated_id;
	}

	type_id NEModuleArray::_generateId()
	{
		_last_generated_id++;
		if(_last_generated_id <= 0)
			KERNAL_ERROR(" : ");

		return _last_generated_id;
	}

	type_index NEModuleArray::insert(type_index index, const NEModule* const source)
	{
		if( ! source)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}
		if(_occupiedset[index])
		{
			type_result result = setElement(index, source);
			if(NEResult::hasError(result))
			{
				KERNAL_ERROR(" : ");
				return result;
			}
		}


		
		//	main:
		
		//		���� manager ��:		
		//			Ÿ����:
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		//			����:
		NEGlobalManagerOffer::_setGlobalManager(getManager());
		//		����:
		type_index inputed_index = Super::insert(index, source);
		//		���̵� �Ҵ�:
		//			Ÿ����:
		NEModule& module = getElement(inputed_index);
		module.initialize();
		if(&module)
			module._id = _generateId();	
		//		�ǵ�����:
		NEGlobalManagerOffer::_setGlobalManager(push);	
		
		return inputed_index;
	}

	type_result NEModuleArray::setElement(type_index index, const NEModule* const source)
	{
		if( ! source)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		NEModule* const_casted = const_cast<NEModule*>(source);

		type_id original_id = const_casted->getId();
		const_casted->_id = _generateId();

		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_result result = Super::setElement(index, source);

		NEGlobalManagerOffer::_setGlobalManager(push);

		const_casted->_id = original_id;

		return result;
	}


	//	---------------------------------------------------------------------------------
	//	����:		����� ���Ϸκ��� �����Ѵ�.
	//				PC�� ȯ���� �ٲ�� ����� �������� �ʰ� �ǰ�, �̶� �������� �ʴ� �����
	//				��������� ���Ϸκ��� �о���̴� �� �Ұ����ϴ�.
	//				����, �������� �ʴ� ����� �޸� ũ�⸦ �˱⵵ �����.
	//				�׷��� Case By Skip Load�� ����Ѵ�.
	//	��������:	
	//	�޸�:		Case By Skip Load��?
	//					:	����� �ε��ϱ� ����, "����� �������� ���� ���, �̵��� �ּ�"��
	//						����Ѵ�. �׷��� ����� �������� �ʴ´ٸ� ������ �����͸� �ٷ�
	//						�������� �̵��ϴ� ����̴�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-07-10	������	�߰�
	//					:	Case By Skip Load�� �߰��ؼ� ����� ���� �������� ȯ���� �������.
	//	---------------------------------------------------------------------------------
	NEBinaryFileSaver& NEModuleArray::serialize(NEBinaryFileSaver& saver) const
	{
		Super::serialize(saver);



		//	main:
		for(int n=0; n < getSize() ;n++)
		{
			const NEModule& module = getElement(n);
			if( ! &module)
				continue;
			//		��ŵ �ּҸ� ����� �ּ� ȹ��:	
			//			��ŵ�ּҴ� �ش� ����� �ε� �Ǳ����� ��ϵǾ���Ѵ�.
			//			������, ��ŵ�ּҸ� ����Ϸ��� ���� ����� ����Ǵ� ������
			//			�˾ƾ� �ϹǷ� ����� ����Ǳ� ���� ����� �� ����.
			//			�׷���, �ϴ� �ּҰ� ��ϵ� ������ ����ΰ�, ����� ����Ǹ�
			//			�ٽ� ������ ���ƿͼ� ��ŵ�ּҸ� ����Ѵ�.
			//
			//			���� �ּ� ȹ��:
			long int place_for_skip_address = saver.getPosition(); // place_for_skip_address�� ����Ű�� ���� ��������� ��ϵǴ� ��ġ�� ����� �Ѵ�.
			//			�ּҸ� �����:	���߿� ����� �� �ֵ���, ������ �ϴ� �����Ѵ�.
			saver.setPosition(place_for_skip_address + sizeof(place_for_skip_address)); // ���� ����� ��ϵǴ� ��ġ = ���� ��ġ + �ּ��� ũ��
			//		��� ���:
			//			����� �ĺ��� ���:	�ε��, �� �ĺ��ڸ� �̿��ؼ� ����� �ν��Ͻ��� ������ ���̴�.
			module.getHeader().NEIdentifier::serialize(saver);
			
			saver << module;
			//		��ŵ�ּ� ���:	���� ������������ ��ġ�� place_for_skip_address�� ����ؾ��ϴ� �ּҴ�.
			//						���ư��� ���� ������ ��ġ�� �������.
			//			�ּ� ȹ��:
			long int skip_address = saver.getPosition(); // �� �ּҸ� place_for_skip_address�� ����Ű�� ���� �����ϸ� �ȴ�.
			//			ȸ��:	���� �ּҷ� ���ư��� ��ŵ�ּҸ� ����Ѵ�.
			saver.setPosition(place_for_skip_address);
			saver << skip_address;
			//			����:	�ٽ� ������� ���ƿ´�.
			saver.setPosition(skip_address);
		}

		return saver << _last_generated_id;
	}



	//	---------------------------------------------------------------------------------
	//	����:		����� ���Ϸκ��� �ε��Ѵ�.
	//				PC�� ȯ���� �ٲ�� ����� �������� �ʰ� �ǰ�, �̶� �������� �ʴ� �����
	//				��������� ���Ϸκ��� �о���̴� �� �Ұ����ϴ�.
	//				����, �������� �ʴ� ����� �޸� ũ�⸦ �˱⵵ �����.
	//				�׷��� Case By Skip Load�� ����Ѵ�.
	//	��������:	�̹� ������ �ν��Ͻ��� �ܺηκ��� �߻��߾�� �Ѵ�.
	//					1. ��ũ��Ʈ ������ ���	:	NENode::serialize�κ��� �ν��Ͻ� �߻�
	//					2. ��� ������ ���		:	NENodeSet::serialize�κ��� �ν��Ͻ� �߻�
	//	�޸�:		Case By Skip Load��?
	//					:	����� �ε��ϱ� ����, "����� �������� ���� ���, �̵��� �ּ�"��
	//						����Ѵ�. �׷��� ����� �������� �ʴ´ٸ� ������ �����͸� �ٷ�
	//						�������� �̵��ϴ� ����̴�.
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//				2011-07-10	������	�߰�
	//					:	Case By Skip Load�� �߰��ؼ� ����� ���� �������� ȯ���� �������.
	//	---------------------------------------------------------------------------------
	NEBinaryFileLoader& NEModuleArray::serialize(NEBinaryFileLoader& loader)
	{
		//	TEST: �ƹ��� ��ġ�� ������ �ʰ� insert, remove�� ���� enlist, unlist�� 
		//			serialize ���߿� ������ enlist, unlist�� ���۵Ǵ���?
		//	pre:
		//		Ÿ����:
		const NEPackageManager& moduler = Kernal::getInstance().getModuleManager();
		//		���� �Լ� ȣ��:
		Super::serialize(loader);



		//	main:
		type_index size = getSize();
		//		���� ���� �ʱ�ȭ:	push�� ����ϱ� ���ؼ�
		_length = 0;
		//		Occupied Ž�� �ε��� �ʱ�ȭ:
		//			occupiedSet���� index�κ��� ������� �ø��������� �˻��Ѵ�.
		//			���� ����� ���߿� OccupiedSet[index] == true�� ���� ����� �߰��ؾ��ϴ� 
		//			�κ��� �ȴ�.
		type_index occupied_index = -1;

		for(int n=0; n < size ;n++)
		{
			if( ! _occupiedset[n])
				continue;
			//		��ŵ �ּ� ȹ��:	���� ����� ���� PC�� �������� �ʾҴٸ� ���� ����� �װ�,
			//						���� ����̶�� ��ŵ�ּҷ� �ٷ� �̵��Ѵ�.
			long int next_module_to_skip = 0;
			loader >> next_module_to_skip;
			//		�ε尡���� ��� �Ǵ�:	�ĺ��ڸ� �ε��ؼ� ����, �������� �ʴ´ٸ�, �ε����� ���ϴ� 
			//								����̶�� �� �ȴ�.
			//								��, ��ũ��Ʈ ���Ͽ� �ش� ����� �����Ͱ� �����ϳ�, ���� PC����
			//								����� �������� �ʴ� ����.
			//								����, ���������͸� ��ŵ�ؾ��Ѵ�.
			//			�ĺ��� �ε�:
			NEIdentifier identifier;
			loader >> identifier;
			//			�ĺ��ڷ� �ν��Ͻ� fetch:	���� ���̸���� ������, fetch�� ���� �ߴٴ� �� ���Ѵ�.
			const NEModule& module = moduler.getModule(identifier);
			//			���� ���̸���̶��:
		
			if( ! &module							/*||*/
/*				module.getType() == NEType::UNDEFINED*/)
			{
				//	���� ����:				
				KERNAL_ERROR(" : ���� ����� �����ؾ��ϴµ�, ���� ����δ� ������ �ݱ� �����");
			}
			else	//	�ε��� ����� �ϳ����:
			{
				//			��� �ε�:	���������� ����� �ε��Ѵ�.
				//				��� ������ ���ؼ� �������� ��Ż:
				_occupiedset[n] = false;
				//				�־��� ��� ����: 
				//					push���� NEModule::clone�� ȣ��
				//					NEModule::NEModuleCodeSet�� ����Ǹ鼭 push(NEModuleCodeSet&)�� ȣ��
				//					push(NEModuleCodeSet&)���� NEKeyArray���κ��� clone���� Ű�� ����
				NEEnlistableManager* nullpointer = NE_NULL;
				insert(n, module);				
				//				������ �ε�:
				//					���⼭ NEModuleCodeSet::serialize�� ȣ��
				//					serialize���� NEKeyCodeSetjuyhg
				loader >> getElement(n); // ���⼭ �ٽ� NEModule::serialize()���� initialize�� ȣ���Ѵ�
			}
			//	��ŵ�ε�:
			//		��ŵ�� ����ϴ°�?:	���������Ͱ� ���� ���������� �ٸ��ٸ�
			if(loader.getPosition() != next_module_to_skip)
			{
				//		��ŵ:	
				//			�ּҸ� ��ŵ�ϰ� ���� ��ġ�� �̵��Ѵ�.
				//			���� �� ����� ������ ����̾��ٸ�, ���� �����Ͱ� ��ġ�� ��������, 
				//			���� NENode Ȥ��, NENode�� �ε��� ���̴�.
				//			���� : ���� ����� �ε尡 �ʿ����.
				KERNAL_ERROR("E100031C77 : ������ ��߳� ����������\n���¿��� ����� �ν��Ͻ��� �����ϴ� ��, ���������Ͱ� ��߳����ϴ�.\n���������Ͱ� ��߳��� ���� ����� �ε��� �� �����Ƿ�, ���������͸� ������ �����մϴ�.\n���� ���������� ��ġ: %d\n�����ߴ� ���������� ��ġ : %d\n������ �߻��� ����� �ĺ��� : \n\t�̸� : %s\n\t������ : %s\n\t������ȣ : %d",loader.getPosition(), next_module_to_skip, identifier.getName().toCharPointer(), identifier.getDeveloper().toCharPointer(), identifier.getRevision());

				loader.setPosition(next_module_to_skip);
			}			
		}

		return loader >> _last_generated_id;
	}

	type_result NEModuleArray::_onEnlisted()
	{
		return RESULT_SUCCESS;
	}

	type_result NEModuleArray::resize(type_index new_size)
	{
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_result result = Super::resize(new_size);

		NEGlobalManagerOffer::_setGlobalManager(push);
		return result;
	}
}