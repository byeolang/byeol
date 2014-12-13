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
#pragma once

#include "../../../Modules/NEObject/NEObject.hpp"
#include "../../Enums/NECodeType/NECodeType.hpp"
#include "../../Interfaces/NEScriptHeader/NEScriptHeader.hpp"

namespace NE
{
	class NE_DLL NECode : public NEObject
	{
	public:
		typedef NECode ThisClass;
		typedef NEObject SuperClass;

	public:
		NECode(type_code new_code = NE_INDEX_ERROR);
		NECode(NECodeType::CodeType new_codetype, type_code new_code);
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
		NECodeType::CodeType getCodeType() const;
		type_result setCodeType(NECodeType::CodeType new_codetype);
		type_code getCode() const;
		type_result setCode(type_code new_code);
		type_result setIfValid(const ThisClass& source);

	public:
		virtual void release();
		virtual NEObject& clone() const;
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;
		virtual NEType::Type getType() const;

	public:		
		bool isAcceptable(NECodeType::CodeType codetype) const;

	private:
		ThisClass& _assign(const ThisClass& source);
		ThisClass _operateWhenAcceptableByValue(const ThisClass& source, type_code new_code) const;
		ThisClass& _operateWhenAcceptableByReference(const ThisClass& source, type_code new_code);
		type_code _getMaxCode() const;
		NEBinaryFileSaver& _serializeAsScript(NEBinaryFileSaver& saver) const;
		NEBinaryFileLoader& _serializeAsScript(NEBinaryFileLoader& loader);

	private:
		NECodeType::CodeType _codetype;
		type_code _code;

	private:
		typedef const NEScriptHeader& (*GET_SCRIPTHEADER_INTERFACE)();
		static GET_SCRIPTHEADER_INTERFACE _interface;
		friend class Editor;
	};
}