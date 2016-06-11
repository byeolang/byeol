//	---------------------------------------------------------------------------------
//	Ŭ������:	NEModuleManager::DLLHeader
//	����	:	���Ŵ����� ���ؼ� fetch�Ǵ� DLL�� ���.
//				���߿� DLL�� ������ �ʿ��� ����.
//	����	:	���Ŭ����.	NEObject
//	Ư��	:	
//	�˰���:	
//	�����:	
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	
//	---------------------------------------------------------------------------------
#pragma once

#include "../NEIdentifier/NEIdentifier.hpp"

namespace NE
{
	typedef NEClassBaseList (NE_FUNCTION_CALL *EntryPoint)(void);

	class NE_DLL NEPackage : public NEIdentifier
	{
		//	Define typedefs for RTTI in manual:
		//		because this class was designated to block calling 'clone()'
		//		for some reason. so, we can't just use DECLARE_CLASS macro.
		typedef NEPackage This;
		typedef NEIdentifier Super;
		typedef NETConcreteClass<This> MetaClass;

	public:
		friend class NEPackageManager;

	private:
		NEPackage();
		NEPackage(const This& rhs);
		
	public:
		virtual ~NEPackage();

	public:
		const This& operator=(const This& source);
		bool operator==(const This& source) const;
		bool operator!=(const This& source) const;

	public:
		NETString& getPath();
		const NETString& getPath() const;
		HINSTANCE& getInstance();
		const HINSTANCE& getInstance() const;
		EntryPoint& getEntryPoint();
		const EntryPoint& getEntryPoint() const;
		const NEClassBaseList& getComponents() const;
		NEClassBaseList& getComponents();
		bool isLoaded() const;

	public:
		virtual const NEClassBase& getClass() const;
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;
	private:
		virtual NEObject& clone() const;

	private:
		NETString _path;
		HINSTANCE _instance;
		EntryPoint _entrypoint;
		NEClassBaseList _components;

	public:
		static const NEClassBase& getClassStatically();
	};
}