#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // mkdir() 함수 정의
#include <unistd.h>   // rmdir() 함수 정의

void execute_command(char *cmd)
{
    char *custom_cmd[9] = {"ls", "pwd", "cd", "mkdir", "rmdir", "ln", "cp", "rm", "mv", "cat"};

    if (strcmp(cmd, "ls") == 0)
    {
        my_ls();
        return 0;
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        my_pwd();
        return 0;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        my_cd();
        return 0;
    }
    else if (strcmp(cmd, "mkdir") == 0)
    {
        my_mkdir();
        return 0;
    }
    else if (strcmp(cmd, "rmdir") == 0)
    {
        my_rmdir();
        return 0;
    }
    else if (strcmp(cmd, "ln") == 0)
    {
        my_ln();
        return 0;
    }
    else if (strcmp(cmd, "cp") == 0)
    {
        my_cp();
        return 0;
    }
    else if (strcmp(cmd, "rm") == 0)
    {
        my_rm();
        return 0;
    }
    else if (strcmp(cmd, "mv") == 0)
    {
        my_mv();
        return 0;
    }
    else if (strcmp(cmd, "cat") == 0)
    {
        my_cat();
        return 0;
    }
    else
    {
        execvp();
    }
}

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

int my_rmdir(const char *path)
{
    if (rmdir(path) == 0)
    {
        printf("Directory '%s' removed successfully.\n", path);
        return 0;
    }
    else
    {
        perror("rmdir");
        return -1;
    }
}
