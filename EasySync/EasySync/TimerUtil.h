#pragma once

#include <chrono>
#include <memory>

namespace EasySync
{
	class TimerUtil
	{
	public:
		void Start();
		void Stop();
		double ElapsedSeconds();
		double ElapsedMilliseconds();
		double ElapsedMins();

	private:
		std::unique_ptr<std::chrono::high_resolution_clock::time_point> startTime{};
		std::unique_ptr<std::chrono::high_resolution_clock::time_point> endTime{};
	};
}