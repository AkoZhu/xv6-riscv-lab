//
// Created by Xuanbiao Zhu on 12/29/22.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char *path, char *name){
    char buf[512], *p, *q;
    int fd;
    struct dirent de;
    struct stat st;

    int offset = 0;

    // If the path starts with a "." or "..".
    if(path[0] == '.'){
        offset = 1;
    }

    // Can't open
    if((fd = open(path + offset, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // Can't get the stat
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if(st.type == T_FILE)
        return;

    // Search the current directory
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    q = p;
    for(int nameIdx = 0; nameIdx < strlen(name); nameIdx++){
        *p++ = name[nameIdx];
    }

    // Print the target in the current directory.
    if(open(buf + offset, 0) > 0){
        printf("%s\n", buf);
    }

    // Recursively search the subdirectory

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        // If the current file is a directory, recursively search it.
        strcpy(q, de.name);

        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type != T_DIR){
            continue;
        }

        char *newPath = malloc(strlen(buf));
        strcpy(newPath, buf);
        find(newPath, name);
        free(newPath);
    }
    close(fd);
}

void main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(2, "usage: find path filename\n");
        exit(1);
    }
    find(argv[1], argv[2]);
}

