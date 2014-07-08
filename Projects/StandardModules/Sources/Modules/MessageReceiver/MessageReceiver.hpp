#pragma once

#include "../../Includes/Includes.hpp"

namespace NE
{
	class NE_DLL MessageReceiver : public NEModule
	{
	public:
		typedef MessageReceiver ThisClass;
		typedef NEModule SuperClass;

	public:
		MessageReceiver();
		MessageReceiver(const MessageReceiver& source);

	public:
		virtual ~MessageReceiver();

	public:
		const NEModuleCodeSet& getActions() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key || key.getType() != NEType::NEMODULE_CODESET_KEY)
			{
				ALERT_ERROR("0��Ű�� Ÿ���� NEMODULE_CODESET_KEY���� �մϴ�.");
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
		const NETStringSet& getMessageNames() const
		{
			const NEKey& key = getKeySet()[1];
			if( ! &key || key.getType() != NEType::NETSTRING_SET_KEY)
			{
				ALERT_ERROR("1��Ű�� Ÿ���� NETSTRING_SET_KEY���� �մϴ�.");
				NETStringSet* nullpoint = 0;
				return *nullpoint;
			}

			return (static_cast<const NETStringSetKey&>(key)).getValue();
		}
		NETStringSet& getMessageNames()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NETStringSet&>(casted->getMessageNames());
		}

	public:
		virtual type_result initialize();

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;

	public:
		virtual type_result execute();	

	public:
		type_result sendMessage(const NETString& msg)
		{
			const NETStringSet& names = getMessageNames();
			for(int n=0; n < names.getLength() ;n++)
				if(names[n] == msg)
					return sendMessage(n);

			ALERT_WARNING("��û�� �޼���(%s)�� �����ϴ�.", msg.toCharPointer());
			return RESULT_TYPE_WARNING;
		}
		type_result sendMessage(type_index msg_index)
		{
			NEModuleCodeSet& actions = getActions();
			NEModule& action = actions[msg_index];
			if( ! &action)
			{
				ALERT_ERROR("�ε��� %d�� ������(Length : %d) ������ϴ�.", msg_index, actions.getLength());
				return RESULT_TYPE_ERROR;
			}

			return action.execute();
		}

	public:
		virtual NEObject& clone() const;

	public:
		static const NECodeSet& getReceiverCodeSet()
		{
			static type_code code = Kernal::getInstance().getModuleManager()
				.getModule(NEExportable::Identifier("MessageReceiver", "kniz", 1)).getScriptCode();
			static NECodeSet cs;
			if(cs.getLength() <= 0)
			{				
				cs.create(1);
				cs.setCodeType(NECodeType::SCRIPT);
				cs.push(code);
			}

			return cs;
		}
	};
}
