#include "CNoiseFilterData.hpp"
//	������
CNoiseFilterData::CNoiseFilterData() : _max(DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL), _min(DEFAULT_CNOISEFILTER_MIN_NOISE_LEVEL), 
_delay(DEFAULT_CNOISEFILTER_DELAY), _before_xrepeat(1.0f), _before_yrepeat(1.0f)
{}
//	�Ҹ���
CNoiseFilterData::~CNoiseFilterData()
{}
//	�߻��� �� �ִ� �ִ� ������ ���� �����´�
int		CNoiseFilterData::getNoiseMaxLevel()
{
	return _max;
}
//	�߻��� �� �ִ� �ִ� ������ ���� ���Ѵ�
void	CNoiseFilterData::setNoiseMaxLevel(int noise_max)
{
	if (noise_max <= _min)
		noise_max = _min + 1;
	if (noise_max > DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL)
		noise_max = DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL;

	_max = noise_max;
}
//	�߻��� �� �ִ� �ּ� ������ ���� �����´�
int		CNoiseFilterData::getNoiseMinLevel()
{
	return _min;
}
//	�߻��� �� �ִ� �ּ� ������ ���� ���Ѵ�
void	CNoiseFilterData::setNoiseMinLevel(int noise_min)
{
	if (noise_min < DEFAULT_CNOISEFILTER_MIN_NOISE_LEVEL)
		noise_min = DEFAULT_CNOISEFILTER_MAX_NOISE_LEVEL;
	if (noise_min >= _max)
		noise_min = _max - 1;

	_max = noise_min;
}
//	����� ���ϰԵǴ� �ð������� ���Ѵ�
void	CNoiseFilterData::setDelay(int delay)
{
	//	���ܻ�Ȳó��:
	if (delay < 0)
		delay = 1;

	_delay = delay;
}
//	����� ���ϰԵǴ� �ð������� �����´�
int		CNoiseFilterData::getDelay()
{
	return _delay;
}
//	���ο� ����� �׸� �ð��� �Ǿ����� Ȯ���Ѵ�
bool	CNoiseFilterData::isTimeElapsed()
{
	DWORD	now = timeGetTime();
	if ( (static_cast<int>(now-_before_worked_milisecond)) >= _delay )
	{
		_before_worked_milisecond = now;
		return true;
	}

	return false;
}
