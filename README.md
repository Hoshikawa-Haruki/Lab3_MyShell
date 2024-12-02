# Open Source Project LAB 3
동의대학교 오픈소스 프로젝트 교과목 LAB 3 과제에 대한 레포지토리입니다.

## 실행/개발 방법
### 1. git clone
``` bash
git clone https://github.com/Hoshikawa-Haruki/Lab3_MyShell.git
```
### 2. 실행 방법
```bash
cd src

// MyShell.c 컴파일
gcc -I../include -L../lib MyShell.c -lcommand -o MyShell

// MyShell 실행
./MyShell
```

### 3. 명령어 개발 방법
```
//디렉토리 이동
cd src

// custom_cmd.c에서 구현

// 컴파일 실행
gcc -c custom_cmd.c

// 정적 라이브러리 생성
ar rcs ../lib/libcommand.a custom_cmd.o

// MyShell.c 컴파일
gcc -I../include -L../lib MyShell.c -lcommand -o MyShell

// MyShell 실행
./MyShell
```



## 디렉토리 구조 
```
.
├── LICENSE
├── My_Shell
│   ├── command.h
│   ├── custom_cmd.c
│   ├── my_cat.c
│   ├── my_cat.h
│   ├── my_dir.h
│   ├── my_mkdir.c
│   ├── my_rmdir.c
│   ├── path.txt
│   ├── simplesh.c
│   └── simpleshell.c
└── README.md
```

## 과제 설명

11. 간단한 쉘 프로그램
    - 팀과제로 간단한 쉘 프로그램을 만들어 여러 기능(종료, 백그라운드 실행, 인터럽트 처리, 파일 재지향, 명령 구현)을 추가함.

