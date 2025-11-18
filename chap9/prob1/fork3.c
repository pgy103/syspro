#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pid1, pid2;

    // 첫 번째 자식 생성
    pid1 = fork();

    if (pid1 == 0) {   // pid1 == 0 → 자식1
        printf("[Child 1] : Hello, world !  pid = %d\n", getpid());
        exit(0);
    }

    // 두 번째 자식 생성
    pid2 = fork();

    if (pid2 == 0) {   // pid2 == 0 → 자식2
        printf("[Child 2] : Hello, world ! pid = %d\n", getpid());
        exit(0);
    }

    // 부모 프로세스
    printf("[PARENT] : Hello, world ! pid = %d\n", getpid());

    return 0;
}

