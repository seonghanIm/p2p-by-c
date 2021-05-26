#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
--------------------
���� : ./Client <IP> <port> <name>

--------------------
*/

int main(int argc, char* argv[]) {
	
	int sock;
	char command[];
	struct sockaddr_in serv_addr;

	// argv �� �˻�
	if (argc < 4)
		error_handling("���� : ./Client <IP> <port> <name>");

	// TCP ���� ����
	// ���� ����
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("���� ���� ����.");

	// ���� ������ ���� ����ü ���� �� ����
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == -1)
		error_handling("���� ����. ���� �ּ� �Ǵ� ��Ʈ�� ��Ȯ�� �Է��ߴ��� Ȯ�����ּ���.");
	
	// �ʿ��� ������ �ۼ����ϴ� �ڵ�
	char name[32] = argv[3];
	/* Ŭ�� ���� ������ ���ڿ��� ���� 
	char name[32];
	char file_name[100][256];
	*/

	// �����κ��� �޼����� �޴� �ڵ�


	fputs("�� ���α׷��� �����Ų �� file ���� �� ������ �̵�/���Ÿ� ���� ���ñ� �ٶ��ϴ�.\n", stdout);
	
	// ��ɾ� ���� �ڵ�
	while (1) {
		fputs("(", stdout);
		fputs(name, stdout);
		fputs(")>", stdout);

		scanf_s("%s", command);
		if (strcmp("request", command)) {	// request -> �������� int 1 �� ����.
			
		} 

		else if (strcmp("list", command)) { // list -> �������� int 2 �� ����.

		}

		else if (strcmp("help", command)) {
			fputs("<>�� �ʼ��� �Է��ؾ� �ϰ�, ()�� ���ÿ� ���� �Է��� �� �ֽ��ϴ�.\n", stdout);
			fputs("request <���� �̸�> <���ϳ��> : �ش� �����κ��� Ư�� ������ ��û�մϴ�. \n", stdout);
			fputs("list (���� �̸�) : �ش� ������ ���� ���� �̸��� �����մϴ�. ", stdout);
			fputs("���� �̸��� ���� ��� ������ ��� ������ �̸��� �����մϴ�. \n", stdout);
			fputs("quit : ������ �����մϴ�.");
		}
		else if (strcmp("quit", command) {
			close(sock); // �����δ� ������ close
			exit(0);
		}

		else
			fputs("�������� �ʴ� ��ɾ��Դϴ�. help�� �Է����ּ���.", stdout);
	}

	return 0;
}

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}