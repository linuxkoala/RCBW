#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

#define SEC_TO_MSEC 1000000

/* standard  end - start */
void cal_time(struct timeval *start, struct timeval *end)
{
    if (start->tv_usec > end->tv_usec) {
        end->tv_usec += 1 * SEC_TO_MSEC;
        end->tv_sec -= 1;
    }

    printf("%ld  ", (unsigned long)end->tv_usec - start->tv_usec);
}


const char *basedir = "/sys/fs/cgroup/cpu/";
const char *interface_share = "/cpu.shares";
const char *interface_quota = "/cpu.cfs_quota_us";
const char *interface_reserve = "/cpu.cfs_reserve_us";


void write_opt(char *grp, const char *interface)
{
    int fd, reval = 0;
    struct timeval start, end;
    char path[1024] = {0};
    char bufval[10];

    if (!grp) {
        strcpy(path, interface);
        strcpy(bufval, "1024");
       // printf("shares: %s, val : %s\n", path, bufval);
    } else {
        strcpy(path, basedir);
        strcat(path, grp);
        strcat(path, interface);
        strcpy(bufval, "40000");
       // printf("reserve: %s, val: %s\n", path, bufval);
    }
    

    fd = open(path, O_RDWR);
    if (fd == -1) {
        printf("Cann't open this file : %s\n", path);
        return ;
    }
        
    gettimeofday(&start, NULL);    
    reval = write(fd, bufval, strlen(bufval));
    gettimeofday(&end, NULL);

    cal_time(&start, &end);
    
    close(fd);
    return;
}


int main(int argc, char **args)
{
    int randval = 0;
    char buf[10];
    const char *compared_path = "/sys/fs/cgroup/cpu/part2/cpu.cfs_quota_us";
    struct sched_param child_param;
    pid_t pid;

    if (argc < 1) {
        printf("Missing parameters in child process!\n");
        return 0;
    }

    child_param.sched_priority = 19;
    pid = getpid();

    sched_setscheduler(pid, SCHED_FIFO, &child_param);

    write_opt(NULL, compared_path);
    write_opt(args[0], interface_reserve);
    
    printf("\n");
    return 0;
}


