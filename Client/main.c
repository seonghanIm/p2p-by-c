#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
--------------------
사용법 : ./Client <IP> <port> <name>

--------------------
*/

int main(int argc, char* argv[]) {
	
	int sock;
	char command[];
	struct sockaddr_in serv_addr;

	// argv 수 검사
	if (argc < 4)
		error_handling("사용법 : ./Client <IP> <port> <name>");

	// TCP 연결 과정
	// 소켓 생성
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("소켓 생성 오류.");

	// 서버 연결을 위한 구조체 생성 후 접속
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
		error_handling("접속 오류. 서버 주소 또는 포트를 정확히 입력했는지 확인해주세요.");
	
	// 필요한 정보를 송수신하는 코드
	char name[32] = argv[3];
	/* 클라에 관한 정보를 문자열로 전송 
	char name[32];
	char file_name[100][256];
	*/

	// 서버로부터 메세지를 받는 코드


	fputs("이 프로그램을 실행시킨 후 file 폴더 안 파일은 이동/제거를 하지 마시기 바랍니다.\n", stdout);
	
	// 명령어 실행 코드
	while (1) {
		fputs("(", stdout);
		fputs(name, stdout);
		fputs(")>", stdout);

		scanf_s("%s", command);
		if (strcmp("request", command)) {	// request -> 서버에게 int 1 을 전달.
			
		} 

		else if (strcmp("list", command)) { // list -> 서버에게 int 2 를 전달.

		}

		else if (strcmp("help", command)) {
			fputs("<>는 필수로 입력해야 하고, ()는 선택에 따라 입력할 수 있습니다.\n", stdout);
			fputs("request <유저 이름> <파일노드> : 해당 유저로부터 특정 파일을 요청합니다. \n", stdout);
			fputs("list (유저 이름) : 해당 유저가 가진 파일 이름을 열람합니다. ", stdout);
			fputs("유저 이름이 없을 경우 접속한 모든 유저의 이름을 열람합니다. \n", stdout);
			fputs("quit : 접속을 종료합니다.");
		}
		else if (strcmp("quit", command) {
			close(sock); // 실제로는 서버가 close
			exit(0);
		}

		else
			fputs("존재하지 않는 명령어입니다. help를 입력해주세요.", stdout);
	}

	return 0;
}

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}