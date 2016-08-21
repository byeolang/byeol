namespace NE
{
	template <typename T>
	NETArithmeticKey<T>::NETArithmeticKey(const T& source, const NETString& keyname)
		: Super(source, keyname)
	{

	}

	template <typename T>
	NETArithmeticKey<T>::NETArithmeticKey(const This& source)
		: Super(source)
	{

	}
	
	template <typename T>
	NETArithmeticKey<T>::NETArithmeticKey(const NEKey& source)
		: Super(source)
	{
		assign(source);
	}

	template <typename T>
	type_bool NETArithmeticKey<T>::isEqualTo(const NEKey& source) const
	{
		//	pre:
		if (this == &source) return true;



		//	main:
		//		���� ����:
		This buffer;
		//		���ۿ� �Ҵ�:
		type_result result = buffer.assign(source);
		if (NEResult::hasError(result))
			return false;



		//	post:
		return _value == buffer._value;
	}

	//	---------------------------------------------------------------------------------
	//	����	:	NEKey���� ĳ���� ���� �Ҵ��� �����ϰ� �ϴ� �Լ�. operator=�� ����.
	//				���ο��� ĳ������ �ǽ��Ѵ�.
	//	��������:
	//	�޸�	:
	//	�����丮:	2011-07-07	������	���� �Ϸ�
	//	---------------------------------------------------------------------------------
	/*
	NEManagedNodeSet�� NEManagedModuleSet�� �⺻ assign�� ����Ѵ�.
	*/
	template <typename T>
	type_result NETArithmeticKey<T>::assign(const NEKey& source)
	{
		if (!&source) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_WRONG_POINTER_ADDRESSING | RESULT_ABORT_ACTION;
		if (this == &source) return RESULT_SUCCESS | RESULT_ABORT_ACTION | RESULT_ABORT_ACTION;
		if (isSubClassOf(NETClass<NENodeSelectorBase>())) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;
		if ( ! isEqualClassTo(source)) return RESULT_TYPE_ERROR | RESULT_WRONG_PARAMETER | RESULT_ABORT_ACTION;

		Super::assign(source);
		_value = static_cast<const NETArithmeticKey<T>&>(source).getValue();

		return RESULT_SUCCESS;
	}
	template <>
	type_result NETArithmeticKey<type_bool>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_byte>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_ubyte>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_char>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_wchar>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_short>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_ushort>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_int>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_uint>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_float>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_double>::assign(const NEKey& source);
	template <>
	type_result NETArithmeticKey<type_int64>::assign(const NEKey& source);


	template <typename T>
	type_bool NETArithmeticKey<T>::isLower(const NEKey& source) const
	{
		//	pre:
		if (!&source) return false;



		//	main:
		//		���� ����:
		This buffer;
		//		���ۿ� �Ҵ�:
		type_result result = buffer.assign(source);
		if (NEResult::hasError(result))
			return false;



		//	post:
		return _value < buffer._value;
	}


	template <typename T>
	type_result NETArithmeticKey<T>::add(NEKey& result, const NEKey& operand2) const
	{
		This converted;
		converted.assign(operand2);		
		
		return result.assign(This(getValue() + converted.getValue()));
	}

	template <typename T>
	type_result NETArithmeticKey<T>::increase()
	{
		_value++;

		return RESULT_SUCCESS;
	}

	template <>
	type_result NETArithmeticKey<type_bool>::increase();

	template <typename T>
	type_result NETArithmeticKey<T>::decrease()
	{
		_value--;

		return RESULT_SUCCESS;
	}

	template <>
	type_result NETArithmeticKey<type_bool>::decrease();

	template <typename T>
	type_result NETArithmeticKey<T>::multiply(NEKey& result, const NEKey& operand2) const
	{
		This converted;
		converted.assign(operand2);

		return result.assign(This(getValue() * converted.getValue()));
	}

	template <typename T>
	type_result NETArithmeticKey<T>::makeReciprocal()
	{
		if (_value == T())
		{
			_value = std::numeric_limits<T>::max();

			return RESULT_TYPE_WARNING | RESULT_WRONG_BOUNDARY;
		}

		_value = 1 / _value;

		return RESULT_SUCCESS;
	}

	template <typename T>
	type_result NETArithmeticKey<T>::negate()
	{
		_value = -_value;

		return RESULT_SUCCESS;
	}
}