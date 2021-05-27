#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/uio.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

struct Client_info // 클라이언트의 정보를 담을 구조체 연결리스트
{
	struct Client_info *next; // 다음 노드의 주소를 저장할 포인터 변수
	char name[32];			  // 클라이언트 이름
	int IP[256];			  // 클라이언트 ip 주소
	char file_name[100][256]; // 파일 인덱스와 이름
} Client_info;
void list(struct Client_info *list);
void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;
pthread_t t[100];

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t[clnt_cnt], NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(t[clnt_cnt]);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void *handle_clnt(void *arg)
{
	int clnt_sock = *((int *)arg);
	int str_len = 0, i;
	char msg[BUF_SIZE];
	int num;
	char clnt_name[32];
	int clnt_IP[256];
	char clnt_file_name[100][256];
	struct Client_info clnt;

	if (readv(clnt_sock, clnt_name, sizeof(clnt_name)) == -1)
	{
		error_handling("read() error");
	}

	if (readv(clnt_sock, clnt_IP, sizeof(clnt_IP)) == -1)
	{
		error_handling("read() error");
	}

	if (readv(clnt_sock, clnt_file_name, sizeof(clnt_file_name)) == -1)
	{
		error_handling("read() error");
	}

	if (read(clnt_sock, num, sizeof(num)) == -1)
	{
		error_handling("read() error");
	}
	for (int i = 0; i < 32; i++)
	{
		clnt.name[32] = clnt_name[32];
	}
	for (int i = 0; i < 256; i++)
	{
		clnt.IP[256] = clnt_IP[256];
	}
	for (int i = 0; i < 100; i++)
		for (int k = 0; k < 256; k++)
		{
			clnt.file_name[i][k] = clnt_file_name[i][k];
		}

	while (1)
	{

		switch (num)
		{
		case 0:
			printf("클라이언트 %d 종료\n", clnt_cnt);
			break;
		case 1:
			// request();
			continue;
		case 2:
			//list();
			continue;
		default:
			printf("Clnt put wrong number\n");
		}
	}

	//while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) //클라이언트에게 메세지 전송
	//send_msg(msg, str_len);

	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++) // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (i++ < clnt_cnt - 1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void list(struct Client_info *list)
{
	struct Client_info *cur = list->next;
	printf("[");
	while (cur != NULL)
	{
		send_msg(cur->name, sizeof((*cur->name)));
		send_msg(cur->IP, sizeof((*cur->IP)));
		send_msg(cur->name, sizeof((*cur->name)));
		cur = cur->next;
	}
	printf("]");
}

void send_msg(char *msg, int len) // send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}