#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(int arg_cnt, char **argv)
{
	int fd[2]; //f[0]- to read, fd[1] - to
	pipe(fd);
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Fork Failed\n");
		exit(1);
	}
	else if(pid == 0)
	{
		close(fd[0]);
		char args[100]="";
		for(int i = 1; i < arg_cnt; i++)
		{
			strcat(args,argv[i]);
			if(i!=arg_cnt-1)
				strcat(args," ");
		}
		struct timeval start_time;
		gettimeofday(&start_time, NULL);
		write(fd[1], &start_time, sizeof(struct timeval));
		close(fd[1]);
		execlp("/bin/sh", "/bin/sh", "-c", args, NULL);
	}
	else
	{
		close(fd[1]);
		wait(NULL);
		struct timeval start_time,end_time;
		read(fd[0], &start_time, sizeof(struct timeval));
		close(fd[0]);
		gettimeofday(&end_time, NULL);
		printf("Elapsed Time: %ld\n", end_time.tv_usec - start_time.tv_usec);
	}
	return 0;
}
