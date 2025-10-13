#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // open() O_RDONLY, O_WRONLY, O_CREAT 등
#include <unistd.h>   // read(), write(), close()
#include <errno.h>    // errno
#include <string.h>   // strerror()

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    int fd_in, fd_out;
    ssize_t nread;
    char buf[BUFSIZE];

    if (argc != 3) {
        fprintf(stderr, "사용법: %s [원본파일명] [새파일명]\n", argv[0]);
        exit(1);
    }

    fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        perror("원본 파일 열기 오류");
        exit(1);
    }

    fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd_out < 0) {
        perror("새 파일 생성 오류");
        close(fd_in);
        exit(1);
    }

    while ((nread = read(fd_in, buf, BUFSIZE)) > 0) {
        if (write(fd_out, buf, nread) != nread) {
            perror("파일 쓰기 오류");
            close(fd_in);
            close(fd_out);
            exit(1);
        }
    }

    if (nread < 0) {
        perror("파일 읽기 오류");
    }

    close(fd_in);
    close(fd_out);

    printf("파일 복사가 완료되었습니다: %s → %s\n", argv[1], argv[2]);
    return 0;
}

