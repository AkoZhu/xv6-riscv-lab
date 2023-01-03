//
// Created by Xuanbiao Zhu on 12/28/22.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
main()
{
    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);

    int pid = fork();
    if(pid == 0){
        // child
        // use p1 to write p1[1]
        // use p2 to read  p2[0]
        close(p1[0]);
        close(p2[1]);
        char* buffer1 = malloc(sizeof(char));
        read(p2[0], buffer1, 10);
        int childPid = getpid();
        printf("%d: received %s\n", childPid, buffer1);
        write(p1[1], "pong", 5);

        close(p1[1]);
        close(p2[0]);
        exit(0);
    }else{
        // parent
        // use p1 to read  p1[0]
        // use p2 to write p2[1]
        close(p1[1]);
        close(p2[0]);
        write(p2[1], "ping", 5);
        char *buffer2 = malloc(sizeof(char));
        read(p1[0], buffer2, 10);
        int parentPid = getpid();
        printf("%d: received %s\n", parentPid, buffer2);

        close(p1[0]);
        close(p2[1]);
        exit(0);
    }


}