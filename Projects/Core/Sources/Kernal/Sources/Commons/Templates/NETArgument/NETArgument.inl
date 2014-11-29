namespace NE
{
	template <typename T>
	NETArgument<T>::NETArgument() 
		: SuperClass(T().getType())
	{

	}

	template <typename T>
	NEObject& NETArgument<T>::clone() const
	{
		return *(new ThisClass(*this));
	}

	template <typename T>
	typename const T::Trait& NE::NETArgument<T>::getValue() const
	{
		const NEKey& binded = getBinded();
		if( &binded)
		{
			if(isValidToBind(binded))
				return (static_cast<const T&>(binded)).getValue();
			else
			{
				_for_casting = binded;
				return _for_casting.getValue();
			}
		}

		return getDefault();
	}

	template <typename T>
	type_result NE::NETArgument<T>::setDefault(typename const T::Trait& source)
	{
		_default_key.getValue() = source;

		return RESULT_SUCCESS;
	}

	template <typename T>
	const NEKey& NE::NETArgument<T>::getDefaultKey() const
	{
		return _default_key;
	}

	template <typename T>
	NEKey& NE::NETArgument<T>::getDefaultKey()
	{
		return _default_key;
	}

	template <typename T>
	type_result NE::NETArgument<T>::setValue(typename const T::Trait& source)
	{
		NEKey& binded = getBinded();
		if(	! &binded)	return RESULT_TYPE_WARNING | RESULT_ABORT_ACTION;


		//	main:
		if(isValidToBind(binded))
		{
			T& casted = static_cast<T&>(binded);
			casted.getValue() = source;
		}
		else
			binded = T(source);

		return RESULT_SUCCESS;			
	}

	template <typename T>
	typename const T::Trait& NE::NETArgument<T>::getDefault() const
	{
		return _default_key.getValue();
	}
	template <typename T>
	void NE::NETArgument<T>::release()
	{
		SuperClass::release();
		_for_casting.release();
		_default_key.release();
	}

	template <typename T>
	type_result NE::NETArgument<T>::isValid() const
	{
		type_result result = SuperClass::isValid();
		if(NEResult::hasError(result)) return result;

		return _default_key.isValid();
	}

	template <typename T>
	NEBinaryFileSaver& NE::NETArgument<T>::serialize(NEBinaryFileSaver& saver) const
	{
		SuperClass::serialize(saver);

		return saver << _default_key;
	}

	template <typename T>
	NEBinaryFileLoader& NE::NETArgument<T>::serialize(NEBinaryFileLoader& loader)
	{
		SuperClass::serialize(loader);

		return loader >> _default_key;
	}

	template <typename T>
	class NE_DLL NETLimitedArgument : public NEArgumentBase
	{
	public:
		typedef NETLimitedArgument ThisClass;
		typedef NEArgumentBase SuperClass;

	public:
		NETLimitedArgument()
			: SuperClass(T().getType())
		{

		}
		NETLimitedArgument(const ThisClass& rhs)
			: SuperClass(rhs), _default_key(rhs._default_key)
		{

		}

	public:
		typename T::Trait& getValue()
		{
			NEKey& binded = getBinded();
			if( &binded					&&
				isValidToBind(binded)	)
				return static_cast<T&>(binded).getValue();

			return getDefault();
		}
		typename const T::Trait& getValue() const
		{
			ThisClass* unconsted = const_cast<ThisClass*>(this);

			return unconsted->getValue();
		}
		typename T::Trait& getDefault()
		{
			return _default_key.getValue();
		}
		typename const T::Trait& getDefault() const
		{
			return _default_key.getValue();
		}

	public:
		virtual	NEKey& getDefaultKey()
		{
			return _default_key;
		}
		virtual const NEKey& getDefaultKey() const
		{
			return _default_key;
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual void release()
		{
			SuperClass::release();

			_default_key.release();
		}
		virtual type_result isValid() const
		{
			type_result result = SuperClass::isValid();
			if(NEResult::hasError(result)) return result;

			return _default_key.isValid();
		}
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader)
		{
			SuperClass::serialize(loader);

			return loader >> _default_key;
		}
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const
		{
			SuperClass::serialize(saver);

			return saver << _default_key;
		}

	private:
		T _default_key;
	};	

	template <>
	class NE_DLL NETArgument<NEBooleanSetKey> : public NETLimitedArgument<NEBooleanSetKey>{};
	template <>
	class NE_DLL NETArgument<NEByteSetKey> : public NETLimitedArgument<NEByteSetKey>{};
	template <>
	class NE_DLL NETArgument<NEUByteSetKey> : public NETLimitedArgument<NEUByteSetKey>{};
	template <>
	class NE_DLL NETArgument<NEShortSetKey> : public NETLimitedArgument<NEShortSetKey>{};
	template <>
	class NE_DLL NETArgument<NEUShortSetKey> : public NETLimitedArgument<NEUShortSetKey>{};
	template <>
	class NE_DLL NETArgument<NEIntSetKey> : public NETLimitedArgument<NEIntSetKey>{};
	template <>
	class NE_DLL NETArgument<NEUIntSetKey> : public NETLimitedArgument<NEUIntSetKey>{};
	template <>
	class NE_DLL NETArgument<NEInt64SetKey> : public NETLimitedArgument<NEInt64SetKey>{};
	template <>
	class NE_DLL NETArgument<NEFloatSetKey> : public NETLimitedArgument<NEFloatSetKey>{};
	template <>
	class NE_DLL NETArgument<NEDoubleSetKey> : public NETLimitedArgument<NEDoubleSetKey>{};
	template <>
	class NE_DLL NETArgument<NEStringSetKey> : public NETLimitedArgument<NEStringSetKey>{};
	template <>
	class NE_DLL NETArgument<NEModuleCodeSetKey> : public NETLimitedArgument<NEModuleCodeSetKey>{};
	template <>
	class NE_DLL NETArgument<NENodeCodeSetKey> : public NETLimitedArgument<NENodeCodeSetKey>{};
	template <>
	class NE_DLL NETArgument<NENodeSelector> : public NETLimitedArgument<NENodeSelector>{};
	template <>
	class NE_DLL NETArgument<NEModuleSelector> : public NETLimitedArgument<NEModuleSelector>{};
	template <>
	class NE_DLL NETArgument<NEKeySelector> : public NETLimitedArgument<NEKeySelector>{};

	template <>
	class NE_DLL NETArgument<NEKey> : public NEArgumentBase	
	{	
	public:	
		typedef NETArgument ThisClass;	
		typedef NEArgumentBase SuperClass;	
		typedef NEKey T;

	public:
		NETArgument();
		NETArgument(const ThisClass& rhs);

	public:
		virtual	NEKey& getDefaultKey();
		virtual const NEKey& getDefaultKey() const;
		virtual NEObject& clone() const;	
	};
}