#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mkdir() 함수 정의
#include <unistd.h>   // rmdir() 함수 정의

void my_cat(char *filename)
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

void my_cat_to_file(char *filename)
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

int my_mkdir(char *path)
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

int my_rmdir(char *path)
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

void my_mv(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage : file_rename src target\n");
        exit(1);
    }

    struct stat buf;
    char *target;
    char *src_file_name_only;

    if (access(argv[1], F_OK) < 0)
    {
        fprintf(stderr, "Usage : %s not exists\n", argv[1]);
        exit(1);
    }
    else
    {
        char *slash = strrchr(argv[1], '/');
        src_file_name_only = argv[1];
        if (slash != NULL)
        {
            src_file_name_only = slash + 1;
        }
    }

    target = (char *)calloc(strlen(argv[2]) + 1, sizeof(char));
    strcpy(target, argv[2]);

    if (access(argv[2], F_OK) == 0)
    {
        if (lstat(argv[2], &buf) < 0)
        {
            perror("lstat");
            exit(1);
        }
        else
        {
            if (S_ISDIR(buf.st_mode))
            {
                free(target);
                target = (char *)calloc(strlen(argv[1]) + strlen(argv[2]) + 2, sizeof(char));
                strcpy(target, argv[2]);
                strcat(target, "/");
                strcat(target, src_file_name_only);
            }
        }
    }
    printf("source = %s\n", argv[1]);
    printf("target = %s\n", target);

    if (rename(argv[1], target) < 0)
    {
        perror("rename");
        exit(1);
    }
    free(target);
}

void my_ln(int argc, char *argv[])
{
    char cmd;
    char *src;
    char *target;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: file_link [l,u,s] ...\n");
        fprintf(stderr, "file_link l[ink] src target\n");
        fprintf(stderr, "file_link u[nlink] filename\n");
        fprintf(stderr, "file_link s[ymlink] src target\n");
        exit(1);
    }
    cmd = (char)*argv[1];
    if (cmd == 'l')
    {
        if (argc < 4)
        {
            fprintf(stderr, "file_link l src target [link]\n");
            exit(1);
        }
        src = argv[2];
        target = argv[3];
        if (link(src, target) < 0)
        {
            perror("link");
            exit(1);
        }
    }
    else if (cmd == 's')
    {
        if (argc < 4)
        {
            fprintf(stderr, "file_link l src target [link]\n");
            exit(1);
        }
        src = argv[2];
        target = argv[3];

        if (symlink(src, target) < 0)
        {
            perror("symlink");
            exit(1);
        }
    }

    else if (cmd == 'u')
    {
        src = argv[2];
        if (unlink(src) < 0)
        {
            perror("unlink");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Unknown command...\n");
    }
}

int execute_command(int argc, char *argv[])
{
    if (strcmp(argv[0], "ls") == 0)
    {
        //my_ls();
        return 0;
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        //my_pwd();
        return 0;
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        //my_cd();
        return 0;
    }
    else if (strcmp(argv[0], "mkdir") == 0)
    {
        my_mkdir(argv[1]);
        return 0;
    }
    else if (strcmp(argv[0], "rmdir") == 0)
    {
        my_rmdir(argv[1]);
        return 0;
    }
    else if (strcmp(argv[0], "ln") == 0)
    {
        my_ln(argc, argv);
        return 0;
    }
    else if (strcmp(argv[0], "cp") == 0)
    {
        //my_cp();
        return 0;
    }
    else if (strcmp(argv[0], "rm") == 0)
    {
       // my_rm();
        return 0;
    }
    else if (strcmp(argv[0], "mv") == 0)
    {
        my_mv(argc, argv);
        return 0;
    }
    else if (strcmp(argv[0], "cat") == 0)
    {
        //my_cat(argv[1]);
        return 0;
    }
    else
    {
        execvp(argv[0], argv);
    }
}
