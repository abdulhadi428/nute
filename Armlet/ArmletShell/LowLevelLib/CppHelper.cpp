#include <iostream>
#include <cstdio>
#include <ctime>

std::clock_t startTime = 0;

namespace Helper {

	long GetRunningTime()
	{
		if (startTime == 0) {
			startTime = std::clock();
			return 0;
		}
		long duration = ( std::clock() - startTime ) / CLOCKS_PER_SEC;
		return duration;
	}

} //namespace
