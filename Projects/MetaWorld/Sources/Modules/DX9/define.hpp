#ifdef _DEBUG
#	ifdef DEVELOPER
#		define	_NAME									"DX9.dev.dbg"
#		define	_DEVELOPER								"kniz"
#		define	_REVISION								1
#		define	_COMMENT								"1��: ini ����ϴ°�\n2��: ��������ȭ ���\n3��:������ ����ΰ�\n4��. 32��Ʈ���\n5��. 32��Ʈ �ؽ��� ���\n6��. ��ũ�� �ʺ�\n7��. ��ũ�� ����\n8��. ��ũ�� ����(�ʺ�/����)\n9��. �� ���� ī��Ʈ\n10��. ��ũ�� ������(refresh rate)\n10��. SWAP���(SWAP_EFFECT)\n11��. Į��Ű\n12��. �⺻����"
#		define	_VERSION								"0.0.1a"
#		define	_DATE									"2013-01-29"
#		define	_DEPENDENCIES_COUNT						0
#		define	_KEY_COUNT								12
//	keynames:
#		define	_KEYNAME_OF_IS_VERTICAL_SYNCHRONIZED	"isVerticalSynchronized"
#		define	_KEYNAME_OF_IS_WINDOW_MODE				"isWindowMode"
#		define	_KEYNAME_OF_IS_32BIT_COLOR_MODE			"is32bitColorMode"
#		define	_KEYNAME_OF_IS_32BIT_TEXTURE_MODE		"is32bitTextureMode"
#		define	_KEYNAME_OF_SCREEN_WIDTH				"screenWidth"
#		define	_KEYNAME_OF_SCREEN_HEIGHT				"screenHeight"
#		define	_KEYNAME_OF_SCREEN_RATIO				"screenRatio"
#		define  _KEYNAME_OF_BACK_BUFFER_COUNT			"backBufferCount"
#		define  _KEYNAME_OF_SCREEN_REFRESH_RATE			"screenRefreshRate"
#		define  _KEYNAME_OF_SWAP_EFFECT					"swapEffect"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_KEY		"backgroundColor"
//	defaults:
#		define	_DEFAULT_IS_LOAD_FROM_INI_WHEN_START	true
#		define	_DEFAULT_IS_WINDOW_MODE					false			
#		define	_DEFAULT_IS_VERTICAL_SYNCHRONIZED		true
#		define	_DEFAULT_IS_32BIT_COLOR_MODE			true
#		define	_DEFAULT_IS_32BIT_TEXTURE_MODE			true
#		define	_DEFAULT_SCREEN_WIDTH					640
#		define	_DEFAULT_SCREEN_HEIGHT					480
#		define	_DEFAULT_SCREEN_RATIO					4.0f / 3.0f
#		define	_DEFAULT_BACK_BUFFER_COUNT				1
#		define	_DEFAULT_SCREEN_REFRESH_RATE			60
#		define	_DEFAULT_SWAP_EFFECT					D3DSWAPEFFECT_DISCARD
#		define	_DEFAULT_COLOR_KEY						255
#		define	_DEFAULT_BACKGROUND_COLOR_KEY			D3DCOLOR_ARGB(0, 0, 0, 0)
#	endif
#else
#	ifdef DEVELOPER
#		define	_NAME									"DX9.dev"
#		define	_DEVELOPER								"kniz"
#		define	_REVISION								1
#		define	_COMMENT								"0��: ini ����ϴ°�\n1��:������ ����ΰ�\n2��: ��������ȭ ���\n3��. 32��Ʈ���\n4��. 32��Ʈ �ؽ��� ���\n5��. ��ũ�� �ʺ�\n6��. ��ũ�� ����\n7��. ��ũ�� ����(�ʺ�/����)\n8��. �� ���� ī��Ʈ\n9��. ��ũ�� ������(refresh rate)\n10��. SWAP���(SWAP_EFFECT)\n11��. Į��Ű\n12��. �⺻����"
#		define	_VERSION								"0.0.1a"
#		define	_DATE									"2013-01-29"
#		define	_DEPENDENCIES_COUNT						0
#		define	_KEY_COUNT								14
//	keynames:
#		define	_KEYNAME_OF_VERTICAL_SYNCHRONIZED		"isVerticalSynchronized"
#		define	_KEYNAME_OF_IS_WINDOWED					"isWindowed"
#		define	_KEYNAME_OF_IS_32BIT_COLOR				"is32bitColor"
#		define	_KEYNAME_OF_IS_32BIT_TEXTURE			"is32bitTexture"
#		define	_KEYNAME_OF_SCREEN_WIDTH				"screenWidth"
#		define	_KEYNAME_OF_SCREEN_HEIGHT				"screenHeight"
#		define	_KEYNAME_OF_SCREEN_RATIO				"screenRatio"
#		define  _KEYNAME_OF_BACKBUFFER_COUNT			"backBufferCount"
#		define  _KEYNAME_OF_SCREEN_REFRESH_RATE			"screenRefreshRate"
#		define  _KEYNAME_OF_SWAP_EFFECT					"swapEffect"
#		define  _KEYNAME_OF_COLOR_KEY_ALPHA				"colorKeyAlpha"
#		define  _KEYNAME_OF_COLOR_KEY_RED				"colorKeyRed"
#		define  _KEYNAME_OF_COLOR_KEY_GREEN				"colorKeyGreen"
#		define  _KEYNAME_OF_COLOR_KEY_BLUE				"colorKeyBlue"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_ALPHA		"backgroundColorAlpha"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_RED		"backgroundColorRed"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_GREEN		"backgroundColorGreen"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_BLUE		"backgroundColorBlue"
#		define  _KEYNAME_OF_TEXTURE_PATHS				"texturePaths"
//	defaults:
#		define	_DEFAULT_IS_LOAD_FROM_INI_WHEN_START	true
#		define	_DEFAULT_IS_WINDOW_MODE					false			
#		define	_DEFAULT_IS_VERTICAL_SYNCHRONIZED		true
#		define	_DEFAULT_IS_32BIT_COLOR_MODE			true
#		define	_DEFAULT_IS_TEXTURE_32BIT				true
#		define	_DEFAULT_SCREEN_WIDTH					640
#		define	_DEFAULT_SCREEN_HEIGHT					480
#		define	_DEFAULT_SCREEN_RATIO					4.0f / 3.0f
#		define	_DEFAULT_BACK_BUFFER_COUNT				1
#		define	_DEFAULT_SCREEN_REFRESH_RATE			60
#		define	_DEFAULT_SWAP_EFFECT					D3DSWAPEFFECT_DISCARD
#		define	_DEFAULT_COLOR_KEY						D3DCOLOR_ARGB(255, 0, 0, 0)
#		define	_DEFAULT_BACKGROUND_COLOR_KEY			D3DCOLOR_ARGB(0, 0, 0, 0)
#	else
#		define	_NAME									"DX9"
#		define	_DEVELOPER								"kniz"
#		define	_REVISION								1
#		define	_COMMENT								"0��: ini ����ϴ°�\n1��:������ ����ΰ�\n2��: ��������ȭ ���\n3��. 32��Ʈ���\n4��. 32��Ʈ �ؽ��� ���\n5��. ��ũ�� �ʺ�\n6��. ��ũ�� ����\n7��. ��ũ�� ����(�ʺ�/����)\n8��. �� ���� ī��Ʈ\n9��. ��ũ�� ������(refresh rate)\n10��. SWAP���(SWAP_EFFECT)\n11��. Į��Ű\n12��. �⺻����"
#		define	_VERSION								"0.0.1a"
#		define	_DATE									"2013-01-29"
#		define	_DEPENDENCIES_COUNT						0
#		define	_KEY_COUNT								14
//	keynames:
#		define	_KEYNAME_OF_VERTICAL_SYNCHRONIZED		"isVerticalSynchronized"
#		define	_KEYNAME_OF_IS_WINDOWED					"isWindowed"
#		define	_KEYNAME_OF_IS_32BIT_COLOR				"is32bitColor"
#		define	_KEYNAME_OF_IS_32BIT_TEXTURE			"is32bitTexture"
#		define	_KEYNAME_OF_SCREEN_WIDTH				"screenWidth"
#		define	_KEYNAME_OF_SCREEN_HEIGHT				"screenHeight"
#		define	_KEYNAME_OF_SCREEN_RATIO				"screenRatio"
#		define  _KEYNAME_OF_BACKBUFFER_COUNT			"backBufferCount"
#		define  _KEYNAME_OF_SCREEN_REFRESH_RATE			"screenRefreshRate"
#		define  _KEYNAME_OF_SWAP_EFFECT					"swapEffect"
#		define  _KEYNAME_OF_COLOR_KEY_ALPHA				"colorKeyAlpha"
#		define  _KEYNAME_OF_COLOR_KEY_RED				"colorKeyRed"
#		define  _KEYNAME_OF_COLOR_KEY_GREEN				"colorKeyGreen"
#		define  _KEYNAME_OF_COLOR_KEY_BLUE				"colorKeyBlue"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_ALPHA		"backgroundColorAlpha"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_RED		"backgroundColorRed"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_GREEN		"backgroundColorGreen"
#		define  _KEYNAME_OF_BACKGROUND_COLOR_BLUE		"backgroundColorBlue"
#		define  _KEYNAME_OF_TEXTURE_PATHS				"texturePaths"
//	defaults:
#		define	_DEFAULT_IS_LOAD_FROM_INI_WHEN_START	true
#		define	_DEFAULT_IS_WINDOW_MODE					false			
#		define	_DEFAULT_IS_VERTICAL_SYNCHRONIZED		true
#		define	_DEFAULT_IS_32BIT_COLOR_MODE			true
#		define	_DEFAULT_IS_TEXTURE_32BIT				true
#		define	_DEFAULT_SCREEN_WIDTH					640
#		define	_DEFAULT_SCREEN_HEIGHT					480
#		define	_DEFAULT_SCREEN_RATIO					4.0f / 3.0f
#		define	_DEFAULT_BACK_BUFFER_COUNT				1
#		define	_DEFAULT_SCREEN_REFRESH_RATE			60
#		define	_DEFAULT_SWAP_EFFECT					D3DSWAPEFFECT_DISCARD
#		define	_DEFAULT_COLOR_KEY						D3DCOLOR_ARGB(255, 0, 0, 0)
#		define	_DEFAULT_BACKGROUND_COLOR_KEY			D3DCOLOR_ARGB(0, 0, 0, 0)
#	endif
#endif