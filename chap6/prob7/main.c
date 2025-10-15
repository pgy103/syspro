#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    if (unlink(argv[1]) == -1) {
        perror(argv[1]);
        exit(1);
    }

    printf("'%s' 링크 삭제 완료\n", argv[1]);
    return 0;
}

