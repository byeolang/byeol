//	NEITArgument<NEKey>:	
//	�� �ʿ��Ѱ�:	
//		Ű�� Ÿ���� ������� �ʰ� �����ؾ� �ϴ� ����� ���� �� �ֱ� �����̴�.	
//		�������, Ű ���� assign�� �����ϴ� ����� ��� Ű�� Ÿ���� Ưȭ��Ű��	
//		(NEIntKey ó��) �� Ű�� Ÿ�Ը��� ����� �ʿ��ϰ� �ȴ�.	
//		(NEIntKeyAssignModule, NEFloatKeyAssignModule ...)	
//		�� ��� ����, �ܺ��� Ű�� Ÿ�Կ� ������� implicit conversion �����	
//		���۵ȴ�.	

#pragma once

#include "../../Units/NEArgumentInterfaceBase/NEArgumentInterfaceBase.hpp"
#include "../NEArgumentTemplate/NEArgumentTemplate.hpp"
#include "../../../Modules/NEArgumentSet/NEArgumentSet.hpp"
#include "../../../Modules/NEKey/NEKey.hpp"

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
		NEITArgument(const ThisClass& rhs);
		NEITArgument(typename const T::Trait& new_default = T::Trait());

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
		typename const T::Trait& getDefault() const;
		typename T::Trait& getDefault();

	private:
		virtual type_result _onInsertedInArguments(type_index index, NEArgumentSet& rhs);
		virtual type_result _onBindInstance(NEArgumentBase& concreted);

	private:
		ConcreteClass* _concrete_class;
		typename T::Trait _constant_default;
		typename T::Trait _default;
	};

	//	���ø� Ŭ������ ��Ī ��뿡 ���Ͽ�:
	//		template<typename T>
	// 		typedef NEITArgument ITArgument;
	//		�ƽ��Ե� ������ �������� �ʴ´�. �ش� ����� alias template�̶�� �Ͽ�,
	//		C++ x11���� ������ �ȴ�. (�ű⼭���ʹ� using�� ����Ѵ�)
}

#include "NEITArgument.inl"