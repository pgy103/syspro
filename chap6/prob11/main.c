#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
// #include <inttypes.h> // 이식성을 높이려면 추가할 수 있지만, 여기선 직접 지정자로 수정했습니다.

void print_permissions(mode_t mode) {
    char perms[10] = "---------";
    
    // 파일 타입 지정자 추가 (ls -l 스타일)
    if (S_ISDIR(mode)) perms[0] = 'd';
    else if (S_ISLNK(mode)) perms[0] = 'l';
    else if (S_ISCHR(mode)) perms[0] = 'c';
    else if (S_ISBLK(mode)) perms[0] = 'b';
    else if (S_ISFIFO(mode)) perms[0] = 'p';
    else if (S_ISSOCK(mode)) perms[0] = 's';
    else perms[0] = '-'; // 일반 파일

    // 소유자 권한
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';
    
    // 그룹 권한
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';
    
    // 기타 사용자 권한
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';
    
    // SetUID, SetGID, Sticky Bit 처리 (선택 사항)
    // 이 부분은 복잡하므로 여기서는 생략하고 기본 권한만 출력합니다.

    printf("%.10s ", perms); // 파일 타입 포함하여 10자 출력
}

int main(int argc, char *argv[]) {
    int opt_i = 0, opt_p = 0, opt_Q = 0;
    char *dir_name = ".";

    // 옵션 처리
    // 이 부분은 getopt를 사용하는 것이 가장 정확하나, 원래 로직을 유지합니다.
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strchr(argv[i], 'i')) opt_i = 1;
            if (strchr(argv[i], 'p')) opt_p = 1;
            if (strchr(argv[i], 'Q')) opt_Q = 1;
        } else {
            dir_name = argv[i]; // 디렉터리 지정
        }
    }

    DIR *dp = opendir(dir_name);
    if (dp == NULL) {
        perror("opendir");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        char path[1024];
        // 디렉터리 경로와 파일 이름을 조합
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        struct stat st;
        // stat 대신 심볼릭 링크 자체의 정보를 얻기 위해 lstat을 권장합니다.
        if (lstat(path, &st) == -1) { 
            perror("lstat");
            continue;
        }

        // i-노드 옵션: __darwin_ino64_t에 맞게 %llu 사용 (경고 수정)
        if (opt_i) {
            printf("%llu ", st.st_ino);
        }

        // 권한, 링크 수, 소유자, 그룹, 크기, 시간
        print_permissions(st.st_mode);
        
        // st_nlink: nlink_t에 맞게 %lu 또는 %hu 사용 (경고 수정 - %lu가 일반적)
        printf("%lu ", st.st_nlink);

        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        printf("%s %s ",
               pw ? pw->pw_name : "unknown",
               gr ? gr->gr_name : "unknown");

        // st_size: off_t에 맞게 %lld 사용 (경고 수정)
        printf("%lld ", st.st_size);

        char timebuf[80];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
        printf("%s ", timebuf);

        // 파일 이름 옵션
        if (opt_Q) printf("\"%s\"", entry->d_name);
        else printf("%s", entry->d_name);

        // 디렉터리 이름 뒤 / 붙이기 옵션
        // lstat을 사용했으므로 S_ISDIR로 디렉터리인지 확인 가능합니다.
        if (opt_p && S_ISDIR(st.st_mode)) printf("/");

        printf("\n");
    }

    closedir(dp);
    return 0;
}
