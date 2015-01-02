//	NEArgumentBase::PurPose ��å:
//		�� 5 ������ �Ǿ��ִ�.
//			UNDEFINED:	
//				���ǵ��� ���� �����̴�. setPurposeLimitation���� ���ǰ� �����ϸ�,
//				�ѹ� ���ǵ� PurposeLimitaion�� ���� ���� �� �� ����.
//				UNDEFINED���� ������ NOT_CONCERNED�� �����ϴ�.
//
//			NOT_CONCERNED:	
//				������� �ʰڴٰ� �����ϴ� ���̴�.
//				
//			READ_BY:
//			WRITTEN:
//				�� ������ �뵵�� ����صд�. ��ⰳ���� ���忡���� �ƹ��� ���� ����, 
//				Editor���α׷��� �����UI�� �̵��� �ִ�.
//				���⼭ ������ ����, READ_BY, WRITTEN�� �������� ��ü�� 
//				System Ȥ�� Module�̶�� ���̴�.
//
//			READ_BY_OR_WRITTEN:
//				����ڴ� ���ڿ� ���� Ű�� �Բ� ��� �뵵�� ���Ǵ��� ��õ� �䱸
//				�Ǿ�����.
#pragma once

#include "../NEKeyNameBinder/NEKeyNameBinder.hpp"

namespace NE
{
	class NEArgumentBaseList;

	class NE_DLL NEArgumentBase : public NEKeyNameBinder
	{
	public:
		friend class NEModule;

	public:
		enum Purpose
		{
			UNDEFINED = 0,
			NOT_CONCERNED,
			READ_OR_WRITTEN,
			READ_BY,
			WRITTEN
		};

	public:
		typedef NEArgumentBase ThisClass;
		typedef NEKeyNameBinder SuperClass;

	public:
		NEArgumentBase(NEType::Type type, Purpose limitation = UNDEFINED);
		NEArgumentBase(const ThisClass& rhs);

	public:
		~NEArgumentBase();

	public:
		bool operator==(const NEArgumentBase& source) const;
		bool operator!=(const NEArgumentBase& source) const;

	public:
		NEType::Type getTypeToBeBinded() const;
		bool isValidToBind(const NEObject& to_be_bind) const;
		const NEKey& getValueKey() const;
		NEKey& getValueKey();
		Purpose getPurpose() const { return _purpose; }
		Purpose getPurposeLimitation() const { return _limitation; }
		type_result setPurpose(Purpose new_purpose) {

			type_result result = RESULT_SUCCESS;
			switch(_limitation)
			{
			case READ_BY:
				new_purpose = READ_BY;
				result = RESULT_TYPE_WARNING | RESULT_ABORT_ACTION;
				break;

			case WRITTEN:
				new_purpose = WRITTEN;
				result = RESULT_TYPE_WARNING | RESULT_ABORT_ACTION;
				break;

			case NOT_CONCERNED:
			case UNDEFINED:
				new_purpose = NOT_CONCERNED;
			}

			_purpose = new_purpose;
			return result;
		}
		type_result setPurposeLimitation(Purpose new_limitation)
		{
			if(_limitation == UNDEFINED)
			{
				_limitation = new_limitation;

				setPurpose(_purpose);

				return RESULT_SUCCESS;
			}

			return RESULT_TYPE_WARNING | RESULT_ABORT_ACTION;
		}

	public:		
		virtual type_result bind();
		virtual NEKey& getDefaultKey() = 0;
		virtual const NEKey& getDefaultKey() const = 0;

	public:
		virtual NEType::Type getType() const;
		virtual type_result isValid() const;
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);

	public:
		bool isNeedingBinding() const;

	private:
		void _release();
		ThisClass& _assign(const ThisClass& source);

	private:
		NEType::Type	_type_validation;
		Purpose			_purpose, 
						_limitation;
	};
}
