namespace NE
{
	template <typename T>
	NEITArgument<T>::NEITArgument(const ThisClass& rhs)
		: NEArgumentInterfaceBase(rhs), _concrete_class(0x00), _default(rhs._default) 
	{

	}

	template <typename T>
	NE::NEITArgument<T>::NEITArgument(typename const T::Trait& new_default)
		: _concrete_class(0x00), _default(new_default) 
	{

	}

	template <typename T>
	NEType::Type NE::NEITArgument<T>::getKeyType() const
	{
		return T().getType();
	}

	template <typename T>
	typename const T::Trait& NE::NEITArgument<T>::getDefault() const
	{
		ThisClass* unconsted = const_cast<ThisClass*>(this);

		return unconsted->getDefault();		
	}

	template <typename T>
	typename T::Trait& NE::NEITArgument<T>::getDefault()
	{
		if( ! _concrete_class)
			return _default;	

		_constant_default = _default;

		return _constant_default;
	}


	template <typename T>
	typename T::Trait& NE::NEITArgument<T>::getValue()
	{
		if( ! _concrete_class) return getDefault();

		typename T::Trait& to_return = _concrete_class->getValue();
		if( ! &to_return)
			return  getDefault();

		return to_return;
	}

	template <typename T>
	typename const T::Trait& NE::NEITArgument<T>::getValue() const
	{
		if( ! _concrete_class) return getDefault();

		typename const T::Trait& to_return = _concrete_class->getValue();
		if( ! &to_return)
			return getDefault();

		return to_return;
	}

	template <typename T>
	type_result NE::NEITArgument<T>::_onInsertedInArguments(type_index index, NEArgumentSet& rhs)
	{
		if(rhs.getLength() == rhs.getSize())
			rhs.resize(rhs.getLength() + 1);

		ConcreteClass& pushed = (ConcreteClass&) rhs[rhs.insert(index, ConcreteClass())];
		if( ! &pushed)
			return RESULT_TYPE_WARNING;

		return RESULT_SUCCESS;
	}

	template <typename T>
	type_result NE::NEITArgument<T>::_onBindInstance(NEArgumentBase& concreted)
	{
		if( ! &concreted				||
			! isValidToBind(concreted)	)
			return RESULT_TYPE_WARNING;

		_concrete_class = static_cast<ConcreteClass*>(&concreted);

		return RESULT_SUCCESS;
	}

	template <typename T>
	typename NEITArgument<T>::ConcreteClass& NE::NEITArgument<T>::getConcreteInstance()
	{
		return *_concrete_class; 
	}

	template <typename T>
	typename const NEITArgument<T>::ConcreteClass& NE::NEITArgument<T>::getConcreteInstance() const
	{ 
		return *_concrete_class; 
	}

	template <typename T>
	T& NE::NEITArgument<T>::getKey()
	{ 
		if(_concrete_class)
			return _concrete_class->getBindedKey();

		T* nullpointer = 0;
		return *nullpointer;
	}

	template <typename T>
	const T& NE::NEITArgument<T>::getKey() const
	{
		if(_concrete_class)
			return _concrete_class->getBindedKey();

		T* nullpointer = 0;
		return *nullpointer;
	}

	template <typename T>
	typename NEITArgument<T>& NE::NEITArgument<T>::operator=(const ThisClass& src)
	{
		if(this == &src) return *this;

		_default = src._default;
		//	��å:
		//		"	����� ���� ���ε��� ����� ���� �� �Ҵ����� �ʴ´�.
		//			���� ���ε��� "�� �� �ִ�" ������ �Ѱ��༭, �� ���� ���ε� �ٽ� �ϵ��� �϶�	"
		//		�׷��� ��99�ε� ������� _concreate_class�� �Ѱ����� �ʴ´�. 
		//		��� default_value ���� �͸� �Ҵ� �����ش�.
		//		�ؿ� ó�� ���� �ʴ´ٴ� �ž�.
		//	
		// 			if(	_concrete_class && src._concrete_class												&&
		// 				_concrete_class>getTypeToBeBinded() == src._concrete_class>getTypeToBeBinded()	)
		// 				*_concrete_class = *src._concrete_class;				

		return *this;
	}
}

namespace NE	
{	
	template <>	
	class NE_DLL NEITArgument<NEKey> : public NEArgumentInterfaceBase	
	{	
	public:	
		typedef NEKey T;	
		typedef NEITArgument<NEKey> ThisClass;	
		typedef NEArgumentInterfaceBase SuperClass;	
		typedef NEArgumentTemplate<T> ConcreteClass;	

	public:	
		NEITArgument();	
		NEITArgument(const ThisClass& rhs);	

	public:	
		ThisClass& operator=(const ThisClass& src);	

	public:	
		virtual NEType::Type getKeyType() const;	
		virtual bool isValidToBind(const NEArgumentBase& arg) const;	

	public:	
		ConcreteClass& getConcreteInstance();	
		const ConcreteClass& getConcreteInstance() const;	
		T& getKey();	
		const T& getKey() const;	

	private:	
		virtual type_result _onInsertedInArguments(type_index index, NEArgumentSet& rhs);	
		virtual type_result _onBindInstance(NEArgumentBase& concreted);	

	private:	
		ConcreteClass* _concrete_class;	
	};	
}	