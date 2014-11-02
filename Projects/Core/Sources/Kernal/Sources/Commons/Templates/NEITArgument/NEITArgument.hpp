#pragma once

#include "../../Units/NEArgumentInterfaceBase/NEArgumentInterfaceBase.hpp"
#include "../NEArgumentTemplate/NEArgumentTemplate.hpp"
#include "../../../Modules/NEArgumentSet/NEArgumentSet.hpp"

namespace NE
{
	template <typename T>
	class NEITArgument : public NEArgumentInterfaceBase
	{
	public:
		typedef NEITArgument ThisClass;
		typedef NEArgumentInterfaceBase SuperClass;
		typedef NEArgumentTemplate<T> ConcreteClass;

	public:
		friend class NEArguments;

	public:
		NEITArgument();
		NEITArgument(const ThisClass& rhs);
		NEITArgument(const T& new_default);

	public:
		ThisClass& operator=(const ThisClass& src);

	public:
		virtual NEType::Type getKeyType() const;

	public:
		ConcreteClass& getConcreteInstance();
		const ConcreteClass& getConcreteInstance() const;
		T& getKey();
		const T& getKey() const;
		typename T::Trait& getValue();
		typename const T::Trait& getValue() const;
		const T& getDefault() const;
		T& getDefault();

	private:
		virtual type_result _onInsertedInArguments(type_index index, NEArgumentSet& rhs);
		virtual type_result _onBindInstance(NEArgumentBase& concreted);

	private:
		ConcreteClass* _concrete_class;
		T _default;
	};

	//	���ø� Ŭ������ ��Ī ��뿡 ���Ͽ�:
	//		template<typename T>
	// 		typedef NEITArgument ITArgument;
	//		�ƽ��Ե� ������ �������� �ʴ´�. �ش� ����� alias template�̶�� �Ͽ�,
	//		C++ x11���� ������ �ȴ�. (�ű⼭���ʹ� using�� ����Ѵ�)
}

#include "NEITArgument.inl"