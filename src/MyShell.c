#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "command.h"

pid_t pid;

// 명령어와 인자를 구분하는 함수
int getargs(char *cmd, char **argv);

// 3. 시그널 처리 함수 
void handler(int signo);

int main(){
	char buf[256];
	char *argv[50];
	int narg;
	int is_background;
	int is_error;

	// 부모 프로세스(쉘)은 인터럽트 시그널 무시하고 자식 프로세스에게 시그널 전송
	struct sigaction p_act;
	p_act.sa_handler = handler;
	sigfillset(&(p_act.sa_mask));

	// 인터럽트 같은 신호들은 부모(쉘)가 아닌 자식(실행중인 명령어)에게 전달
	sigaction(SIGINT, &p_act, NULL);
	sigaction(SIGQUIT, &p_act, NULL);
	//sigaction(SIGTSTP, &p_act, NULL);

	while(1){
		is_error = 0;
		// 사용자 명령 입력 
		printf("shell> ");
		gets(buf);
		clearerr(stdin);

		is_background = 0;
		// 명령어와 인자를 구분
		narg = getargs(buf, argv);
		if(narg == 0){
			continue;
		}

		// 1. exit 이면 탈출할 것
		if(strcmp(argv[0], "exit") == 0)
			exit(0);

		// 2. 마지막 인자가 &이면 백그라운드 작업해야함
		// 	- 인자하나 없얘야함.
		if(strcmp(argv[narg - 1], "&") == 0){
			argv[narg--] = NULL;
			is_background = 1;
		}


		// 4. 리다이렉션 및 파이프
		// 4-1 리다이렉션
		int i;
		int fd = -1;

		// 백업용 fd
		int fd_in = dup(0);
		int fd_out = dup(1);
		int fd_err = dup(2);

		for(i = 0; i < narg; i++){
			if(strcmp(argv[i], ">") == 0){	
				if((fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
					perror("open");
					exit(1);
				}
				if((dup2(fd, 1) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}
			else if(strcmp(argv[i], ">>") == 0){
				if((fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1){
					perror("open");
					exit(1);
				}
				if((dup2(fd, 1) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}	
			else if(strcmp(argv[i], "<") == 0){
				if((fd = open(argv[i + 1], O_RDONLY, 0644)) == -1){
					perror("open");
					is_error = 1;
					break;
				}
				if((dup2(fd, 0) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}
		}

		// 4-2 파이프
		// 명령어 앞 뒤로 구분하기
		// 앞의 명령어 출력을 뒤의 명령어 입력으로 바꾸기
		int is_pipe = 0;
		int pfd[2];
		int j;

		// 뒤의 명령어 분할
		char *argv2[50];

		// 파이프 유무 확인
		for(i = 0; i < narg; i++){
			if(strcmp(argv[i], "|") == 0){
				is_pipe = 1;
				argv[i] = NULL;

				for(j = 0; j + i + 1 < narg; j++){
					argv2[j] = argv[j+i+1];
				}
				argv2[j] = NULL;
			}

		}
		// 혹시 이전에 에러나오면 명령어 수행하지 말기
		if(is_error == 1)
			continue;

		if (is_pipe) {
			// 파이프 생성
			if (pipe(pfd) == -1) {
				perror("pipe");
				exit(1);
			}

			// 첫 번째 자식 프로세스 생성
			if ((pid = fork()) == 0) {
				// 파이프 출력 연결
				close(pfd[0]); 
				dup2(pfd[1], 1);
				close(pfd[1]);

				// 첫 번째 명령 실행
				execvp(argv[0], argv);
				perror("execvp");
				exit(1);
			} else if (pid < 0) {
				perror("fork failed");
				exit(1);
			}

			// 두 번째 자식 프로세스 생성
			if ((pid = fork()) == 0) {
				// 파이프 입력 연결
				close(pfd[1]);
				dup2(pfd[0], 0);
				close(pfd[0]);

				// 두 번째 명령 실행
				execvp(argv2[0], argv2);
				perror("execvp");
				exit(1);
			} else if (pid < 0) {
				perror("fork failed");
				exit(1);
			}

			// 부모 프로세스: 파이프 닫기
			close(pfd[0]);
			close(pfd[1]);

			// 자식 프로세스 종료 대기
			wait(NULL);
			wait(NULL);
		}

		if(!is_pipe){
			// 명령어 수행 과정 
			// 자식 프로세스 생성
			pid = fork();

			if(pid == 0){
				// 자식 프로세스는 명령어 수행
				// 명령어 수행
				execute_command(narg, argv);
				perror("명령어 수행 불가");
				exit(1);
			}
			else if(pid > 0){

				// 백그라운드 작업이 아닐 경우 
				// 부모 프로세스는 자식의 동작을 대기
				if(is_background == 0)
					wait((int *)0);
				else
					printf("pid : %d\n", pid);
			}
			else{
				perror("fork failed");
			}

			// 리다이렉션 후 원래 파일 복구 및  파일 디스크립터 닫기
			if (fd != -1){

				if((dup2(fd_in, 0) == -1) || (dup2(fd_out, 1) == -1) || (dup2(fd_err, 2) == -1)){
					perror("dup2");
					exit(1);
				}

				if((close(fd) == -1) || (close(fd_in) == -1) || (close(fd_out) == -1) || (close(fd_err) == -1)) {
					perror("close");
					exit(1);
				}
			}

		}
	}
}

int getargs(char *cmd, char **argv){
	int narg = 0;

	while(*cmd){
		if(*cmd == ' ' || *cmd == '\t')
			*cmd++ = '\0';
		else{
			argv[narg++] = cmd++;
			while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
				cmd++;
		}
	}

	argv[narg] = NULL;
	return narg;
}

// 자식에게 신호 전달
void handler(int signo){
	if(pid > 0){
		kill(pid, signo);
	}
}
