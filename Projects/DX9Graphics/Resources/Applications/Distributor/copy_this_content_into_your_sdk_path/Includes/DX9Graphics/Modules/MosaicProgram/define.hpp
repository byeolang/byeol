#ifdef _DEBUG
#	ifdef DEVELOPER
#		define	_NAME				"MosaicProgram.dev.dbg"
#		define	_DEVELOPER			"kniz"
#		define	_REVISION			1
#		define	_COMMENT			"0��: ��⼿���ͷ� ���ε��Ͽ�, ���۸� ����.\n1��: ����Ÿ���� Width����(X����Blur����) \n2��: ����Ÿ���� Height����(Y����Blur����) \n3��: ���������� Ÿ��(0: ȭ�鿡 ��� 1: ���ο� ���ۿ� �ʱ�ȭ �� ��� 2: ���� ���ۿ� �����) \n4��: ������ũ ���е�(�⺻ 30)"
#		define	_VERSION			"0.0.1a"
#		define	_DATE				"2013-08-29"
#		define	_DEPENDENCIES_COUNT	0
#		define	_KEY_COUNT			1
#	endif
#else
#	ifdef DEVELOPER
#		define	_NAME				"Model.dev"
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
#		define	_NAME				"Model"
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