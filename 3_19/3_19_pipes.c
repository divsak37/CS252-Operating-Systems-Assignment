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
    int fd[2]; /* f[0]- to read, fd[1] - to */
    /* creating a pipe */
    pipe(fd);
    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork Failed\n");
        exit(0);
    } else if (pid == 0) {
        /* child process here */
        /* close the read file descripter
           since child does not need to read */
        close(fd[0]);
        char args[200] = "";
        for (int i = 1; i < arg_cnt; i++) {
            strcat(args, argv[i]);
            if (i != arg_cnt - 1)
                strcat(args, " ");
        }
        struct timeval start_time;
        gettimeofday( & start_time, NULL);
        /* write start_time to the pipe */
        write(fd[1], & start_time, sizeof(struct timeval));
        /* close the write file descripter */
        close(fd[1]);
        /* following will execute the command on shell */
        execlp("/bin/sh", "/bin/sh", "-c", args, NULL);
    } else {
        /* close the write file descripter
           since parent does not need to read */
        close(fd[1]);
        wait(NULL); /* wait for child process to finish */
        struct timeval start_time, end_time;
        read(fd[0], & start_time, sizeof(struct timeval)); /* read the start_time form pipe */
        close(fd[0]);
        gettimeofday( & end_time, NULL);
        printf("Elapsed Time: %ld\n", end_time.tv_usec - start_time.tv_usec);
    }
    return 0;
}

