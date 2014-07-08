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
		type_int& getMaxRow()
		{
			const ThisClass* consted_this = this;

			return const_cast<type_int&>(consted_this->getMaxRow() );
		}
		const type_int& getMaxRow() const
		{
			const NEKey& somekey = getKeySet()[6];
			if( ! somekey.isSubClassOf(NEType::NEINT_KEY))
			{
				ALERT_ERROR("Ű�� �迭�� �̻��մϴ�. 6��Ű�� NEIntKey���� �մϴ�.");
				const type_int* nullpoint = 0;
				return *nullpoint;
			}

			const NEIntKey& target = static_cast<const NEIntKey&>(somekey);
			return target.getValue();
		}
		type_int& getMaxColumn()
		{
			const ThisClass* consted_this = this;

			return const_cast<type_int&>(consted_this->getMaxColumn() );
		}
		const type_int& getMaxColumn() const
		{
			const NEKey& somekey = getKeySet()[7];
			if( ! somekey.isSubClassOf(NEType::NEINT_KEY))
			{
				ALERT_ERROR("Ű�� �迭�� �̻��մϴ�. 7��Ű�� NEIntKey���� �մϴ�.");
				const type_int* nullpoint = 0;
				return *nullpoint;
			}

			const NEIntKey& target = static_cast<const NEIntKey&>(somekey);
			return target.getValue();
		}
		type_int& getDelayPerFrame()
		{
			const ThisClass* consted_this = this;

			return const_cast<type_int&>(consted_this->getDelayPerFrame() );
		}
		const type_int& getDelayPerFrame() const
		{
			const NEKey& somekey = getKeySet()[8];
			if( ! somekey.isSubClassOf(NEType::NEINT_KEY))
			{
				ALERT_ERROR("Ű�� �迭�� �̻��մϴ�. 8��Ű�� NEIntKey���� �մϴ�.");
				const type_int* nullpoint = 0;
				return *nullpoint;
			}

			const NEIntKey& target = static_cast<const NEIntKey&>(somekey);
			return target.getValue();
		}
		NEIntSet& getColumnCountPerRow()
		{
			const ThisClass* consted_this = this;

			return const_cast<NEIntSet&>(consted_this->getColumnCountPerRow() );
		}
		const NEIntSet& getColumnCountPerRow() const
		{
			const NEKey& somekey = getKeySet()[9];
			if( ! somekey.isSubClassOf(NEType::NEINT_SET_KEY))
			{
				ALERT_ERROR("Ű�� �迭�� �̻��մϴ�. 9��Ű�� NEIntSetKey���� �մϴ�.");
#pragma warning(push)
#pragma warning(disable: 4172)
				type_int* nullpoint = 0;
				return *nullpoint;
#pragma warning(pop)
			}

			const NEIntSetKey& target = static_cast<const NEIntSetKey&>(somekey);
			return target.getValue();
		}
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
			type_float	width_per_1_frame		= static_cast<type_float>(getWidth()) / getMaxColumn(),
						height_per_1_animation	= static_cast<type_float>(getHeight()) / getMaxRow();
			to_return.left = static_cast<LONG>(frame_index * width_per_1_frame);
			to_return.top = static_cast<LONG>(animation_index * height_per_1_animation);
			to_return.right = static_cast<LONG>((frame_index+1) * width_per_1_frame);
			to_return.bottom = static_cast<LONG>((animation_index+1) * height_per_1_animation);
			
			return to_return;
		}

	public:
		virtual type_uint getWidthOfOneFrame() const
		{
			return getWidth() / getMaxColumn();
		}
		virtual type_uint getHeightOfOneFrame() const
		{
			return getHeight() / getMaxRow();
		}
		virtual type_result dock(Model& model);
		virtual RECT createSourceRect() const 
		{
			return createTexelPosition(_row_extracted_from_model, _column_extracted_from_model);
		}
		virtual type_result initialize()
		{
			SuperClass::initialize();

			NEKeyCodeSet& keyset = getKeySet();
			keyset.resize(keyset.getLength() + 4);						
			keyset.push(NEIntKey(1));			//	3:	�ؽ��� ������ ��� Row�� ���������°�
			keyset.push(NEIntKey(1));			//	4:	�ؽ��� ������ ��� col�� ���������°�
			keyset.push(NEIntKey(0));			//	5:	�ϳ��� �����Ӵ� �ʿ���ϴ� Delay. -1�� ����.
			keyset.push(NEIntSetKey());		//	6:	[row] = ������� �ִ� col(row��0�̸� �Ϲ� Texture�� ���·� ���)

			return RESULT_SUCCESS;
		}
		virtual const NEExportable::ModuleHeader& getHeader() const;	
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
			const NEIntSet& columns = getColumnCountPerRow();
			type_int	max_column = getMaxColumn(),
						max_row = getMaxRow();
			if(columns.getSize() > (type_int) max_row)
			{
				ALERT_ERROR("������� Row�� ���� �ִ� Row Count�� �Ѿ����ϴ�.");
				return RESULT_TYPE_ERROR;
			}
			for(int n=0; n < columns.getSize() ;n++)
				if(columns[n] < 0 || columns[n] > max_column)
				{
					ALERT_ERROR("������� Column�� ���� �ִ� Column Count�� �Ѿ����ϴ�.");
					return RESULT_TYPE_ERROR;
				}

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
			const NEIntSet& table = getColumnCountPerRow();
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

			return *this;
		}
		void _release()
		{
			_row_extracted_from_model = 0;
			_column_extracted_from_model = 0;
		}

	private:
		type_int _row_extracted_from_model;
		type_int _column_extracted_from_model;
	};
}