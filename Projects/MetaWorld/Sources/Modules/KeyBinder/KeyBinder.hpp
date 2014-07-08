#pragma once

#include "../../Includes/Includes.hpp"

namespace MetaWorld
{
	class NE_DLL KeyBinder : public NEModule
	{
	public:
		typedef KeyBinder ThisClass;
		typedef NEModule SuperClass;

	public:
		const NEKeySelector& getSourceSelector() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key || ! key.isSubClassOf(NEType::NEKEY_SELECTOR))
			{
				ALERT_ERROR("0�� Ű�� NEKEY_SELECTOR ���� �մϴ�.");
				NEKeySelector* nullptr = 0;
				return *nullptr;
			}

			return static_cast<const NEKeySelector&>(key);
		}
		NEKeySelector& getSourceSelector()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NEKeySelector&>(casted->getSourceSelector());
		}

	public:
		virtual type_result initialize()
		{
			NEKeyCodeSet& ks = getKeySet();
			ks.create(1);
			ks.push(NEKeySelector());	//	0:	ScriptEditor�� �ִ� ���� ���. NodeVisualizer, WorldVisualizer�� ���ؼ� ���ε��ȴ�.
			return RESULT_SUCCESS;
		}

	public:
		virtual type_result execute()
		{
			return RESULT_SUCCESS;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _instance;

			if(_instance.getName().getSize() <= 0)
			{
				_instance.getName() = "KeyBinder";
				_instance.setRevision(1);
				_instance.getDeveloper() = "kniz";
			}

			return _instance;
		}

	public:
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}

	public:
		static const NECodeSet& getScriptCodeSet()
		{
			static NECodeSet _instance;
			if(_instance.getSize() <= 0)
			{
				_instance.create(1);
				_instance.setCodeType(NECodeType::SCRIPT);
				_instance.push(ThisClass().getHeader());
			}

			return _instance;
		}

	};
}