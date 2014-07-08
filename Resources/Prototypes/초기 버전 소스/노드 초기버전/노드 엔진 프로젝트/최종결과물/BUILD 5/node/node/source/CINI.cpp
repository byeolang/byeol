#include "CINI.hpp"

//	������
CINI::CINI() 
: _graphic_window_mode(DEFAULT_CINI_GRAPHIC_MODE), _graphic_height(DEFAULT_CINI_GRAPHIC_HEIGHT),
_graphic_width(DEFAULT_CINI_GRAPHIC_WIDTH), _graphic_ratio(DEFAULT_CINI_GRAPHIC_RATIO), 
_graphic_vertical_synchronize(DEFAULT_CINI_GRAPHIC_VERTICAL_SYNCHRONIZE),
_graphic_32colorbit(DEFAULT_CINI_GRAPHIC_32COLORBIT), _grpahic_32bit_texture(DEFAULT_CINI_GRAPHIC_32BITTEXTURE),
_graphic_isShaderOn(DEFAULT_CINI_GRAPHIC_IS_SHADER_ON), 
_graphic_shader_quality(DEFAULT_CINI_GRAPHIC_SHADER_QUALITY),
_info_hardware_shader_support(false)
{	
	// #�۾�# ���⿡ INI�� ���Ϸκ��� �а� ����.
}
//	�Ҹ���
CINI::~CINI()
{}
//	����ǥ�� �ʺ� �����´� : �ػ� �� ������ ũ��ʹ� �ٸ� ����
int		CINI::getScreenWidth() 
{
	switch (_graphic_ratio)
	{
	case	RATIO_4_3:
		return 1200;
	case	RATIO_16_9:
		return 1600;
	case	RATIO_16_10:
		return 1440;
	default:
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���04:\tȭ�� ����(4:3)�� ���� ini������ �սǵǾ����ϴ�. �߸��� ������ �ƴѰ� �ǽɵ˴ϴ�."), 
			_T("�ذ���: ������ �ؽ��ĸ� ����� ���ο� �ؽ��ķ� ����ϴ�. �߸��� �׸��� ��µ� �� �ֽ��ϴ�."),
			_T(__FUNCTION__)
		);
		e.log();	
		return 1200;
	}
}
//	���̴�����Ƽ�� ����, ���̴��� ���� �ؽ����� ũ�⸦ ��ȯ�Ѵ�
float	CINI::getShaderWidth() 
{
	switch (_graphic_shader_quality)
	{
	case	SHADER_VERYLOW:
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 160.0f;

		case	RATIO_16_9:
			return 213.0f;

		case	RATIO_16_10:
			return 192.0f;			

		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���11:\t SHADER_VERYLOW����, ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 160.0f;
		}				

	case	SHADER_LOW:
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 320.0f;

		case	RATIO_16_9:
			return 427.0f;

		case	RATIO_16_10:
			return 384.0f;		
		
		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���12:\t SHADER_LOW����, ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 320.0f;
		}			
	case	SHADER_MID:
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 640.0f;

		case	RATIO_16_9:
			return 853.0f;

		case	RATIO_16_10:
			return 768.0f;		

		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���13:\t SHADER_MID����,ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 640.0f;
		}	

	case	SHADER_HIGH:
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 800.0f;

		case	RATIO_16_9:
			return 1067.0f;

		case	RATIO_16_10:
			return 960.0f;		

		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���14:\t SHADER_HIGH����, ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 800.0f;
		}	

	case	SHADER_VERYHIGH:
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 1024.0f;

		case	RATIO_16_9:
			return 1365.0f;

		case	RATIO_16_10:
			return 1229.0f;		

		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���15:\t SHADER_VERYHIGH����, ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 1024.0f;
		}	

	default:
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���16:\t �߸��� SHADER�� ǰ������. ���� ���� ǰ���� �ڵ� �����մϴ�."), 
			_T("�ذ���:"),
			_T(__FUNCTION__)
		);
		e.log();
		switch (_graphic_ratio)
		{
		case	RATIO_4_3:
			return 160.0f;

		case	RATIO_16_9:
			return 213.0f;

		case	RATIO_16_10:
			return 192.0f;			

		default:
			//	���� ����. ���� ������ ����Ѵ�
			CError e
			(	
				_T("���17:\t ���16����, ȭ�����(GRAPHIC_RATIO) ���� �սǵǾ����ϴ�. ���� ���� ũ��� �ڵ� �����մϴ�."), 
				_T("�ذ���:"),
				_T(__FUNCTION__)
			);
			e.log();	
			return 160.0f;
		}				
	}
}
//	���̴�����Ƽ�� ����, ���̴��� ���� �ؽ����� ũ�⸦ ��ȯ�Ѵ�
float	CINI::getShaderHeight() 
{
	switch (_graphic_shader_quality)
	{
	case	SHADER_VERYLOW:
		return 120.0f;			

	case	SHADER_LOW:
		return 240.0f;			

	case	SHADER_MID:		
		return 480.0f;			

	case	SHADER_HIGH:		
		return 600.0f;			

	case	SHADER_VERYHIGH:		
		return 768.0f;			
	
	default:
		//	���� ����. ���� ������ ����Ѵ�
		CError e
		(	
			_T("���18:\t ���̴� ǰ�� ���� �սǵǾ����ϴ�. ���� ���� ǰ���� �ڵ� �����մϴ�."), 
			_T("�ذ���:"),
			_T(__FUNCTION__)
		);
		e.log();					
		return 120.0f;			
	}
}
//	����ǥ�� ���̸� �����´�.
int		CINI::getScreenHeight() 
{ 
	return 900; 
}
// �ػ󵵸� ã�� �������� ������ ������ �߻������� ����������, �⺻�� (������ ����Ǵ�..)���� ����. ���� ���� �׷��� ����. ������ �׻� �����ϵ��� �Ѵ�
void	CINI::setDefaultVeryLow() 
{
	_graphic_window_mode = true;
	_graphic_height = 640;
	_graphic_width = 480;
	_graphic_ratio = RATIO_4_3;
	_graphic_vertical_synchronize = false;
	_graphic_32colorbit = false;
	_graphic_isShaderOn = false;
	setDefaultVeryLowShader();
}
//	���̴��� "����ǰ������" ���� �����Ѵ�.
void	CINI::setDefaultVeryLowShader()
{
	_graphic_shader_quality = SHADER_VERYLOW;
}