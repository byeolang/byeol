//	---------------------------------------------------------------------------------
//	Ŭ������:	NEExecutableUnit
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
#include "../NESwitchableUnit/NESwitchableUnit.hpp"
#include "../../Commons/Interfaces/NEExecutable/NEExcutable.hpp"

namespace NE
{
	class NE_DLL NEExecutableUnit : public NESwitchableUnit, public NEExecutable
	{
		NE_DECLARE_INTERFACE(NEExecutableUnit, NESwitchableUnit)

		//	������:
	public:
		NEExecutableUnit();
		NEExecutableUnit(const NEExecutableUnit& source);

		//	�Ҹ���:
	public:
		virtual ~NEExecutableUnit();

		//	������ ��ø:
	public:
		const NEExecutableUnit& operator=(const NEExecutableUnit& source);

		//	�������̽�:
		//		���� �������̽�:
	public:
		virtual type_result execute() = 0;
	};
}