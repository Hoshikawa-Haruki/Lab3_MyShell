#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // mkdir() 함수 정의
#include <unistd.h>   // rmdir() 함수 정의

void my_cat(const char *filename);
void my_cat_stdin();
void my_cat_to_file(const char *filename);
int my_mkdir(const char *path);
int my_rmdir(const char *path);
