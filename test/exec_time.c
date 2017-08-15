#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

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

void write_opt(char *grp, const char *interface, char *bufval)
{
    int fd, reval = 0;
    struct timeval start, end;
    char path[1024] = {0};
    int lenbasedir = strlen(basedir), lengrp = strlen(grp);

    strcpy(path, basedir);
    strcat(path, grp);
    strcat(path, interface);
/*
    fd = open(path, O_RDWR);
    if (fd == -1) {
        printf("Cann't open this file : %s\n", path);
        return ;
    }

    gettimeofday(&start, NULL);
    if (!bufval)
        reval = write(fd, "30000", strlen("30000"));
    else
        reval = write(fd, "40000", strlen("40000"));
    gettimeofday(&end, NULL);
*/

    if (!bufval) {
        fd = open("/sys/fs/cgroup/cpu/part2/cpu.cfs_quota_us", O_RDWR);
        gettimeofday(&start, NULL);
        reval = write(fd, "30000", strlen("30000"));
        gettimeofday(&end, NULL);
    } else {
        fd = open(path, O_RDWR);
        gettimeofday(&start, NULL);
        reval = write(fd, "40000", strlen("40000"));
        gettimeofday(&end, NULL);
    }

    cal_time(&start, &end);
    
    close(fd);
    return;
}


int main(int argc, char **args)
{
    int randval = 0;
    char buf[10];

    if (argc < 1) {
        printf("Missing parameters in child process!\n");
        return 0;
    }

    write_opt(args[0], interface_quota, NULL);

    srand((unsigned int)time(NULL));
    do{
        randval = rand() % 10;
    }while (randval < 3);

    sprintf(buf, "%d", randval * SEC_TO_MSEC / 100);
    write_opt(args[0], interface_reserve, buf);
    printf("\n");
    return 0;
}


