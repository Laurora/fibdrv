#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
//#include "bigN.h"

#define FIB_DEV "/dev/fibonacci"
#define MAX_LENGTH 200

int main()
{
    int fd;
    long kernel_time;

    int buf;
    char write_buf[] = "testing writing";
    //    int offset = 1000;  // TODO: test something bigger than the limit
    int i = 0;

    fd = open(FIB_DEV, O_RDWR);

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    /*
        for (i = 0; i <= offset; i++) {
            sz = write(fd, write_buf, strlen(write_buf));
            printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
        }
    */

    struct timespec start, end;
    printf(
        "|n|\t|user-space 時間差|\t|kernel 計算所花時間|\t|kernel傳遞到user "
        "space時間開銷|\n");
    for (i = 0; i <= MAX_LENGTH; i++) {
        lseek(fd, i, SEEK_SET);
        // buf.lower = 0;
        // buf.upper = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        kernel_time = read(fd, &buf, sizeof(int));
        clock_gettime(CLOCK_MONOTONIC, &end);

        printf("%3d\t\t%ld\t\t\t%ld\t\t\t\t%ld ", i,
               end.tv_nsec - start.tv_nsec, kernel_time,
               end.tv_nsec - start.tv_nsec - kernel_time);
        //        printBigN(buf);
        printf("\n");
    }
    /*
        for (i = offset; i >= 0; i--) {
            lseek(fd, i, SEEK_SET);
            sz = read(fd, buf, 1);
            printf("Reading from " FIB_DEV
                   " at offset %d, returned the sequence "
                   "%lld.\n",
                   i, sz);
        }
    */
    close(fd);
    return 0;
}