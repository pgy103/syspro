#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // create(), O_CREAT, O_WRONLY
#include <unistd.h>   // write(), close(), dup()
#include <errno.h>    // errno

int main(void) {
    int fd, fd_dup;

    fd = creat("myfile", 0600);
    if (fd < 0) {
        perror("myfile 생성 오류");
        exit(1);
    }

    if (write(fd, "Hello! Linux\n", 13) != 13) {
        perror("myfile 쓰기 오류(원본 fd)");
        close(fd);
        exit(1);
    }

    fd_dup = dup(fd);
    if (fd_dup < 0) {
        perror("파일 디스크립터 복제 오류");
        close(fd);
        exit(1);
    }

    if (write(fd_dup, "Bye! Linux\n", 11) != 11) {
        perror("myfile 쓰기 오류(복제 fd)");
        close(fd);
        close(fd_dup);
        exit(1);
    }

    close(fd);
    close(fd_dup);

    printf("파일 생성 및 복제 디스크립터 쓰기 완료: myfile\n");
    return 0;
}

