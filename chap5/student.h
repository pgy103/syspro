// student.h
#ifndef STUDENT_H
#define STUDENT_H

#define NAMELEN 24       // 이름 최대 공간
#define START_ID 1401001 // 레코드 시작 학번

struct student {
    int id;                   // 학번
    char name[NAMELEN];       // 이름
    int score;                // 점수
};

#endif

