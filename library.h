#pragma once

#include <stdio.h>

#ifdef WIN32
#include <Windows.h>

double windows_timer_get_time()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	
	LARGE_INTEGER performance_count;
	QueryPerformanceCounter(&performance_count); //get current tick
	return performance_count.QuadPart * (1000000.0 / frequency.QuadPart); //returns time in micro seconds
}

void test_windows_timer()
{
	double start_time = windows_timer_get_time();
	Sleep(500);
	double end_time = windows_timer_get_time();
	double elapsed_time = end_time - start_time;
	fprintf(stderr, "%lf(micro_seconds)\n", elapsed_time);
}
#endif
