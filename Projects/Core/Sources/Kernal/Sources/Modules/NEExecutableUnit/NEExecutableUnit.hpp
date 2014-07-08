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
#include "../NEUnit/NEUnit.hpp"
#include "../../Commons/Interfaces/NEExecutable/NEExcutable.hpp"

namespace NE
{
	class NE_DLL NEExecutableUnit : public NEUnit, public NEExecutable
	{
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
		bool operator==(const NEExecutableUnit& source) const;
		bool operator!=(const NEExecutableUnit& source) const;

		//	������:
	public:
		bool isEnable() const;
		void setEnable(bool enable);

		//	�������̽�:
		//		���:
		//			NEUnit:
	public:
		virtual NEType::Type getType() const;
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

		//		���� �������̽�:
	public:
		virtual type_result execute() = 0;

		//	�����Լ�:
	private:
		void _release();
		const NEExecutableUnit& _assign(const NEExecutableUnit& source);

		//	�������:
	protected:
		bool _enable;
	};
}