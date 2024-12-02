#include "my_cat.h"
#include <stdio.h>

void my_cat(const char *filename)
{
    FILE *file = fopen(filename, "r"); // 파일 열기 (읽기 모드)
    if (file == NULL)
    {
        perror("cat"); // 파일 열기 실패 시 에러 메시지 출력
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // 파일 내용을 표준 출력으로 출력
    }

    fclose(file); // 파일 닫기
}

void my_cat_stdin()
{
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        printf("%s", buffer); // 표준 입력 내용 출력
    }
}

void my_cat_to_file(const char *filename)
{
    FILE *file = fopen(filename, "w"); // 파일 열기 (쓰기 모드)
    if (file == NULL)
    {
        perror("cat");
        return;
    }

    char buffer[1024];
    printf("Enter text (Ctrl+D to save and exit):\n");
    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        fprintf(file, "%s", buffer); // 표준 입력 내용을 파일에 쓰기
    }

    fclose(file); // 파일 닫기
    printf("Content saved to '%s'.\n", filename);
}
