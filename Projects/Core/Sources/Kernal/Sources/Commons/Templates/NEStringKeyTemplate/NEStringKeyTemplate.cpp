#pragma warning(push)
#pragma warning(disable : 4800)
#pragma warning(disable : 4804)
#pragma warning(disable : 4146)	//	unsigned�� -�� �����
#include "NEStringKeyTemplate.hpp"
#include "../../../Modules/NEBooleanKey/NEBooleanKey.hpp"
#include "../../../Modules/NECharKey/NECharKey.hpp"
#include "../../../Modules/NEByteKey/NEByteKey.hpp"
#include "../../../Modules/NEUByteKey/NEUByteKey.hpp"
#include "../../../Modules/NEWCharKey/NEWCharKey.hpp"
#include "../../../Modules/NEShortKey/NEShortKey.hpp"
#include "../../../Modules/NEUShortKey/NEUShortKey.hpp"
#include "../../../Modules/NEIntKey/NEIntKey.hpp"
#include "../../../Modules/NEUIntKey/NEUIntKey.hpp"
#include "../../../Modules/NEInt64Key/NEInt64Key.hpp"
#include "../../../Modules/NEFloatKey/NEFloatKey.hpp"
#include "../../../Modules/NEDoubleKey/NEDoubleKey.hpp"
#include "../../../Modules/NEStringKey/NEStringKey.hpp"
#include "../../../Modules/NEWStringKey/NEWStringKey.hpp"

namespace NE
{
	template <>
	type_result NE_DLL NEStringKeyTemplate<NEString, NEType::NESTRING_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;


		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = (static_cast<const NECharKey&>(source)).getValue();
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEUByteKey&>(source)).getValue());
			break;
		case NEType::NEWCHAR_KEY:
			_value = (static_cast<const NEWCharKey&>(source)).getValue();
			break;
		case NEType::NESHORT_KEY:			
			_value = static_cast<type_int>((static_cast<const NEShortKey&>(source)).getValue());
			break;
		case NEType::NEUSHORT_KEY:
			_value = (static_cast<const NEUShortKey&>(source)).getValue();;
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEDOUBLE_KEY:
			_value = (static_cast<const NEDoubleKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_int>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue();
			break;
		}

		return RESULT_SUCCESS;
	}
	template <>
	type_result NE_DLL NEStringKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::assign(const NEKey& source)
	{
		//	pre:
		if( ! &source) return RESULT_TYPE_ERROR;
		if( this == &source) return RESULT_TYPE_ERROR;


		SuperClass::assign(source);

		//	main:
		switch(source.getType())
		{
		case NEType::NEBOOLEAN_KEY:
			_value = (static_cast<const NEBooleanKey&>(source)).getValue();
			break;
		case NEType::NECHAR_KEY:
			_value = (static_cast<const NECharKey&>(source)).getValue();
			break;
		case NEType::NEBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEByteKey&>(source)).getValue());
			break;
		case NEType::NEUBYTE_KEY:
			_value = static_cast<type_int>((static_cast<const NEUByteKey&>(source)).getValue());
			break;
		case NEType::NEWCHAR_KEY:
			_value = (static_cast<const NEWCharKey&>(source)).getValue();
			break;
		case NEType::NESHORT_KEY:
			_value = (static_cast<const NEShortKey&>(source)).getValue();
			break;
		case NEType::NEUSHORT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUShortKey&>(source)).getValue());
			break;
		case NEType::NEINT_KEY:
			_value = (static_cast<const NEIntKey&>(source)).getValue();
			break;
		case NEType::NEUINT_KEY:
			_value = static_cast<type_int>((static_cast<const NEUIntKey&>(source)).getValue());
			break;
		case NEType::NEFLOAT_KEY:			
			_value = (static_cast<const NEFloatKey&>(source)).getValue();
			break;
		case NEType::NEDOUBLE_KEY:
			_value = (static_cast<const NEDoubleKey&>(source)).getValue();
			break;
		case NEType::NEINT64_KEY:
			_value = static_cast<type_int>((static_cast<const NEInt64Key&>(source)).getValue());
			break;
		case NEType::NESTRING_KEY:
			_value = (static_cast<const NEStringKey&>(source)).getValue();
			break;
		case NEType::NEWSTRING_KEY:
			_value = (static_cast<const NEWStringKey&>(source)).getValue();
			break;
		}

		return RESULT_SUCCESS;
	}

	template <>
	bool NEStringKeyTemplate<NEString, NEType::NESTRING_KEY>::isLower(const NEKey& source) const
	{
		//	pre:
		if( ! &source) return false;



		//	main:
		//		���� ����:
		ThisClass buffer;
		//		���ۿ� �Ҵ�:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;
		


		//	post:
		//		���� ��:
		//			Ÿ����:
		const NEString	&source_string = getValue(),
						&target_string = buffer.getValue();		
		//			���� ����ó��:
		if(target_string.getLength() <= 0) return false;
		type_index length_of_source = getValue().getLength();
		//			����:
		for(int n=0; n < length_of_source ;n++)
		{
			//			����ó��:
			/*
							�񱳴��(= Ÿ��)�� ���̰� ������쿡��, Ž���� �ߴ��ϰ� ����
							������ ����� true�����Ƿ� "true"�� ��ȯ�Ѵ�.

							�׷��� Ÿ���� ���̰� 0�̶��, for���� ������ ���� ����ó���� 
							���� false�� ��ȯ�� ���̴�.
			*/
			if(n > target_string.getLengthLastIndex()) break;

			//			���� ��:
			//				���� ����:
			char	source_character = source_string[n],
					target_character = target_string[n];
			//				��:
			/*
								���������� ������ �ΰ��� ���ڸ� ���ؼ� "���� >= �񱳴��"�� 
								�Ǵ�, �������� false�� ��ȯ�Ѵ�.
			*/
			if(source_character >= target_character)
				return false;
		}

		return true;
	}

	template <>
	bool NEStringKeyTemplate<NEWString, NEType::NEWSTRING_KEY>::isLower(const NEKey& source) const
	{
		//	pre:
		if( ! &source) return false;



		//	main:
		//		���� ����:
		ThisClass buffer;
		//		���ۿ� �Ҵ�:
		type_result result = buffer.assign(source);
		if(NEResult::hasError(result))
			return false;
		


		//	post:
		//		���� ��:
		//			Ÿ����:
		const NEWString	&source_string = getValue(),
						&target_string = buffer.getValue();		
		//			���� ����ó��:
		if(target_string.getLength() <= 0) return false;
		type_index length_of_source = getValue().getLength();
		//			����:
		for(int n=0; n < length_of_source ;n++)
		{
			//			����ó��:
			/*
							�񱳴��(= Ÿ��)�� ���̰� ������쿡��, Ž���� �ߴ��ϰ� ����
							������ ����� true�����Ƿ� "true"�� ��ȯ�Ѵ�.

							�׷��� Ÿ���� ���̰� 0�̶��, for���� ������ ���� ����ó���� 
							���� false�� ��ȯ�� ���̴�.
			*/
			if(n > target_string.getLengthLastIndex()) break;

			//			���� ��:
			//				���� ����:
			wchar_t	source_character = source_string[n],
					target_character = target_string[n];
			//				��:
			/*
								���������� ������ �ΰ��� ���ڸ� ���ؼ� "���� >= �񱳴��"�� 
								�Ǵ�, �������� false�� ��ȯ�Ѵ�.
			*/
			if(source_character >= target_character)
				return false;
		}

		return true;
	}
}

#pragma warning(pop)