#pragma once;

#include "../Texture/Texture.hpp"

namespace DX9Graphics
{
	class NE_DLL TabledTexture : public Texture
	{
	public:
		typedef Texture SuperClass;
		typedef TabledTexture ThisClass;

	public:
		NETArgument<NEIntKey>		arg_max_row;
		NETArgument<NEIntKey>		arg_max_column;
		NETArgument<NEIntKey>		arg_delay_per_frame;
		NETArgument<NEIntSetKey>	arg_column_count_per_row;

	protected:
		virtual type_result _onFetchModule()
		{
			SuperClass::_onFetchModule();

			arg_max_row.setValue(1);
			arg_max_row.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_max_column.setValue(1);
			arg_max_column.setPurposeLimitation(NEArgumentBase::READ_BY);
			arg_delay_per_frame.setEnable(false);
			arg_delay_per_frame.setPurposeLimitation(NEArgumentBase::READ_BY);

			NEIntSet sample(1);
			sample.push(1);
			arg_column_count_per_row.getValue() = sample;
			arg_column_count_per_row.setEnable(false);
			arg_column_count_per_row.setPurposeLimitation(NEArgumentBase::READ_BY);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			SuperClass::_onFetchArguments(tray);

			tray.push(arg_max_row);
			tray.push(arg_max_column);
			tray.push(arg_delay_per_frame);
			tray.push(arg_column_count_per_row);

			return RESULT_SUCCESS;
		}

	public:
		TabledTexture()
			: SuperClass()
		{
			_release();
		}
		TabledTexture(const ThisClass& source)
			: SuperClass(source)
		{
			_assign(source);
		}
		~TabledTexture()
		{

		}

	public:
		const ThisClass& operator=(const ThisClass& source)
		{
			SuperClass::operator=(source);

			return _assign(source);
		}
		bool operator==(const ThisClass& source) const
		{
			if(SuperClass::operator!=(source)) return false;
			if(_column_extracted_from_model != source._column_extracted_from_model) return false;
			if(_row_extracted_from_model != source._row_extracted_from_model) return false;

			return true;
		}
		bool operator!=(const ThisClass& source) const
		{
			return ! operator==(source);
		}

	public:
		type_int getRowExtractedFromModel() const { return _row_extracted_from_model; }
		type_int getColumnExtractedFromModel() const { return _column_extracted_from_model; }		
		RECT createTexelPosition(type_int animation_index, type_int frame_index) const
		{
			RECT to_return = {0, };
			if(_isFrameIndexOverflowed(animation_index, frame_index))
			{
				ALERT_WARNING("������ animation_index, frame_index�� ColumnTable ������ �Ѿ���ϴ�:\n���Ƿ� (U, V)=(0, 0)�� �������ϴ�.\n\t�Է��� (animation, frame)=(%d, %d)", animation_index, frame_index);
				to_return.left = 0;
				to_return.top = 0;
				to_return.right = getWidth();
				to_return.bottom = getHeight();
				return to_return;
			}



			//	main:
			type_float	width_per_1_frame		= static_cast<type_float>(getWidth()) / arg_max_column.getValue(),
				height_per_1_animation	= static_cast<type_float>(getHeight()) / arg_max_row.getValue();
			to_return.left = static_cast<LONG>(frame_index * width_per_1_frame);
			to_return.top = static_cast<LONG>(animation_index * height_per_1_animation);
			to_return.right = static_cast<LONG>((frame_index+1) * width_per_1_frame);
			to_return.bottom = static_cast<LONG>((animation_index+1) * height_per_1_animation);

			return to_return;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const
		{
			static NEExportable::ModuleHeader _header;

			if (_header.isValid() != RESULT_NOTHING)
			{
				const NEExportable::ModuleHeader& supers = SuperClass::getHeader();
				_header.getName() = "TabledTexture";
				_header.getDeveloper() = "kniz";
				_header.setRevision(1);
				_header.getComment() = supers.getComment() +
					"TabledTexture�� �̷� ������ Texture ��ɿ�, Animation�� ����� �����մϴ�.\n"
					"TabledTexture�� �������� Animation���� �̷���� �ְ�, �̴� ��(row)���� ���е˴ϴ�."
					"�� Animation�� �������� KeyFrame ��� �̷���� �ְ� �̴� �� ���� ��(column)���� ���е˴ϴ�.\n"
					"�������, 2��° Animation�� 5��° KeyFrame�� �׸��� ������ ���� �������ϴ�.\n"
					"�־��� �׸�(.bmp, .jpg ��)�� ��� ���� �ִ���(row_cnt), �ִ� ��� ���� �ִ���(column_cnt)"
					"�� ���� ��, �׸��� �� ��� ���� ���ڷ� ǥ(table)ó�� �����ϴ�.\n"
					"�׸��� 2��°���� 5��°�� ������ Texture�� ����ϰ� �˴ϴ�.";					
				_header.getVersion() = "0.0.1a";
				_header.getReleaseDate() = "2013-08-10";
				NETStringSet& args = _header.getArgumentsComments();
				args = supers.getArgumentsComments();
				args.resize(args.getLength() + 3);
				args.push("MaxRow\nTexture�� ǥ�� ������ �������� ������ ��� ������ �����մϴ�.");
				args.push("MaxColumn\nTexture�� ǥ�� ������ �������� ������ �� ���� ������ �����մϴ�.");
				args.push("DelayPerFrame\nAnimation�� �� KeyFrame�� �Ѿ�� �ɸ��� �ð��Դϴ�.\n-1 Ȥ�� disabled�� ������ ���Ѵ� �Դϴ�.");
				args.push("Customized Key Frame\nAnimation ���� KeyFrame�� �ٸ� ���, �� Animation�� ���� KeyFrame ���� �Է��մϴ�.\nEnable�϶��� ����մϴ�.");
			}

			return _header;
		}
		virtual type_uint getWidthOfOneFrame() const
		{
			type_count max = arg_max_column.getValue();
			if(arg_column_count_per_row.isEnable())
			{
				const NEIntSet& set = arg_column_count_per_row.getValue();
				for (int n = 0; n < set.getLength(); n++)
					if(set[n] > max)
						max = set[n];
			}

			return getWidth() / max;
		}
		virtual type_uint getHeightOfOneFrame() const
		{
			type_count max = arg_max_row.getValue();

			if(arg_column_count_per_row.isEnable())
			{
				const NEIntSet& set = arg_column_count_per_row.getValue();

				max = set.getLength() > max ? set.getLength() : max;
			}

			return getHeight() / max;
		}
		virtual type_result dock(Model& model);
		virtual RECT createSourceRect() const 
		{
			return createTexelPosition(_row_extracted_from_model, _column_extracted_from_model);
		}
		virtual NEObject& clone() const
		{
			return *(new ThisClass(*this));
		}
		virtual type_result isValid() const
		{
			//	pre:
			type_result result = SuperClass::isValid();
			if(NEResult::hasError(result)) return result;
			//		������� Row�� Col�� üũ:
			const NEIntSet& columns = arg_column_count_per_row.getValue();
			type_int	max_column = arg_max_column.getValue(),
				max_row = arg_max_row.getValue();
			if(columns.getSize() > (type_int) max_row)
				return ALERT_ERROR("������� Row�� ���� �ִ� Row Count�� �Ѿ����ϴ�.");

			for(int n=0; n < columns.getSize() ;n++)
				if(columns[n] < 0 || columns[n] > max_column)
					return ALERT_ERROR("������� Column�� ���� �ִ� Column Count�� �Ѿ����ϴ�.");

			return RESULT_SUCCESS;
		}
		virtual void release()
		{
			SuperClass::release();

			_release();
		}

	private:
		bool _isFrameIndexOverflowed(type_int animation_index, type_int frame_index) const
		{
			const NEIntSet& table = arg_column_count_per_row.getValue();
			if(animation_index < 0 || animation_index > static_cast<type_int>(table.getSizeLastIndex())) return true;
			type_int max_frame_index = table[animation_index] - 1;
			if(frame_index < 0 || frame_index > max_frame_index)	return true;

			return false;
		}
		ThisClass& _assign(const ThisClass& source)
		{
			if(this == &source) return *this;

			_row_extracted_from_model = source._row_extracted_from_model;
			_column_extracted_from_model = source._column_extracted_from_model;
			arg_max_row = source.arg_max_row;
			arg_max_column = source.arg_max_column;
			arg_delay_per_frame = source.arg_delay_per_frame;
			arg_column_count_per_row = source.arg_column_count_per_row;

			return *this;
		}
		void _release()
		{
			_row_extracted_from_model = 0;
			_column_extracted_from_model = 0;
		}
		void _validate()
		{
			if(arg_max_row.getValue() <= 0)
				arg_max_row.setValue(1);
			if(arg_max_column.getValue() <= 0)
				arg_max_column.setValue(1);
		}

	private:
		type_int _row_extracted_from_model;
		type_int _column_extracted_from_model;
	};
}