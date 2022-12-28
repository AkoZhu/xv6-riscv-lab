//
// Created by Xuanbiao Zhu on 12/27/22.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



void
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "usage: sleep seconds\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    sleep(n);
    exit(0);
}
