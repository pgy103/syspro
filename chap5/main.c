#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NAME_LEN 20

typedef struct {
    int id;              // 학번
    char name[NAME_LEN];  // 이름
    int score;           // 점수
} Student;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <레코드파일명>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("파일 열기 오류");
        return 1;
    }

    char cont;
    do {
        int search_id;
        printf("검색할 학생의 학번 입력 : ");
        if (scanf("%d", &search_id) != 1) {
            printf("입력 오류\n");
            while (getchar() != '\n');
            continue;
        }

        Student s;
        int found = 0;
        off_t offset = 0;

        lseek(fd, 0, SEEK_SET);

        while (read(fd, &s, sizeof(Student)) == sizeof(Student)) {
            if (s.id == search_id && s.id != 0) {
                printf("학번 : %d\n", s.id);
                printf("이름 : %s\n", s.name);
                printf("점수 : %d\n", s.score);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("레코드 [%d] 없음\n", search_id);
        }

        printf("계속하시겠습니까?(Y/N) : ");
        while (getchar() != '\n'); // 버퍼 비우기
        scanf("%c", &cont);
    } while (cont == 'Y' || cont == 'y');

    close(fd);
    return 0;
}

