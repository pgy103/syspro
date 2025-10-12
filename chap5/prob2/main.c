#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 512

int main(int argc, char *argv[]) {
    int fd;
    ssize_t nread;
    char buf[BUFSIZE];
    long total = 0;

    if (argc != 2) {
        fprintf(stderr, "사용법: %s [파일이름]\n", argv[0]);
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("파일 열기 실패");
        exit(1);
    }

    while ((nread = read(fd, buf, BUFSIZE)) > 0) {
        total += nread;
    }

    if (nread == -1) {
        perror("파일 읽기 실패");
        close(fd);
        exit(1);
    }

    printf("파일명: %s\n", argv[1]);
    printf("파일 크기: %ld 바이트\n", total);

    close(fd);
    return 0;
}

