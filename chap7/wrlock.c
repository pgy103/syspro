#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "student.h"

/* 잠금을 이용한 학생 데이터베이스 수정 프로그램 */
#define START_ID 1001001  // 학생 ID 시작 번호 (test.dat의 첫 학번 기준)

int main(int argc, char *argv[]) {
    int fd, id;
    struct student record;
    struct flock lock;

    if (argc < 2) {
        fprintf(stderr, "How to use: %s File\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    printf("\nEnter StudentID you want to modify: ");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_WRLCK;  // 쓰기 잠금
        lock.l_whence = SEEK_SET;
        lock.l_start = (id - START_ID) * sizeof(record);
        lock.l_len = sizeof(record);

        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror(argv[1]);
            exit(3);
        }

        lseek(fd, (id - START_ID) * sizeof(record), SEEK_SET);

        if ((read(fd, (char *)&record, sizeof(record)) > 0) && (record.id != 0)) {
            printf("Name: %s\t StuID: %d\t Score: %d\n",
                   record.name, record.id, record.score);
            printf("Enter new score: ");
            scanf("%d", &record.score);

            lseek(fd, -sizeof(record), SEEK_CUR);
            write(fd, (char *)&record, sizeof(record));

            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);  // 잠금 해제
        } else {
            printf("No record %d\n", id);
        }

        printf("\nEnter StudentID you want to modify: ");
    }

    close(fd);
    exit(0);
}

