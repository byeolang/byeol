#ifdef _DEBUG
#	ifdef DEVELOPER
#		define	_NAME				"TabledTexture.dev.dbg"
#		define	_DEVELOPER			"kniz"
#		define	_REVISION			1
#		define	_COMMENT			"0��: DX9��� \n1��: ��� \n\n2��: ColorKeyRed \n3��. ColorKeyGreen\n4��. ColorKeyBlue\n5��. ColorKeyAlpha \n6��: �ؽ��������� MaxRow\n7��. �ؽ��������� MaxCol\n8��: �� �����Ӵ� Delay(-1:���Ѵ�)\n9��: Animation�� ��밡���� Col"
#		define	_VERSION			"0.0.1a"
#		define	_DATE				"2013-08-07"
#		define	_DEPENDENCIES_COUNT	0
#		define	_KEY_COUNT			0
#	endif
#else
#	ifdef DEVELOPER
#		define	_NAME				"TabledTexture.dev"
#		define	_DEVELOPER			"kniz"
#		define	_REVISION			1
#		define	_COMMENT			"0��: ��⼿���ͷ� ���ε��Ͽ�, ���۸� ����.\n1��: ��� ������ ��\n2��. FLAG"
#		define	_VERSION			"0.0.1a"
#		define	_DATE				"2013-01-29"
#		define	_DEPENDENCIES_COUNT	0
#		define	_KEY_COUNT			2
#		define	_DEFAULT_COLOR		D3DCOLOR_ARGB(255, 0, 0, 75)
#		define	_DEFAULT_FLAG		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
#	else
#		define	_NAME				"TabledTexture"
#		define	_DEVELOPER			"kniz"
#		define	_REVISION			1
#		define	_COMMENT			"0��: ��⼿���ͷ� ���ε��Ͽ�, ���۸� ����.\n1��: ��� ������ ��\n2��. FLAG"
#		define	_VERSION			"0.0.1a"
#		define	_DATE				"2013-01-29"
#		define	_DEPENDENCIES_COUNT	0
#		define	_KEY_COUNT			2
#		define	_DEFAULT_COLOR		D3DCOLOR_ARGB(255, 0, 0, 75)
#		define	_DEFAULT_FLAG		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
#	endif
#endif