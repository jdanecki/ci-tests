/*
Copyright (c) 2012-2017 Jacek Danecki <jacek.m.danecki@gmail.com>

This file is part of KaNaPi project

KaNaPi is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KaNaPi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KaNaPi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h> 
#include <sys/sysinfo.h>

struct thread_data {
	pthread_t tid;
	int id;
	unsigned long counter;
};

#define T_MAX 64

struct thread_data threads_data[T_MAX];
int finish;
unsigned long sum;

void * foo(void *data)
{
	struct thread_data *p = data;
//	printf("tid: %u\n", syscall(SYS_gettid));
	while (!finish) p->counter++;
}

int main(int argc, char **argv) 
{
	int i;
	int th = get_nprocs();
	unsigned int ticks;
  	struct timespec tp1, tp2;
	time_t sta, sto, delta;

	if (argc > 1) th = atoi(argv[1]);
	if (th < 0) th = get_nprocs();
	if (th > T_MAX) th = T_MAX;

	printf("testing with %d threads\n", th);

	clock_gettime(CLOCK_MONOTONIC_RAW, &tp1);

	for (i = 0; i < th; i++) {
		threads_data[i].id = i;
	   pthread_create(&threads_data[i].tid, NULL, foo, &threads_data[i]);
	}

	while (!finish) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &tp2);

		sta = tp1.tv_sec * 1000000000 + tp1.tv_nsec;
		sto = tp2.tv_sec * 1000000000 + tp2.tv_nsec;
		delta = sto - sta;
		if (delta > 10000000000 ) finish = 1;
	}
	printf("delta: %4lu %u ms\n", delta / 1000000);

	for (i = 0; i < th; i++) {
   		pthread_join(threads_data[i].tid, NULL);
	}
	
	for (i = 0; i < th; i++) {
		sum += threads_data[i].counter;
		printf("%d: %20lu %lu %lu\n", i, 
				threads_data[i].counter, 
				threads_data[i].counter / 1000,
				threads_data[i].counter / 1000000
		);
	}

	printf("s: %20lu %lu %lu %lu\n", sum, sum / 1000, sum / 1000000, sum / 1000000000);

	return 0;
}
