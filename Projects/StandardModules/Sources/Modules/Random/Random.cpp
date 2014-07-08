#include "Random.hpp"
#include <stdlib.h>

namespace NE
{
	NE_DLL Random::Random()
		: NEModule()
	{
		static type_bool is_random_seed_initialized = false;
		if( ! is_random_seed_initialized)
		{
			srand(time(0));
			is_random_seed_initialized = true;
		}
	}

	NE_DLL Random::Random(const Random& source)
		: NEModule(source)
	{

	}

	NE_DLL Random::~Random()
	{

	}

	type_result NE_DLL Random::initialize()
	{
		getKeySet().create(4);
		getKeySet().push(NEKeySelector());	//	0:	���
		getKeySet().push(NEIntKey());		//	1:	Maximum
		getKeySet().push(NEIntKey());		//	2:	Minimum
		getKeySet().push(NEFloatKey());		//	3:	����
		return RESULT_SUCCESS;
	}

	type_result NE_DLL Random::execute()
	{
		NEKeySelector& sel = getTargets();
		while(NEKey* itr = &sel.getKey())
			*itr = NEFloatKey(_createRandomValue());

		return RESULT_SUCCESS;
	}

	NEObject NE_DLL &Random::clone() const
	{
		return *(new Random(*this));
	}

	const NEExportable::ModuleHeader NE_DLL &Random::getHeader() const
	{
		static NEExportable::ModuleHeader _header;

		if(_header.isValid() != RESULT_NOTHING)
		{
			_header.getName() = _T("Random");
			_header.getDeveloper() = _T("kniz");
			_header.setRevision(1);
			_header.getComment() = _T("0��: ���ǰ��� �Ҵ���� Ű��\n1��: ���ǰ��� �ִ���� \n2��: ���ǰ��� �ּҹ��� \n3��: ���� ���ǰ��� ������ ����. 0.1�� �Է��� ���, �������� ������ 1/10�� �ȴ�.");
			_header.getVersion()  = _T("0.0.1a");
			_header.getReleaseDate() = _T("2013-10-15");
			_header.getModuleDependencies().create(0);
			_header.setMaxErrorCodeCount(0);
		}

		return _header;
	}

	type_float Random::_createRandomValue()
	{
		type_int	min = getMinimum(),
					range = getMaximum() - min,
					raw = rand() % range + min;
		
		return raw * getRate();
	}
}