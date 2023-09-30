#include "TimerUtil.h"

using namespace std;
using namespace std::chrono;

void EasySync::TimerUtil::Start()
{
	startTime = std::make_unique<high_resolution_clock::time_point>(high_resolution_clock::now());
}

void EasySync::TimerUtil::Stop()
{
	endTime = std::make_unique<high_resolution_clock::time_point>(high_resolution_clock::now());
}

double EasySync::TimerUtil::ElapsedSeconds()
{
	return (double)duration_cast<seconds>(*endTime - *startTime).count();
}

double EasySync::TimerUtil::ElapsedMilliseconds()
{
	return (double)duration_cast<milliseconds>(*endTime - *startTime).count();
}

double EasySync::TimerUtil::ElapsedMins()
{
	return (double)duration_cast<minutes>(*endTime - *startTime).count();
}
