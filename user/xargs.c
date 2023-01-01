//
// Created by Xuanbiao Zhu on 12/31/22.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void copy(char **p1, char *p2){
    *p1 = malloc(strlen(p2) + 1);
    strcpy(*p1, p2);
}

int readLine(char **pars, int i){
    int d = 1024;
    char buf[d];
    int j = 0;
    // read a line
    while(read(0, buf + j, 1)){
        if(buf[j] == '\n'){
            buf[j] = '\0';
            break;
        }
        j++;
        if(j == d){
            fprintf(2, "Parameters are too long.\n");
            exit(1);
        }
    }

    if(j == 0){
        return -1;
    }

    // parse the line
    int k = 0;
    while(k < j){
        if(i > MAXARG){
            fprintf(2, "Too many parameters.\n");
            exit(1);
        }

        // '   abc   sdf'
        while((k < j) && (buf[k] == ' ')){
            k++;
        }
        // start position
        int l = k;
        while((k < j) && (buf[k] != ' ')){
            k++;
        }
        buf[k++] = 0;
        copy(&pars[i], buf + l);
        i++;
    }
    return i;
}


int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(2, "Please enter more parameters!\n");
        exit(1);
    }

    int i;
    char *pars[MAXARG];
    for(i = 1; i < argc; i++){
        copy(&pars[i - 1], argv[i]);
    }

    int end;
    while((end = readLine(pars, argc - 1)) != -1){
        pars[end] = '\0';

        if(fork() == 0){
            exec(pars[0], pars);

            // if wrong.
            exit(1);
        }else{
            wait(0);
        }
    }
    return 0;
}