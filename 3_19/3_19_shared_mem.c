#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>

struct timeval *start_time;

int main(int arg_cnt, char **argv)
{
	start_time = mmap(NULL, sizeof(struct timeval), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS , -1, 0);

	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Fork Failed\n");
		exit(1);
	}
	else if(pid == 0)
	{
		char args[100]="";
		for(int i = 1; i < arg_cnt; i++)
		{
			strcat(args,argv[i]);
			if(i!=arg_cnt-1)
				strcat(args," ");
		}
		gettimeofday(start_time, NULL);
		execlp("/bin/sh", "/bin/sh", "-c", args, NULL);
	}
	else if(pid > 0)
	{
		wait(NULL);
		struct timeval end_time;
		gettimeofday(&end_time, NULL);
		printf("Elapsed Time: %ld\n", end_time.tv_usec - start_time->tv_usec);
	}
	munmap(start_time, sizeof(*start_time));
	return 0;
}
