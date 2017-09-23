#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#pragma warning(disable:4996)

#define rand32() rand()
#define rand64() ((uint64_t)rand() << 48) | ((uint64_t)rand() << 32) | ((uint64_t)rand() << 16) | ((uint64_t)rand() & 0xffff)

const double PI = 3.14159265358979323846;

double avg = 0;
uint64_t sum_count = 0;

uint64_t inline gcd(uint64_t a, uint64_t b) {
	uint64_t r;
	while (b) {
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

uint64_t inline randSMALL() {
	return (uint64_t)rand();
}

uint64_t inline randBIG() {
	return rand64();
}

int main() {
	uint64_t a, b, sz, max, counter;
	double ans, error, avg_error;
	uint64_t(*randF)(void);

	printf("Enter max num: ");
	scanf("%llu", &max);
	printf("Enter sample size: ");
	scanf("%llu", &sz);

	if (max > UINT32_MAX) {
		randF = &randBIG;
	} else {
		randF = &randSMALL;
	}

	srand(time(NULL));

	a = randF();
	b = randF();
	while (1) {

		counter = 0;
		do {
			for (uint64_t i = 0; i < sz; i++) {
				a = b;
				b = randF();
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

		printf("%.7f\t%.7f%%\t\tAVG: %.7f\t error: %.7f%%\t%llu\n", ans, error, avg, avg_error, sum_count);

	}

	system("pause");
	return 0;
}