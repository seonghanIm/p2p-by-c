#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256 // 클라이언트 접속 최대

void *handle_clnt(void *arg); // 클라이언트 연결 후 쓰레드를 실행하여 handle_clnt 실행
void error_handling(char *msg);

struct Client_info // 클라이언트의 정보를 담을 구조체 연결리스트
{
	struct Client_info *next; // 다음 노드의 주소를 저장할 포인터 변수
	char name[32];			  // 클라이언트 이름
	int IP[256];			  // 클라이언트 ip 주소
	char file_name[100][256]; // 파일 인덱스와 이름
} Client_info;

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
int clnt_num = 0;
struct Client_info clnt[100];
pthread_mutex_t mutx;
pthread_t t[100];

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	printf("최대 허용 클라이언트 인원을 입력하세요 : ");
	scanf("%d", &clnt_cnt); // 최대허용 클라 결정
	if (argc != 2)			// port번호 입력을 하지 않으면 에러 출력
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);			 // 뮤텍스 동적 초기화
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 서버 소켓 생성(TCP)

	memset(&serv_adr, 0, sizeof(serv_adr));		  // 서버 주소 0으로 초기화
	serv_adr.sin_family = AF_INET;				  //구조체 설정
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network long
	serv_adr.sin_port = htons(atoi(argv[1]));	  // host to nework short / 아스키 to integer

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) // 서버 소켓 바인드
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1) // listen
		error_handling("listen() error");

	while (1)
	{
		int str_len2;
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr,clnt_adr_sz); // 클라소켓 accept
		//read(clnt_sock, &clnt[clnt_num], sizeof(clnt[clnt_num]));

		pthread_mutex_lock(&mutx); // 뮤텍스 lock 을 통해 다중 접속시 일어날 수 있는 동기화 문제 해결
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
		clnt_num++;
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
		pthread_create(&t[clnt_num], NULL, handle_clnt, (void *)&clnt_sock); // 클라이언트 함수 시작

		pthread_detach(t[clnt_num - 1]);
	}
	close(serv_sock);
	return 0;
}

void *handle_clnt(void *arg)
{
	int clnt_sock = *((int *)arg);
	int str_len = 0;
	int msg;
	printf("connected clnt number : %d", clnt_num);

	str_len = read(clnt_sock, &msg, sizeof(msg));
	while (1)
	{

		switch (msg)
		{

		case 0:
			printf("클라이언트 %d 종료\n", clnt_num);
			continue;

		case 1:
			// request();
			break;
		case 2:
			//list();
			break;

		default:
			printf("Clnt put wrong number\n");
		}
	}
	return NULL;
}

//int request()
//{
//}
void list(struct Client_info *list)   ///탐색 코드
{
	struct Client_info *cur = list->next;
	printf("[");
	while (cur != NULL)
	{
		printf("클라이언트 이름 : %s\n", cur->name);
		printf("클라이언트 IP 주소 : %d\n", cur->IP);
		printf("클라이언트 파일인덱스 번호,이름 : %c\n", cur->file_name);
		cur = cur->next;
	}
	printf("]");
}
