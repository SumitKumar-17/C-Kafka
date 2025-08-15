#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../include/mapper.h"

void read_log_with_mmap(char *filepath){
    int fd=open(filepath,O_RDONLY);
    if(fd<0){
        perror("open error");
        return;
    }

    struct stat st;
    fstat(fd,&st);

    return;
}