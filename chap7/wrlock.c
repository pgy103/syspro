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

    // 파일 열기 (읽기/쓰기 모드)
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror(argv[1]);
        exit(2);
    }

    printf("Enter StudentID you want to modify: ");
    scanf("%d", &id);

    // 쓰기 잠금 설정
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);

    // 파일 스트림 연결
    fp = fdopen(fd, "r+");

    // 임시 파일 생성
    FILE *tempfp = fopen("temp.txt", "w");
    if (!tempfp) {
        perror("temp.txt");
        exit(3);
    }

    // 한 줄씩 읽으면서 수정
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d %s %d", &s.id, s.name, &s.score);
        if (s.id == id) {
            found = 1;
            printf("Name:%s\tStuID:%d\tScore:%d\n", s.name, s.id, s.score);
            printf("Enter new score: ");
            scanf("%d", &s.score);
        }
        fprintf(tempfp, "%d %s %d\n", s.id, s.name, s.score);
    }

    // 파일 갱신
    freopen(argv[1], "w", fp);
    rewind(tempfp);
    while (fgets(line, sizeof(line), tempfp))
        fputs(line, fp);

    fclose(tempfp);
    fclose(fp);

    // 잠금 해제
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    if (!found)
        printf("Student ID %d not found.\n", id);

    return 0;
}

