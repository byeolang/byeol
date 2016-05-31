#include "DX9.hpp"
#include "define.hpp"

namespace DX9Graphics
{
	DX9::DisplayModeManager::DisplayModeManager()
		: NETList<DisplayMode>()
	{

	}
	type_index DX9::DisplayModeManager::findCorrespondingDisplayWithoutFormat(const DisplayMode& source)
	{
		int index_count = 0;
		for(NETList<DisplayMode>::Iterator* iterator=getIterator(0); iterator ;iterator=iterator->getNext())
		{
			DisplayMode& displaymode = iterator->getValue();
			if(	displaymode.Width == source.Width				&&
				displaymode.Height == source.Height				&&
				displaymode.isWindowed() == source.isWindowed() &&
				displaymode.RefreshRate == source.RefreshRate	)
				return index_count;

			index_count++;
		}

		return NE_INDEX_ERROR;
	}
	type_result DX9::DisplayModeManager::makeDisplayModeAvailable(DisplayMode& target, LPDIRECT3D9 direct)
	{
		/*
			����:
				�ܺο��� ��ȣ�ϴ� ���÷��̰� ���õǴ� ���� �ش��ϴ� ���÷��̸�
				���� �ϵ��� ���� ���ɿ��θ� �Ǵ��� �˸°� ��������� �Ѵ�.
				����, �ϵ���������� �о����� �ʾ��� ��쿡�� �ϵ���� ���� �ʱ�ȭ�� �����Ѵ�.
		*/
		//	pre:
		//		parameter�� Window ��� �ΰ�: ������ ����� �ٷ� ���� �Ҵ��Ѵ�
		if(target.isWindowed())
		{
			target.Format = D3DFMT_UNKNOWN;
			target.RefreshRate = 0;	//	������ ��忡���� RefreshRate�� �����ؼ��� �ȵȴ�.
			return RESULT_SUCCESS;
		}
		//		parameter Valid üũ:
		if(NEResult::hasError(target.isValid())) return RESULT_TYPE_ERROR;
		//		�ϵ���� ������ �ѹ� �ʱ�ȭ �Ǿ��°�:
		if(getLength() <= 0) // : �׷��� �ϵ������ �ּ��� 1�� �̻��� ��带 �����ϰ� ���� ���̹Ƿ� �̰� ����ؼ� "�ѹ� �о�����" ���θ� �Ǵ��� �� �ִ�.
			initialize(direct); // : �ϵ���� ���� �б�



		//	main:
		//		������ �������� �����Ѵ�.
		//		�ϵ���� ������ �־��� display ��尡 ���ٸ�:
		if(find(target) == NE_INDEX_ERROR)
		{
			int index = findCorrespondingDisplayWithoutFormat(target);

			if(index == NE_INDEX_ERROR)
			{
				ALERT_WARNING_IN_SPECIFIED_MODULE(NEExportable::Identifier(NETString(_NAME), NETString(_DEVELOPER), _REVISION), "�ػ� %dx%d %dhz�� �����ϴ� �� � ���˵� �߰����� ���߽��ϴ�. ������ �Էµ� ������ �����ɰ̴ϴ�.", target.Width, target.Height, target.RefreshRate)

				target.Format = D3DFMT_UNKNOWN;

				return RESULT_TYPE_ERROR;
			}
			else
			{
				ALERT_INFORMATION_IN_SPECIFIED_MODULE(NEExportable::Identifier(NETString(_NAME), NETString(_DEVELOPER), _REVISION), "������ ��� %dx%d %dhz Format=%d �� �������� �ʽ��ϴ�. format�� �����ϰ� ��ġ�ϴ� ���� �˻��մϴ�.", target.Width, target.Height, target.RefreshRate, target.Format);

				target.Format = getElement(index).Format;
			}
		}
		
		return RESULT_SUCCESS;	
	}

	type_result DX9::DisplayModeManager::initialize(LPDIRECT3D9 direct)
	{
		release();

		return _pushAvailableDisplayMode(direct);
	}

	//	�׷��� �ϵ��� �����ϴ� ����� ������ ���Ѵ�
	type_result DX9::DisplayModeManager::_pushAvailableDisplayMode(LPDIRECT3D9 direct)
	{
		if( ! direct) return RESULT_TYPE_ERROR;

		
		
		//	main:
		int	count = 0,
			iter = 0;

		for(int n=D3DFMT_R8G8B8; n <= D3DFMT_X8B8G8R8 ;n++)
			__enumerateOneDisplayMode(direct, D3DFORMAT(n)); 



		//	post:
		return RESULT_SUCCESS;
	}
	type_result DX9::DisplayModeManager::__enumerateOneDisplayMode(LPDIRECT3D9 direct, D3DFORMAT format)
	{
		int count = direct->GetAdapterModeCount(D3DADAPTER_DEFAULT, format); // D3DFMT_R5G6B5�� ����
		
		for (int n = 0; n < count ;n++)
		{
			DisplayMode buffer;
			buffer.setWindowed(false);

			direct->EnumAdapterModes(D3DADAPTER_DEFAULT, format, n, &(buffer));

			push(buffer);
		}

		return RESULT_SUCCESS;
	}
}