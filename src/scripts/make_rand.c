#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 512

int buf[BUF_SIZE];

main(int argc, char **argv) {
    int i;
    off_t ret, total, size;
    int fd;

    fd = open("test_file", O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
            
    size = atoll(argv[1]);
    total = 0;
    while (total < size) {   
        for (i = 0; i < BUF_SIZE; i++) {
            buf[i] = random();
        }

        i = 1;
        while ((ret = write(fd, &(buf[i]), BUF_SIZE - i)) > 0 && (BUF_SIZE - i) > 0) i += ret;

        total++;
    }

    close(fd);
}
