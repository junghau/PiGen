#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <thread>
#include <atomic>
#include <random>
#include <functional>
#include "timeit.h"

#pragma warning(disable:4996)

using namespace std;

using UID_64 = uniform_int_distribution<uint64_t>;
using rng_t = decltype(bind(UID_64(), mt19937_64())); // RNG type
rng_t *randomizer;

constexpr double PI = 3.14159265358979323846;

bool print_time_saved = false;
uint64_t thread_count, sz, max;
atomic<uint64_t> sum_count(0);
atomic<double> avg(0.0), ElapsedCombined(0.0);

uint64_t inline gcd(uint64_t a, uint64_t b) {
	uint64_t r;
	while (b) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

void approx() {
	timeit local_timer;
	local_timer.start();

	uint64_t counter = 0; //number of coprimes
	uint64_t a, b;
	double ans, error, avg_error;

	rng_t &myrand = *randomizer;
	printf("\r");

	do {
		b = myrand();
		for (uint64_t i = 1; i < sz; i++) {
			a = b;
			b = myrand();
			if (gcd(a, b) == 1) {
				counter++;
			}
		}
	} while (counter == 0); //if counter is 0, then divide_by_zero will occur

	ans = sqrt(6 / (counter / (double)sz));
	sum_count++;
	avg = (ans / sum_count) + avg * ((sum_count - 1) / (double)sum_count);
	error = fabs(1 - (ans / PI)) * 100;
	avg_error = fabs(1 - (avg / PI)) * 100;

	local_timer.stop();
	ElapsedCombined = ElapsedCombined + local_timer.elapsed();
	printf("%.7f\t%.7f%%\t\tAVG: %.7f\t error: %.7f%%\t%llu\n", ans, error, (double)avg, avg_error, sum_count);
}

int main() {
	printf("Enter thread count: ");
	scanf("%llu", &thread_count);
	printf("Enter max num: ");
	scanf("%llu", &max);
	printf("Enter sample size: ");
	scanf("%llu", &sz);

	char c;
	printf("Print total time saved from multi-threading? (Y/N): ");
	scanf(" %c",&c);
	if (c == 'Y' || c == 'y') print_time_saved = true;

	UID_64 dice_distribution(1, max);
	mt19937_64 random_number_engine(time(0)); // pseudorandom number generator
	randomizer = reinterpret_cast<rng_t*>(&bind(dice_distribution, random_number_engine));

	thread *th = new thread[thread_count];

	timeit mainTimer;
	double ElapsedActual, totalTimeSaved = 0;
	while (1) {
		ElapsedCombined = 0.0;
		mainTimer.start();
		for (int i = 0; i < thread_count;i++) {
			th[i] = thread(approx);
		}
		for (int i = 0; i < thread_count;i++) {
			th[i].join();
		}
		mainTimer.stop();
		ElapsedActual = mainTimer.elapsed();
		totalTimeSaved += ElapsedCombined - ElapsedActual;
		if (print_time_saved)
			printf("\t\t\t\t\t\tTotal Time SAVED: %.8f", totalTimeSaved);
		//approx();
	}

	delete[] th;
	return 0;
}
