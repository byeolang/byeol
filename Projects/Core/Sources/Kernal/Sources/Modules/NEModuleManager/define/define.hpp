#ifdef DEVELOPER
#	ifdef _DEBUG
//	��� ���:
#		define _DEFAULT_ID					0
#		define _NAME						"ModuleManager"
#		define _REVISION					1
#		define _DATE						"2011-07-11"
#		define _VERSION						"0.0.1a"
#		define _COMMENT						"���� DLL�� �����Ѵ�."
#		define _DEPENDENCIES				""
#		define _DEVELOPER					"haku"
#		define _DLL_ENTRYPOINT_STRING		"?registerModuleDevDbg@NE@@YG?AV?$NETList@PAVNEModule@NE@@$00$0HIECCCA@@1@XZ"
//	���:
#		define _DEFAULT_DLL_PATH			"dev.dbg.module"
#	else
//	��� ���:
#		define _DEFAULT_ID					0
#		define _NAME						"ModuleManager"
#		define _REVISION					1
#		define _DATE						"2011-07-11"
#		define _VERSION						"0.0.1a"
#		define _COMMENT						"���� DLL�� �����Ѵ�."
#		define _DEPENDENCIES				""
#		define _DEVELOPER					"haku"
#		define _DLL_ENTRYPOINT_STRING		"?registerModuleDev@NE@@YG?AV?$NETList@PAVNEModule@NE@@$00$0HIECCCA@@1@XZ"
//	���:									 
#		define _DEFAULT_DLL_PATH			"dev.module"
#		define _MODULE_EXTRACTOR_TO_SEARCH	"*.dll"
#	endif
#else
//	��� ���:
#	define _DEFAULT_ID					0
#	define _NAME						"ModuleManager"
#	define _REVISION					1
#	define _DATE						"2011-07-11"
#	define _VERSION						"0.0.1a"
#	define _COMMENT						"���� DLL�� �����Ѵ�."
#	define _DEPENDENCIES				""
#	define _DEVELOPER					"haku"
#	define _DLL_ENTRYPOINT_STRING		"?registerModule@NE@@YG?AV?$NETList@PAVNEModule@NE@@$00$0HIECCCA@@1@XZ"
//	���:
#	define _DEFAULT_DLL_PATH			"module"
#	define _MODULE_EXTRACTOR_TO_SEARCH	"*.dll"
#endif