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
#include "../NEIdentifier/NEIdentifier.hpp"

namespace NE
{
	class NE_DLL NECode : public NECodeType
	{
		NE_DECLARE_CLASS(NECode, NECodeType)

	public:
		NECode();
		NECode(const NEIdentifier& identifier);
		NECode(type_code new_code, const NECodeType& codetype = NECodeType());
		NECode(const This& source);

	public:
		virtual ~NECode();

	public:
		This& operator=(const This& source);
		NECode& operator++();
		NECode operator++(int);
		NECode& operator--();
		NECode operator--(int);
		type_bool operator==(const This& source) const;
		type_bool operator==(const type_code source) const;
		type_bool operator!=(const type_code source) const;
		type_bool operator!=(const This& source) const;
		type_bool operator<(const This& source) const;
		type_bool operator<=(const This& source) const;
		type_bool operator>(const This& source) const;
		type_bool operator>=(const This& source) const;
		This operator+(const This& source) const;
		This operator-(const This& source) const;
		This operator*(const This& source) const;
		This operator/(const This& source) const;
		This operator%(const This& source) const;
		This& operator+=(const This& source);
		This& operator-=(const This& source);
		This& operator*=(const This& source);
		This& operator/=(const This& source);		
		This& operator%=(const This& source);
		operator type_code();

	public:
		type_code getCode() const;
		type_result setCode(type_code new_code);
		type_result setCode(const NEIdentifier& identifier);

	public:
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;

	private:
		This& _assign(const This& source);
		NEBinaryFileSaver& _serializeAsScript(NEBinaryFileSaver& saver) const;
		NEBinaryFileLoader& _serializeAsScript(NEBinaryFileLoader& loader);
		void _release();
		This _createCode(const NECode& source, type_code new_code) const;

	private:
		type_code _code;
	};
}