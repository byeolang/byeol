//	UNDEFINED:
//		�ڵ��� Ÿ���� ���ǵ��� �ʾҴ�. ���� Target�� CodeType�� ���󰣴�.
//		void* Ȥ�� var �͵� ���� �Ŷ� ���� �ȴ�. � CodeType���ε� ��޵ȴ�.
//
//	setCode(getCode() + rhs.getCode())�� operator+(rhs)�� ����:
//		-	setCode�� Code�� ���� ���ռ��� ������(NEScriptHeader�� ��õ� max_code��
//			���ռ� ���θ� �����Ѵ�)
//		-	operator+�� CodeType�� ���� ���ռ� ���θ� �����ϰ�, setCode�� ȣ��
//			�Ѵ�.
//
//		�ٽø��ؼ�, operator+(rhs)�� �ϰԵǸ�, �־��� rhs�� ������ CodeType����
//		�˻��ϸ�, �ƴ� ��� �۾��� ����Ѵ�.
//		�ݸ�, setCode�� �ϴ� ���� rhs�� � Ÿ�������� ������� �ʰڴٴ� �� 
//		�ǹ��Ѵ�. ��, operator+(NECode(NEcodeType::UNDEFINED)) �� ���� ��.
//
//	FixedType�� ���� ���� ���̺�:
//		This = Source�� ������ ���,
//		+===================================================+
//		|	This \ Source	|	Flexible	|	Fixed		|
//		|====================================================
//		|	Flexible		|	Source's	|	Source's	|
//		|---------------------------------------------------|
//		|	Fixed			|	Mine		|	Mine		|
//		|====================================================
#pragma once

#include "../NECodeType/NECodeType.hpp"
#include "../../Interfaces/NEExportable/NEExportable.hpp"

namespace NE
{
	class NE_DLL NECode : public NECodeType
	{
	public:
		typedef NECode ThisClass;
		typedef NECodeType SuperClass;

	public:
		NECode();
		NECode(const NEExportable::Identifier& identifier);
		NECode(NECodeType::CodeType codetype_to_be_fixed);
		NECode(type_code new_code);
		NECode(type_code new_code, NECodeType::CodeType new_codetype, bool is_type_fixed = true);
		NECode(const ThisClass& source);

	public:
		virtual ~NECode();

	public:
		ThisClass& operator=(const ThisClass& source);
		NECode operator++();
		NECode& operator++(int);
		NECode operator--();
		NECode& operator--(int);
		bool operator==(const ThisClass& source) const;
		bool operator!=(const ThisClass& source) const;
		bool operator<(const ThisClass& source) const;
		bool operator<=(const ThisClass& source) const;
		bool operator>(const ThisClass& source) const;
		bool operator>=(const ThisClass& source) const;
		ThisClass operator+(const ThisClass& source) const;
		ThisClass operator-(const ThisClass& source) const;
		ThisClass operator*(const ThisClass& source) const;
		ThisClass operator/(const ThisClass& source) const;
		ThisClass operator%(const ThisClass& source) const;
		ThisClass& operator+=(const ThisClass& source);
		ThisClass& operator-=(const ThisClass& source);
		ThisClass& operator*=(const ThisClass& source);
		ThisClass& operator/=(const ThisClass& source);		
		ThisClass& operator%=(const ThisClass& source);
		operator type_code();

	public:
		type_code getCode() const;
		type_result setCode(type_code new_code);
		type_result setCode(const NEExportable::Identifier& identifier);

	public:
		virtual void release();
		virtual NEObject& clone() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;
		virtual NEType::Type getType() const;

	private:
		ThisClass& _assign(const ThisClass& source);
		NEBinaryFileSaver& _serializeAsScript(NEBinaryFileSaver& saver) const;
		NEBinaryFileLoader& _serializeAsScript(NEBinaryFileLoader& loader);
		void _release();
		ThisClass _createCode(const NECode& source, type_code new_code) const;

	private:
		type_code _code;
	};
}