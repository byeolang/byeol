//	---------------------------------------------------------------------------------
//	Ŭ������:	NESwitchableUnit
//	����	:	�������̽� ��Ÿ����.
//				�ٸ� ���� ������ �����ϴ� ��ü�� �������̽��� ����ؾ� �Ѵ�.
//	����	:	��� Ŭ����.	NEUnit
//	Ư��	:	�߻�Ŭ����
//	�˰���:	
//	�����:	execute()�� Ŭ������ ����ϰ� �ִ� ���� ����� �����Ų��.
//				enable(), disable()�� �������̽��� ����� ON / OFF �� �� �ִ�.
//	�޸�	:	
//	�����丮:	2011-07-07	������	���� �Ϸ�	2011-06-05	������	���� �Ϸ�.
//	---------------------------------------------------------------------------------
#pragma once

//	include:
#include "../NEUnit/NEUnit.hpp"

namespace NE
{
	class NE_DLL NESwitchableUnit : public NEUnit
	{
		NE_DECLARE_INTERFACE(NESwitchableUnit, NEUnit)

		//	������:
	public:
		NESwitchableUnit();
		NESwitchableUnit(const NESwitchableUnit& source);

		//	�Ҹ���:
	public:
		virtual ~NESwitchableUnit();

		//	������ ��ø:
	public:
		const NESwitchableUnit& operator=(const NESwitchableUnit& source);
		type_bool operator==(const NESwitchableUnit& source) const;
		type_bool operator!=(const NESwitchableUnit& source) const;

		//	������:
	public:
		type_bool isEnable() const;
		void setEnable(type_bool enable);

		//	�������̽�:
		//		���:
		//			NEUnit:
	public:
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//	�����Լ�:
	private:
		void _release();
		const NESwitchableUnit& _assign(const NESwitchableUnit& source);

		//	�������:
	private:
		type_bool _enable;
	};
}