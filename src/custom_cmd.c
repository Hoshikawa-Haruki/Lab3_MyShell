#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> // mkdir() 함수 정의
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

void my_cat(const char *filename)
{
    int fd = open(filename, O_RDONLY); // 파일 열기 (읽기 전용)
    if (fd < 0)
    {
        perror("cat"); // 파일 열기 실패 시 에러 메시지 출력
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read); // 읽은 데이터를 표준 출력으로 출력
    }

    if (bytes_read < 0)
    {
        perror("read"); // 읽기 실패 시 에러 메시지 출력
    }

    close(fd); // 파일 닫기
}

void my_mkdir(char *path)
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

void my_rmdir(char *path)
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

void my_cd(int argc, char *argv[])
{
    const int MAX_BUF = 1024;
    char buf[MAX_BUF];
    if (argc < 2)
    {
        fprintf(stderr, "Usage: file_chdir dirname\n");
        exit(1);
    }
    memset(buf, 0, MAX_BUF);
    if (getcwd(buf, MAX_BUF) < 0)
    {
        perror("getcwd");
        exit(1);
    }
    printf("working directory (before) = %s\n", buf);
    if (chdir(argv[1]) < 0)
    {
        perror("chdir");
        exit(1);
    }

    memset(buf, 0, MAX_BUF);
    if (getcwd(buf, MAX_BUF) < 0)
    {
        perror("getcwd");
        exit(1);
    }
    printf("working directory (after ) = %s\n", buf);
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

void my_pwd()
{
    char path[1024];

    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
    }
    else
    {
        perror("명령어 실행 실패");
    }
}


void my_ls(){
    DIR *pdir;
    struct dirent *pde;
    int i=0;

    char dirPath[1024];

    if(getcwd(dirPath,sizeof(dirPath))!=NULL){
        pdir = opendir(dirPath);
        while((pde = readdir(pdir))!=NULL){
            printf("%20s",pde->d_name);
        }
    }

    printf("\n");
    closedir(pdir);
}


void my_cp(int argc, char *argv[])
{
    const int MAX_READ = 1024;
    int src_fd;
    int des_fd;

    char buf[MAX_READ];
    ssize_t rcnt;
    ssize_t tot_rcnt = 0;

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if (argc < 3)
    {
        fprintf(stderr, "Usage : file_copy src_file des_file\n");
        exit(1);
    }

    if ((src_fd = open(argv[1], O_RDONLY)) == -1)
    {
        perror("src open");
        exit(1);
    }

    if ((des_fd = creat(argv[2], mode)) == -1)
    {
        perror("des open");
        exit(1);
    }

    while ((rcnt = read(src_fd, buf, MAX_READ)) > 0)
    {
        tot_rcnt += write(des_fd, buf, rcnt);
    }

    if (rcnt < 0)
    {
        perror("read");
        exit(1);
    }

    printf("total write count = %ld\n", tot_rcnt);
    close(src_fd);
    close(des_fd);
}

void my_rm(int argc, char *argv[]){

    char *path = argv[1];

    if(argc<2){
        fprintf(stderr,"rm : missing opernad \n");
    }

    if(remove(path)==0){
        printf("%s 삭제 완료\n",path);
    }
    else{
        perror("error ");
    } 
}

void execute_command(int argc, char *argv[])
{
    if (strcmp(argv[0], "ls") == 0)
    {
        my_ls();
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        my_pwd();
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        my_cd(argc, argv);
    }
    else if (strcmp(argv[0], "mkdir") == 0)
    {
        my_mkdir(argv[1]);
    }
    else if (strcmp(argv[0], "rmdir") == 0)
    {
        my_rmdir(argv[1]);
    }
    else if (strcmp(argv[0], "ln") == 0)
    {
        my_ln(argc, argv);
    }
    else if (strcmp(argv[0], "cp") == 0)
    {
        my_cp(argc, argv);
    }
    else if (strcmp(argv[0], "rm") == 0)
    {
        my_rm(argc,argv);
    }
    else if (strcmp(argv[0], "mv") == 0)
    {
        my_mv(argc, argv);
    }
    else if (strcmp(argv[0], "cat") == 0)
    {
        my_cat(argv[1]);
    }
    else
    {
        execvp(argv[0], argv);
    }

    exit(0);
}
