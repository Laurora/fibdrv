#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define FIB_DEV "/dev/fibonacci"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define loop 1000000
#define times 50


int main()
{
    int fd;
    long long sz;

    int buf;
    char write_buf[] = "testing writing";
    int offset = 0;  // TODO: test something bigger than the limit
    int i = 0;
    int j = 0;
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    double sum = 0;
    FILE *fp = fopen("output.txt", "w");

    for (j = 0; j < times; j++) {
        clock_gettime(CLOCK_ID, &start);
        fd = open(FIB_DEV, O_RDWR);
        if (fd < 0) {
            perror("Failed to open character device");
            exit(1);
        }

        /*for (i = 0; i <= offset; i++) {
            sz = write(fd, write_buf, strlen(write_buf));
            printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
        }
    */
        for (i = 0; i <= offset; i++) {
            lseek(fd, i, SEEK_SET);
            sz = read(fd, buf, 1);
            printf("Reading from " FIB_DEV
                   " at offset %d, returned the sequence "
                   "%lld.\n",
                   i, sz);
        }

        /* for (i = offset; i >= 0; i--) {
             lseek(fd, i, SEEK_SET);
             sz = read(fd, buf, 1);
             printf("Reading from " FIB_DEV
                    " at offset %d, returned the sequence "
                    "%lld.\n",
                    i, sz);
         }*/

        close(fd);
        clock_gettime(CLOCK_ID, &end);
        sum += (double) (end.tv_sec - start.tv_sec) +
               (end.tv_nsec - start.tv_nsec) / ONE_SEC;
        // fprintf(fp,"%lf ",(double)(end.tv_sec - start.tv_sec)+(end.tv_nsec -
        // start.tv_nsec)/ONE_SEC);
    }
    fprintf(fp, "%lf ", sum / times);
    fclose(fp);

    return 0;
}
