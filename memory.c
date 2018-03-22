/*
Copyright (c) 2012-2018 Jacek Danecki <jacek.m.danecki@gmail.com>

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
#include <unistd.h>
#include <stdlib.h>	
#include <string.h>
#include <errno.h>

#define MAX_MEM (4LL * 1024 * 1024 * 1024)
#define MEM_CHUNK (100LL * 1024 * 1024)

int main(int argc, char **argv) 
{
	int i;
	unsigned long long mem = MAX_MEM;
	unsigned long long allocated = 0;

	if (argc > 1) mem = 1024LL * atoll(argv[1]) ;
	if (mem < 0) mem = MAX_MEM;

	printf("testing with %u GB\n", mem / 1024/1024/1024);

	for (i = 0; allocated <= mem; i++) {
	   char * p = malloc(MEM_CHUNK);
	   if (!p) {
		   printf("can't allocate more\n");
		   break;
	   }
	   memset(p, 0, MEM_CHUNK);
		allocated += MEM_CHUNK;
		printf("allocated = %u MB\n", allocated / 1024/1024);
		if (system("free")) {
			int e = errno;
			printf("can't execute free errno=%d %s\n", e, strerror(e));
			break;
		}
	}

	return 0;
}
