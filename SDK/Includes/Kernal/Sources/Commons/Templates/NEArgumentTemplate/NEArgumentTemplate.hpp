#pragma once

#include "../../Units/NEArgumentBase/NEArgumentBase.hpp"

namespace NE
{
	template <typename T>
	class NEArgumentTemplate : public NEArgumentBase
	{
	public:
		typedef NEArgumentTemplate ThisClass;
		typedef ThisClass TArgument;
		typedef NEArgumentBase SuperClass;

	public:	
		NEArgumentTemplate() 
			: SuperClass(T().getType()), _binded(0x00) 
		{
			
		}

	public:
		bool operator==(const ThisClass& rhs) const
		{		
			if(SuperClass::operator!=(rhs)) return false;

			return	_binded == _binded;
		}
		bool operator!=(const ThisClass& rhs) const
		{
			return ! operator==(rhs);
		}

	public:
		T& getBindedKey() { return *_binded; }
		const T& getBindedKey() const { return _binded; }

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual void release()
		{
			SuperClass::release();

			_binded = 0x00;
		}


	public:
		typename T::Trait& getValue() 
		{
			if( ! _binded)
				bind(); 

			if(_binded)
				return _binded->getValue();
			else
			{
				T::Trait* nullpoint = 0;
				return *nullpoint;
			}
		}
		type_result bind()
		{
			if(getBinder().isBinded())	return RESULT_SUCCESS | RESULT_ABORT_ACTION;

			type_result result = getBinder().bind();
			if( ! NEResult::hasError(result)							&&	//	���ε��� ����� �ǰ�
				getBinder().getBinded().isSubClassOf(getTypeToBeBinded())	)	//	Ÿ���� �´ٸ�(�̸���ġ ���δ� _binder�� Ȯ�����ش�.
				_binded = &static_cast<T&>(getBinder().getBinded());

			return result;
		}

	private:
		T* _binded;
	};
}