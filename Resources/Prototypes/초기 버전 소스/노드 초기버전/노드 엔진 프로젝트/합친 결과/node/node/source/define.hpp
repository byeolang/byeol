/*
	���ϸ�:	define.hpp
	�̸�:	NODE�� ����Ʈ
	���:	���� NODE���� ���Ǵ� ��� ����Ʈ���� ����ִ�. ��� �κп����� �����Ӱ� include�ؼ� ����ϸ� �ȴ�.
*/
#pragma once
//	����:
//		�⺻ ��Ŭ���:
#include <d3dx9.h>
//		������:
//			NODE:
#define NODE_BUILD_NUMBER									5
#define NODE_VERSION										0.1 // �ܺο� ǥ��Ǵ� ����
#define NODE_KERNAL_VERSION									1.0.0 // ���� ��������, dll�� import�Ҷ� ���Ǵ� Ŀ�ι���
//			CHUB:
#define DEFAULT_CHUB_TIMER_PERIOD							1
//			CWindow:
#define DEFAULT_CWINDOW_WINDOW_X							100
#define DEFAULT_CWINDOW_WINDOW_Y							100
#define DEFAULT_CWINDOW_WINDOW_NAME							_T("DirectX 9 Tutorial 2 - Triangle")
#define DEFAULT_CWINDOW_WINDOW_CLASSNAME					_T("DX9_TUTORIAL2_CLASS")
//			CINI:
#define DEFAULT_CINI_GRAPHIC_WIDTH							1280
#define DEFAULT_CINI_GRAPHIC_HEIGHT							1024
#define DEFAULT_CINI_GRAPHIC_32COLORBIT						true
#define DEFAULT_CINI_GRAPHIC_MODE							false
#define DEFAULT_CINI_GRAPHIC_RATIO							RATIO_4_3
#define DEFAULT_CINI_GRAPHIC_VERTICAL_SYNCHRONIZE			true
#define	DEFAULT_CINI_GRAPHIC_32BITTEXTURE					true
#define DEFAULT_CINI_GRAPHIC_IS_SHADER_ON					true
#define DEFAULT_CINI_GRAPHIC_SHADER_QUALITY					SHADER_MID
//			CError:
#define	DEFAULT_CERROR_MESSAGEBOX_COMMENT_TOP				_T("������ ���� ������ �߻���, error.log�� ��� �߽��ϴ�.\n\n")
#define DEFAULT_CERROR_MESSAGEBOX_COMMENT_BOTTOM			_T("\n\n�ذ����� �𸦰�쿣, Ȩ�������� ����ʽÿ�.")	
//			CEngine:
#define DEFAULT_CENGINE_CREATEDEVICE_BACKBUFFERCOUNT		2
#define DEFAULT_CENGINE_CREATEDEVICE_FULLSCREEN_REFRESHRATE	D3DPRESENT_RATE_DEFAULT // 60�ϸ�, 60hz�� ��
#define DEFAULT_CENGINE_CREATEDEVICE_SWAPEFFECT				D3DSWAPEFFECT_DISCARD
#define DEFAULT_CENGINE_D3DFVF_MYVERTEX						(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define DEFAULT_CENGINE_PI									3.141592
#define DEFAULT_CENGINE_MATRIXTRANSFORM_MAX_Z				1.0f // �̰ͺ��� Z�� ũ�� ©��
#define DEFAULT_CENGINE_MATRIXTRANSFORM_MIN_Z				0.0f // �̰ͺ��� Z�� ������ ©��
#define DEFAULT_CENGINE_COLORKEY							D3DCOLOR_ARGB(255, 255, 0, 255)
#define DEFAULT_CENGINE_MAINFRAME_BACKGROUND				D3DCOLOR_XRGB(0, 0, 255)
#define DEFAULT_CENGINE_RENDERTARGET_BACKGROUND				D3DCOLOR_ARGB(0, 0, 0, 0)
#define DEFAULT_CENGINE_RENDERSTATE_NUMBER					4
#define DEFAULT_CENGINE_RENDERSTATE_DEFAULT					DEFAULT_CENGINE_RENDERSTATE_LINEAR
#define DEFAULT_CENGINE_RENDERSTATE_LINEAR					1	
#define DEFAULT_CENGINE_RENDERSTATE_ALPHABLENDING			2
#define DEFAULT_CENGINE_RENDERSTATE_DARKEN					3
#define DEFAULT_CENGINE_RENDERSTATE_NEGATIVE				4
#define	DEFAULT_CENGINE_VERTEXSHADER_VERSION				D3DVS_VERSION(2,0)
#define DEFAULT_CENGINE_PIXELSHADER_VERSION					D3DPS_VERSION(2,0)
//			CTexture:
#define	DEFAULT_CTEXTURE_MIPMAPLEVEL						1 // �Ӹ��� ������� ����
//			CShader:
#define DEFAULT_CSHADER_FILTER_NUMBER						3
#define DEFAULT_CSHADER_BLURFILTER							0
#define DEFAULT_CSHADER_NOISEFILTER							1
#define DEFAULT_CSHADER_BLOOMFILTER							2
//			CBlurFilter:
#define DEFAULT_CBLURFILTER_HLSL_PATH						_T("..\\resource\\shader\\blur.shader")
#define DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MIN				5.0f
#define DEFAULT_CBLURFILTERDATA_BLUR_LEVEL_MAX				50.0f
//			CNoiseFilter:
#define DEFAULT_CNOISEFILTER_HLSL_PATH						_T("..\\resource\\shader\\noise.shader")
#define DEFAULT_CNOISEFILTER_NOISEMAP_PATH					_T("..\\resource\\graphic\\noisemap.bmp")
#define DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL				1000
#define DEFAULT_CNOISEFILTER_MIN_NOISE_LEVEL				1
#define DEFAULT_CNOISEFILTER_DELAY							40
//			CBloomFilter:
#define DEFAULT_CBLOOMFILTER_HLSL_PATH						_T("..\\resource\\shader\\bloom.shader")
#define DEFAULT_CBLOOMFILTER_BLOOM_LEVEL					0.35f
#define DEFAULT_CBLOOMFILTER_BLOOM_MIN_LEVEL				0.0f
//		����ü:
//			���ؽ�:
typedef struct struct_myvertex
{
	FLOAT x, y, z;
	DWORD color; // RGBA
	FLOAT tu, tv; // �ؽ��� 1�� ���
} MYVERTEX;
//			ȭ�����:
typedef enum enum_window_ratio
{
	RATIO_4_3	= 0, // 1200x900
	RATIO_16_9	= 1, // 1600x900
	RATIO_16_10	= 2 // 1440x900
} WINDOW_RATIO;
//			���̴� ����Ƽ:
typedef enum enum_shader_quality
{
	SHADER_VERYLOW	= 0,
	SHADER_LOW,
	SHADER_MID,
	SHADER_HIGH,
	SHADER_VERYHIGH
} SHADER_QUALITY;