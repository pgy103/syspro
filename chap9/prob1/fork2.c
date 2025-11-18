#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>   // ★ fork(), getpid() 사용 시 필요

int main() 
{
   int pid;

   pid = fork();   // 자식 프로세스 생성

   if (pid == 0) {     // 자식 프로세스
      printf("[Child] : Hello, world pid=%d\n", getpid());
   }
   else {              // 부모 프로세스
      printf("[Parent] : Hello, world pid=%d\n", getpid());
   }
}

