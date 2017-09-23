#ifndef TIMEIT_H
#define TIMEIT_H

#ifdef __cplusplus
#include <chrono>

class timeit {
public:
	void start() {
		start_time = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		end_time = std::chrono::high_resolution_clock::now();
	}
	double elapsed() {
		std::chrono::duration<double> elapsed = end_time - start_time;
		return elapsed.count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
};

#else

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

class timeit {
public:
	void start() {
		gettimeofday(&start_time, NULL);
	}
	void stop() {
		gettimeofday(&end_time, NULL);
	}
	double elapsed() {
		return (double)(end_time.tv_usec - start_time.tv_usec) / 1000000 + (double)(end_time.tv_sec - start_time.tv_sec);
	}
private:

	// MSVC defines this in winsock2.h!?
	struct timeval {
		long tv_sec;
		long tv_usec;
	};

	int gettimeofday(timeval * tp, struct timezone * tzp)
	{
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		return 0;
	}
	timeval start_time, end_time;
};
#endif
#endif