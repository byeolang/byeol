//	---------------------------------------------------------------------------------
//	Ŭ������:	NEClassBase
//	����	:	
//	����	:	
//	Ư��	:	
//	�˰���:	
//	�����:	
//	�޸�	:	
//	�����丮:	2016-02-09	������	����
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NEUnit/NEUnit.hpp"
#include "../../Modules/NETString/NETString.hpp"

namespace NE
{
	class NEClassBaseList;

	class NE_DLL NEClassBase : public NEUnit
	{
		//	Declarations:
	public:
		typedef NEClassBase This;
		typedef NEUnit Super;
		friend class NETypeManager;

		//	Constructors:
	public:
		NEClassBase();
		NEClassBase(const NEClassBase& source);

		//	Destructors:
	public:
		virtual ~NEClassBase();

		//	Interfaces:
		//		Virtuals:
	public:
		virtual NEObject& instantiate() const = 0;
		virtual const NETString& getName() const = 0;
		virtual const NEClassBaseList& getSuperClasses() const = 0;
		virtual const bool& isRegistered() const = 0;
		virtual const NEClassBaseList& getSubClasses() const = 0;
		virtual const type_id& getClassId();

		//			Inherits:
		//				NEObject:
	public:
		virtual type_result isValid() const;
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//		General interfaces:
	public:
		type_result enroll();
		bool isEqualTypeWith(const This& source) const;
		bool isEqualTypeWith(const NEObject& source) const;
		bool isSuperClassOf(const This& parent) const;
		bool isSuperClassOf(const NEObject& parent) const;
		bool isSubClassOf(const This& parent) const;
		bool isSubClassOf(const NEObject& parent) const;
		const This& getSuperClass() const;

	protected:
		type_result _alert(type_result result, const type_tchar* message) const;

	private:
		//	these method were used only for TypeManager friend class.
		void _setRegistered(bool new_value);
		void _setClassId(type_id new_id);
		type_result _registerSubClass(const NEClassBase& subclass);
		type_result _registerSubClassAsMyChild(const NEClassBase& subclass);
		NEClassBaseList& _getSubClasses();
	};
}
