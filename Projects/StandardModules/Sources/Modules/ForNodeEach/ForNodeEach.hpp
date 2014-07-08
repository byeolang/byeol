#pragma once

#include "../../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL ForNodeEach : public NEModule
	{
	public:
		typedef ForNodeEach ThisClass;
		typedef NEModule SuperClass;

	public:
		ForNodeEach();
		ForNodeEach(const ForNodeEach& source);

	public:
		virtual ~ForNodeEach();

	public:
		const NENodeSelector& getTargetSelector() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key)
			{
				ALERT_ERROR("0�� Ű�� �����ϴ�");
				NENodeSelector* nullpoint = 0;
				return *nullpoint;
			}

			return static_cast<const NENodeSelector&>(key);
		}
		NENodeSelector& getTargetSelector()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NENodeSelector&>(casted->getTargetSelector());
		}
		const type_int& getCount() const
		{
			const NEKey& key = getKeySet()[1];
			if( ! &key || ! key.isSubClassOf(NEType::NEINT_KEY))
			{
				ALERT_ERROR("1�� Ű�� NE_INT_KEY���� �մϴ�.");
				type_int* nullpoint = 0;
				return *nullpoint;
			}

			return (static_cast<const NEIntKey&>(key)).getValue();
		}
		type_int& getCount()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<type_int&>(casted->getCount());
		}
		const NEModuleCodeSet& getActions() const
		{
			const NEKey& key = getKeySet()[2];
			if( ! &key || ! key.isSubClassOf(NEType::NEMODULE_CODESET_KEY))
			{
				ALERT_ERROR("2�� Ű�� NEMODULE_CODESET_KEY���� �մϴ�.");
				NEModuleCodeSet* nullpoint = 0;
				return *nullpoint;
			}

			return (static_cast<const NEModuleCodeSetKey&>(key)).getValue();
		}
		NEModuleCodeSet& getActions()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NEModuleCodeSet&>(casted->getActions());
		}

	public:
		virtual type_result initialize();

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;

	public:
		virtual type_result execute();	

	public:
		virtual NEObject& clone() const;
	};
}
