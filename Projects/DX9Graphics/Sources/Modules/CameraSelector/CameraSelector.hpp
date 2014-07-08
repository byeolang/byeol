#pragma once

#include "../../Includes/Includes.hpp"

namespace DX9Graphics
{
	class NE_DLL CameraSelector : public NEModule
	{
	public:
		typedef NEModule SuperClass;
		typedef CameraSelector ThisClass;

	public:
		virtual type_result initialize()
		{
			NEKeyCodeSet& keyset = getKeySet();
			keyset.create(1);

			return keyset.push(NEModuleSelector());
		}

	public:
		const NEModuleSelector& getCameraSelector() const
		{
			const NEKey& key = getKeySet()[0];
			if( ! &key	||	! key.isSubClassOf(NEType::NEMODULE_SELECTOR))
			{
				ALERT_ERROR("1��°Ű�� �߸��Ǿ����ϴ�. 1��° Ű�� NEMODULE_SELECTOR ���� �մϴ�.");
				NEModuleSelector* nullpoint=0;
				return *nullpoint;
			}

			return static_cast<const NEModuleSelector&>(key);			
		}
		NEModuleSelector& getCameraSelector()
		{
			const ThisClass* casted = (const ThisClass*) this;

			return const_cast<NEModuleSelector&>(casted->getCameraSelector());
		}
	};
}