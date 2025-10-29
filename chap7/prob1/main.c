#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    Student s;
    
    if (argc < 2) {
        fprintf(stderr, "사용법: %s 파일이름\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        exit(1);
    }

    printf("StuID\tName\tScore\n");

    while (fscanf(fp, "%d %s %d", &s.StuID, s.Name, &s.Score) == 3) {
        printf("%d\t%s\t%d\n", s.StuID, s.Name, s.Score);
    }

    fclose(fp);
    return 0;
}

