#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>

int main(int arg_cnt, char ** argv) {
    if (arg_cnt < 2) {
        printf("Invalid Syntax");
        exit(0);
    }
    int fd[2]; 
    /* creating a pipe for one-way communication 
    *  using a read and a write descriptor, 
    *  f[0]- to read, fd[1] - to */
    pipe(fd);
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(0);
    } 
    else if (pid == 0) 
    {
        /* child process here */
        /* close the read descripter
           since child does not need to read */
        close(fd[0]);
        int tot_len = 0; /* to store total length of arguments */
        for(int i=1;i<arg_cnt;i++)
        	tot_len += strlen(argv[i]) + 1;
        char *args = malloc(sizeof(char) * tot_len);
        for (int i = 1; i < arg_cnt; i++) {
            strcat(args, argv[i]);
            if (i != arg_cnt - 1)
                strcat(args, " ");
        }
        struct timeval start_time;
        gettimeofday( & start_time, NULL);
        /* write start_time to the pipe */
        if(write(fd[1], & start_time, sizeof(struct timeval)) == -1)
        {
        	printf("Error writing to the pipe \n");
        	exit(0);
        }
        /* close the write descripter */
        close(fd[1]);
        /* following will execute the command on shell */
        execlp("/bin/sh", "/bin/sh", "-c", args, NULL);
    } 
    else 
    {
        /* close the write descripter
           since parent does not need to read */
        close(fd[1]);
        /* wait for child process to finish */
        wait(NULL); 
        struct timeval start_time, end_time;
        /* read the start_time from pipe */
        if(read(fd[0], & start_time, sizeof(struct timeval)) == -1) 
        {
        	printf("Error reading from the pipe \n");
        	exit(0);
        }
        /* close the read descriptor */
        close(fd[0]);
        gettimeofday( & end_time, NULL);
        printf("Elapsed Time: %ld\n", end_time.tv_usec - start_time.tv_usec);
    }
    return 0;
}

