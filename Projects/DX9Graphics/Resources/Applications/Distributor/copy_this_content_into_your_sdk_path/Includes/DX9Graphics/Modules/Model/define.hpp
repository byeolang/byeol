#ifdef _DEBUG
#	ifdef DEVELOPER
#		define	_NAME				"Model"
#		define	_DEVELOPER			"kniz"
#		define	_REVISION			1
#		define	_COMMENT			"\n0��: TranslationX \n1��: TranlationY \n2��: TranslationZ \n3��: RotationX \n4��: RotationY \n5��: RotationZ \n6��: ScalingX \n7��: ScalingY \n8��: ScalingZ \n9��: ColorRed \n10��. ColorGreen \n11��. ColorBlue \n12��. ColorAlpha \n13��: SourceBlending(0�� ī�޶��� ������ �ڵ�����) \n14��: DestBlending(0�� ī�޶� ������ �ڵ�����)\n15��: SubModels \n16��. �������� �ϴ°�? \n17��: Modeling��� \n18��: Texture��� \n19��. �ؽ�Ʈ�� �������ΰ�? \n20��. �ִ´ٸ�, �ؽ��� ���"
#		define	_VERSION			"0.0.1a"
#		define	_DATE				"2013-08-08"
#		define	_DEPENDENCIES_COUNT	0
#		define	_KEY_COUNT			2
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