#include "my_dir.h"

int my_mkdir(const char *path)
{
    if (mkdir(path, 0755) == 0)
    { // 디렉토리 생성 함수
        printf("Directory '%s' created successfully.\n", path);
        return 0;
    }
    else
    {
        perror("mkdir");
        return -1;
    }
}