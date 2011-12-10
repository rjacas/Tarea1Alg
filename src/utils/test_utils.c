#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 8

void print_file_integers(int fd) {
    int ret, i, total;
    int buf[BUF_SIZE];
    lseek(fd, 0,SEEK_SET);
    total = 0;
    while ((ret = read(fd, (void *)buf, BUF_SIZE * sizeof(int))) > 0) {
        for (i = 0; i < ret/sizeof(int); i++) {
            printf ("Read integer: %d\n", buf[i]);
        }
        total += ret/sizeof(int);

    }

    printf("Total number of integers in file: %d\n", total);
}
