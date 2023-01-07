//
// Created by Xuanbiao Zhu on 1/6/23.
//

/**
 * Trace
 *
 * In this assignment you will add a system call tracing feature that may help you when debugging later labs.
 * You'll create a new trace system call that will control tracing.
 * It should take one argument, an integer "mask", whose bits specify which system calls to trace.
 *
 * For example, to trace the fork system call, a program calls trace(1 << SYS_fork),
 * where SYS_fork is a syscall number from kernel/syscall.h. You have to modify the xv6 kernel to print out a
 * line when each system call is about to return, if the system call's number is set in the mask. The line
 * should contain the process id, the name of the system call and the return value; you don't need to print
 * the system call arguments. The trace system call should enable tracing for the process that calls it and
 * any children that it subsequently forks, but should not affect other processes.
 *
 * */


#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int i;
    char *nargv[MAXARG];

    if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
        fprintf(2, "Usage: %s mask command\n", argv[0]);
        exit(1);
    }

    if (trace(atoi(argv[1])) < 0) {
        fprintf(2, "%s: trace failed\n", argv[0]);
        exit(1);
    }

    for(i = 2; i < argc && i < MAXARG; i++){
        nargv[i-2] = argv[i];
    }
    exec(nargv[0], nargv);
    exit(0);
}



