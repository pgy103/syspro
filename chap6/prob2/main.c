#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // chmod() 사용을 위해 필요

/* 파일 접근권한을 변경한다. */
int main(int argc, char *argv[])
{
    int newmode;

    // 인자 개수 확인
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <mode(octal)> <filename>\n", argv[0]);
        exit(1);
    }

    // 8진수 모드값을 정수로 변환
    newmode = (int) strtol(argv[1], NULL, 8);

    // 권한 변경 시도
    if (chmod(argv[2], newmode) == -1) {
        perror(argv[2]);
        exit(1);
    }

    return 0;
}

