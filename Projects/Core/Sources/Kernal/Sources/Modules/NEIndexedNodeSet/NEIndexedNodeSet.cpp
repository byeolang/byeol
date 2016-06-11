#include "NEIndexedNodeSet.hpp"
#include "../Kernal/Kernal.hpp"

namespace NE
{
	type_id NEIndexedNodeSet::_last_generated_id = 0;

	NEIndexedNodeSet::NEIndexedNodeSet(NEEnlistableManager& manager)
		: Super(manager)
	{

	}

	NEIndexedNodeSet::NEIndexedNodeSet(type_count size)
		: Super(size)
	{

	}

	NEIndexedNodeSet::NEIndexedNodeSet(const This& source)
		: Super(source)
	{

	}

	NEIndexedNodeSet::~NEIndexedNodeSet()
	{

	}

	const NEIndexedNodeSet& NEIndexedNodeSet::operator+=(const This& source)
	{
		Super::operator+=(source);

		return *this;
	}

	NEIndexedNodeSet NEIndexedNodeSet::operator+(const NEIndexedNodeSet& source) const
	{
		This buffer(getSize() + source.getSize());

		buffer.push(*this);
		buffer.push(source);

		return buffer;
	}

	type_id NEIndexedNodeSet::peekLastGeneratedId() const
	{
		return _last_generated_id;
	}

	type_id NEIndexedNodeSet::_generateId()
	{
		_last_generated_id++;
		if(_last_generated_id <= 0)
			KERNAL_ERROR(" : ");

		return _last_generated_id;
	}

	type_index NEIndexedNodeSet::insert(type_index index, const NENode* const source)
	{
		if( ! &source)
		{
			KERNAL_ERROR(" : �־��� ������ �����ϴ�.");
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



		//	��������ڷ� �ν��Ͻ� ����:
		//		�ν��Ͻ� ���� �˰���:
		//			���� �����ΰ�:
		//				1. �־��� source�� _manager�� ������ �ִ�. 
		//				2. �־��� source�� Ÿ���� NEModule�ν� �̴� ADT�̴�. ���� source��
		//				���� Ÿ���� ���������� �� �� ����.
		//				3. source�� ����Ÿ�Կ� ������� ����� ���縦 �ϴ� �Լ��δ� ����,
		//				clone(���󺹻������)�� �����ϴ�.
		//				4. clone�� ��� �Ķ���͵� ���� �ʵ��� �ۼ��Ǿ��ִ�.
		//				5. Super::insert������ clone�� ������ T*�� ������ �δµ�, 
		//				source.keyset�� clone�� ���ؼ��� ���簡 �Ұ����ϴ�.
		//				���簡 �Ƿ���, ��������� ��ü�� manager�� �Ҵ�Ǿ� �־�� keyset��
		//				manager �����͸� ���� �ν��Ͻ��� ����ϱ� �����̴�.
		//				6. clone�� ���ڿ����̴�.
		//				��, �����ϰ�->manager�� set�ϰ�->�����Ѵٴ� ������ ������ �� ����.
		//
		//			� �ذ����� �־��°�:
		//				������ ������ ���ÿ� �����Ѵ�.----> ����->manager set-> ����
		//				�� ���·� �߰��� manager�� set �� �� �ֵ��� �ϸ� �ȴ�.
		//				����, �����غ���, "������ ���ÿ� set. ���� ����" ��
		//				"���� ����, manager set�� ���ÿ� ����"�� ������ �� �� �ִ�.
		//				������ ���� �����ڿ��� manager�� �Ѱ��� ���Ŀ� �����
		//				�����Ҵ���(���� ��� virtual assign(module))���� ���� �߰��ϴ� 
		//				����� �ǰڴ�.
		//				�ƴϸ� ���� virtual clone() �̿ܿ��� virtual clone(manager)�� �ϳ� ��
		//				����� ����� ������ �� �� �ִ�.
		//				��������ڿ��� source�� _manager�� ���� �����ϴ� ����� ������ �� ��
		//				�ְ�����, �̷��� �ϸ� ����� �ν��Ͻ��� ������ manager�� �ƴ�, source��
		//				manager�� �����ְ� �ǹ����Ƿ� ����ġ ���ϴ�.
		//				�̷��� ����� �ִ�. clone�� ȣ���ϸ� �������(keyset)�� �Ҵ���� �ʴ���
		//				source�� ����� ��Ȯ�ϰ� ���� �� �� �ִ�.
		//				�� ���Ŀ� manager�� set�ϰ�, �ٽ��ѹ� operator=�� �ϴ� ����̴�.
		//					NEModule& cloned = source.clone();	//	����� ��ü�� �������
		//					cloned.manager is my manager;		//	�Ŵ��� �Ҵ�
		//					cloned.operator=(source)			//	���� ��������� ����
		//				�ٸ� �� ����� ���� ū ������ keyset�� ������ �ʴ� ��������� ���� �� �� 
		//				���ٴ� ������ �ִٴ� ���̴�.
		//			
		//			���� �ذ����� �����ΰ�:
		//				��ü ������, ��Ӱ����� �Ųٷ� �ö󰡸鼭 �����ڸ� ȣ���ذ���.
		//				�� �߰��뿡 manager�� �����ϴ� Ŭ������ ���� ���̴�. �� �����ڰ� ȣ��Ǿ�
		//				���� �ܺο��� Ư���� manager���� �����ϴ� �̺�Ʈ �ڵ鷯�� ������ �ɰ��̴�.
		//				�׷��� �̸� �ذ��ϱ� ���� static���� global_manager��� ����� ����Ѵ�.
		//					static Manager* getGlobalManager();
		//					static setGlobalManager(Manager*);
		//				�� �Լ��� �̿��ؼ� �ش��ϴ� manager ��������� ������ Ŭ������ ȣ�� �Ǿ�����
		//				Ư���� �־��� manager�� ������ global_manager�� �Ҵ��ϴ� ����̴�.
		//				�ٸ� �� globalmanager�� ����ϴ� �����ڴ� ��������ڷ� �����Ѵ�.
		//				�Ϲ� �����ڴ� manager�� �Ҵ��� �� �ִ� �Լ��� �̹� �����Ƿ�, ����ڰ� manager
		//				�ܺηκ��� �Ҵ��ϰ� �;����� �ƴ��� �ǵ��� �ľ��� �� �ֱ� �����̴�.
		//				
		//			����Ǵ� ������:
		//				�� ������ ����Ǵ� �������� ������ ����.
		//					1. static�̱� �ϳ� �����ڿ��� �޼ҵ带 ȣ���Ѵٴ� ������ ����ġ���� ������
		//					���� �� �ִ�.	-> push pop�� ������ �����Ͽ� ���� �κ� �ذ�
		//					2. �Ҵ��� global manager���� ���������� ������ �����ٸ� ��ü�� �����ɶ���
		//					�� manager�� �Ҵ�ǹ����� ������ �߻��� ���̴�.	-> �ڵ� �ۼ��� �����ϸ� 
		//																	������ Ȯ�� ����
		//					3. ���� ������ �ϳ��� ��ü�� ���� global_manager�� �Ҵ��Ѵ� �ϴ��� ����������
		//					�ٸ� manager ������ �ִ� ��ü�� �����ϰ��� �Ҷ��� ���� ���� �ִ�.
		//
		//			�������:
		//				Super�� IndexedArrayTemplate���� clone�� ���Ǳ� �������� _setGlobalManager
		//				�� ������Ѵ�. ���������� ���� ����� ������ ����.
		//					1. insert
		//					2. resize
		//					3. setElement

		//	������ �ڵ鷯:	���� �ڼ��� ������ NEIndexedModuleSet.cpp �����϶�
		//		���� manager ��:		
		//			Ÿ����:
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		//			����:
		NEGlobalManagerOffer::_setGlobalManager(getManager());
		//		����:
		type_index inputed_index = Super::insert(index, source);
		//		���̵� �Ҵ�:
		//			Ÿ����:
		NENode& node = getElement(inputed_index);
		if(&node)
			node._id = _generateId();
		//		�ǵ�����:
		NEGlobalManagerOffer::_setGlobalManager(push);

		//		��� �ʱ�ȭ ���� �Ǵ�:	
		//			�־��� ����� �ʱ�ȭ�� �Ǿ��ִ� ���� �Ǵ��ϰ�, �ʱ�ȭ��
		//			�ȵǾ����������� �ʱ�ȭ�� �����ؾ� Ű���� ����� �����ϰ� �ȴ�.
		//
		//			�� �ʱ�ȭ�� �ؾ� �ϴ°�:
		//				����� enlist���� ���� ä�� Ű�¿� Ű�� ���� ���� ���� �����̴�.
		//				����� Ű��(�ڵ�)�� ���� �ν��Ͻ��� ��ܾ� �ϴ� Ű��(�ν��Ͻ�)
		//				�� �����ϴ� �Ŵ����� �˰� �־�� �Ѵ�.
		//				�׷��� �ܺο��� ���ǵ� ����� ���� �Ŵ����� �����Ƿ� Ű�¿� 
		//				��� Ű�� ���� �� ����.
		//					
		//			��� �Ŵ����� ����� �ʱ�ȭ�� ������ �� ����:
		//				���Ŵ����� �����̳� instance ���� �������� �ʴ´�.
		//				�̴� EnlistableManager�κ��� ����� �޾ƾ� �ϳ�, ���Ŵ�����
		//				��尡 ��ϰ����� �Ŵ����� ������ �ƴϹǷ� �Ұ����ϴ�.
		//				
		//			��� �ʱ�ȭ�� �Ǿ������� Ȯ�� �ϴ°�:
		//				�˻� ����� �̹� �߰��� �ν��Ͻ���. ���� ������ "�־��� �����
		//				������ �ִ� Ű���� �̹� �ʱ�ȭ�� ����Ǿ������� �� �� �ִ°�"
		//				�� �ȴ�. 
		//					1. ��� ����� NEKeyCodeSet�� ������ �ִ�.
		//					2. ��� CodeSet�� enlist�ɶ� �ݵ�� �ڽ��� Manager���� �Ҵ�޴´�.
		//				���� ���� ���� ����� �̿��Ͽ�, ����� Ű���� �Ŵ����� �Ҵ���
		//				�Ǿ� ���� ���� ���� �ʱ�ȭ�� �ʿ��� ����� ���� �� �� �ִ�.
		//
		//			�ó����� ����:
		//				���Ŵ����κ��� push�� ���:
		//					1. ���Ŵ����κ��� ����� �ϳ� ������(����X)
		//						���Ŵ����� ����� �ʱ�ȭ�� �� �Ǿ������Ƿ�, KeyCodeSet.manager 
		//						is 0x00�̴�.
		//					2. �ش� ����� enlisted ���¿� push.
		//						���¿����� �־��� ����� �������. ��������ÿ� _manager ����
		//						������� �ʴ� "�ν��Ͻ� �켱"��å�� ������ �ִ�.
		//						(_is_rootnode)�� ����
		//					3. ���������� ����� push�ǰ� _onEnlist(Module) ȣ��
		//						�־��� ����� Ű���� manager = 0x00�̹Ƿ� �Ŵ����� �� �ϸ鼭,
		//						����� �ʱ�ȭ.


		return inputed_index;
	}

	type_result NEIndexedNodeSet::setElement(type_index index, const NENode* const source)
	{
		if( ! &source)
		{
			KERNAL_ERROR(" : ");
			return NE_INDEX_ERROR;
		}



		NENode* const_casted = const_cast<NENode*>(source);

		type_id original_id = const_casted->getId();
		const_casted->_id = _generateId();

		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());

		type_result result = Super::setElement(index, source);

		NEGlobalManagerOffer::_setGlobalManager(push);

		const_casted->_id = original_id;

		return result;
	}

	NEBinaryFileSaver& NEIndexedNodeSet::serialize(NEBinaryFileSaver& saver) const
	{
		Super::serialize(saver);
		
		for(int n=0; n < getSize() ;n++)
		{
			const NENode& node = getElement(n);
			if( ! &node) continue;

			saver << node;
		}
		
		return saver << _last_generated_id;
	}

	NEBinaryFileLoader& NEIndexedNodeSet::serialize(NEBinaryFileLoader& loader)
	{
		//	pre:
		//		���� �Լ� ȣ��:
		//			���ο��� size, length�� _occupiedset�� load �ؼ� ������ �´�.
		Super::serialize(loader);
		

		//	main:
		type_index size = getSize();
		//		push�� ���� ���� ���� �ʱ�ȭ:
		_length = 0;
		type_index occupied_n = -1;

		for(int n=0; n < size ;n++)
		{
			if( ! _occupiedset[n]) continue;

			//	push:
			//		push�� ���ؼ� �������� �Ͻ� ����:
			_occupiedset[n] = false;
			//		����:
			insert(n, NENode());
			//		����ó��:
			NENode& inputed = getElement(n);
			if( ! &inputed)
			{
				KERNAL_ERROR(" : IndexedNodeSet���� �ε� ���߿� ��带 �������� ���߽��ϴ�.\n�����͸� ������ ���� ��带 �н��ϴ�.");
				//	������ ���� ��带 �ϳ� ����:
				loader >> NENode();
				continue;
			}
			//		�ε�:
			loader >> inputed;
		}

		return loader >> _last_generated_id;
	}

	type_result NEIndexedNodeSet::remove(type_index index)
	{
		//	����� _unlist ȣ��:
		NENode& node = getElement(index);
		if( ! &node	|| ! getOccupiedSet()[index])
		{
			KERNAL_WARNING(" : IndexedNodeSet���� �������� �ʴ� ��带 �����Ϸ��߽��ϴ�.")
			return RESULT_ABORT_ACTION | RESULT_TYPE_WARNING;
		}

		type_result result = node._onUnlisted();
		
		return result |= Super::remove(index);
	}

	type_result NEIndexedNodeSet::_onEnlisted()
	{
		return RESULT_SUCCESS;
	}

	type_result NEIndexedNodeSet::resize(type_count new_size)
	{
		NEEnlistableManager& push = NEGlobalManagerOffer::getGlobalManager();
		NEGlobalManagerOffer::_setGlobalManager(getManager());
		
		type_result result = Super::resize(new_size);

		NEGlobalManagerOffer::_setGlobalManager(push);

		return result;
	}
}