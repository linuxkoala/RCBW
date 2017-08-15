#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>

int main(int argc, char **args)
{
    pid_t pid;
    int i;

    if (argc < 2) {
        printf("Missing parameters in main\n");
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if ((pid = fork()) < 0) {
            printf("fork error!\n");
            break;
        }else if (pid == 0) {
            execl("./exec_time", args[1], NULL);
        } 

        waitpid(pid, NULL, 0);
    }
    return 0;
}

