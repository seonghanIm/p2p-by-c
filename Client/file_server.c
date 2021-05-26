#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

/* 
반드시 새 스레드에서 실행할 것. 그렇지 않으면 종료될 수 있음.
각 스레드는 exit가 아닌 return으로 종료해야 함. exit 는 "프로세스"를 종료 시키는 함수임.
*/

//
int file_send(char* file_name) { 
	int serv_sd, clnt_sd;
	FILE* fp;
	char* file_location;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
		
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sd == -1) {
	}
		error_handling("소켓 생성 오류.");

	// serv_adr 구조체 작성
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(10982); // port = 10982

	// 구조체를 토대로 바인딩
	if (bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr) == -1))
		error_handling("바인딩 실패.");

	// listen 상태
	if (listen(serv_sd, 5) == -1)
		error_handling("listen 에러 발생.");

	// accpet 상태
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if (clnt_sd == -1)
		error_handling("연결이 거부되었습니다.");

	// 파일이 있는지 여부를 확인
	file_location = strcat("file\\", file_name); // 경로 지정. 폴더 이름은 file.
	fp = fopen(file_location, "rb")
		if (!fp) {
			fclose(fp);
			error_handling("해당 파일을 불러올 수 없습니다. 프로그램을 재실행하시기 바랍니다.");
		}

	// 데이터 전송
	fputs("데이터 전송을 시작합니다...\n", stdout);

	while (1)
	{
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (read_cnt < BUF_SIZE)
		{
			write(clnt_sd, buf, read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}

	fputs("데이터 전송이 완료되었습니다.")
}


// 