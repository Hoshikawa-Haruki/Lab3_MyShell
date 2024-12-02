#ifndef MY_DIR_H
#define MY_DIR_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // mkdir() 함수 정의
#include <unistd.h>   // rmdir() 함수 정의

int my_mkdir(const char *path);
int my_rmdir(const char *path);

#endif // MY_DIR_H