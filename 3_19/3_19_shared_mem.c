#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>

/* shared object */
struct timeval * start_time;

int main(int arg_cnt, char ** argv) {
    if (arg_cnt < 2) {
        printf("Invalid Syntax");
        exit(0);
    }
    /* creating a shared memory space using mmap */
    start_time = mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (start_time == MAP_FAILED)
    {
    	printf("Mapping failed\n");
    	exit(0);
    }
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(1);
    } 
    else if (pid == 0) 
    {
        /* child process here */
        int tot_len = 0; /* to store total length of arguments */
        for(int i=1;i<arg_cnt;i++)
        	tot_len += strlen(argv[i]) + 1;
        char *args = malloc(sizeof(char) * tot_len);
        for (int i = 1; i < arg_cnt; i++) {
            strcat(args, argv[i]);
            if (i != arg_cnt - 1)
                strcat(args, " ");
        }
        gettimeofday(start_time, NULL);
        /* following will execute the command on shell */
        execlp("/bin/sh", "/bin/sh", "-c", args, NULL);
    } 
    else if (pid > 0) 
    {
    	/* wait for child to finish */
        wait(NULL); 
        struct timeval end_time;
        gettimeofday( & end_time, NULL);
        printf("Elapsed Time: %ld\n", end_time.tv_usec - start_time -> tv_usec);
    }
	/* deleting the shared memory mapping */
    munmap(start_time, sizeof( * start_time));
    return 0;
}

