# 컴파일러
CC = gcc

# 라이브러리 생성 도구
AR = ar

# 컴파일 옵션
CFLAGS = -I../include
LDFLAGS = -L../lib -lcommand

# 소스 파일
CUSTOM_CMD_SRC = custom_cmd.c
MYSHELL_SRC = MyShell.c

# 객체 파일
CUSTOM_CMD_OBJ = custom_cmd.o

# 정적 라이브러리
LIBRARY = ../lib/libcommand.a

# 실행 파일
TARGET = MyShell

# 기본 목표
all: $(TARGET)

# MyShell 실행 파일 생성
$(TARGET): $(CUSTOM_CMD_OBJ) $(MYSHELL_SRC) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(MYSHELL_SRC) $(LDFLAGS)

# custom_cmd.o 파일 생성
$(CUSTOM_CMD_OBJ): $(CUSTOM_CMD_SRC)
	$(CC) -c $<

# 정적 라이브러리 생성
$(LIBRARY): $(CUSTOM_CMD_OBJ)
	$(AR) rcs $@ $^

# 클린업 목표
clean:
	rm -f $(CUSTOM_CMD_OBJ) $(TARGET) $(LIBRARY)
