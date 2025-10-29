#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct student {
    int id;
    char name[20];
    int score;
};

int main(int argc, char *argv[]) {
    int fd;
    struct flock lock;
    struct student s;
    int id, found = 0;
    FILE *fp;
    char line[100];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        exit(2);
    }

    printf("Enter StudentID to search: ");
    scanf("%d", &id);

    // 읽기 잠금 설정
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);

    fp = fdopen(fd, "r");

    // 한 줄씩 읽으며 검색
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d %s %d", &s.id, s.name, &s.score);
        if (s.id == id) {
            printf("Name:%s\tStuID:%d\tScore:%d\n", s.name, s.id, s.score);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student ID %d not found.\n", id);

    fclose(fp);

    // 잠금 해제
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return 0;
}

