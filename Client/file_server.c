#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

/* 
�ݵ�� �� �����忡�� ������ ��. �׷��� ������ ����� �� ����.
�� ������� exit�� �ƴ� return���� �����ؾ� ��. exit �� "���μ���"�� ���� ��Ű�� �Լ���.
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
		error_handling("���� ���� ����.");

	// serv_adr ����ü �ۼ�
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(10982); // port = 10982

	// ����ü�� ���� ���ε�
	if (bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr) == -1))
		error_handling("���ε� ����.");

	// listen ����
	if (listen(serv_sd, 5) == -1)
		error_handling("listen ���� �߻�.");

	// accpet ����
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if (clnt_sd == -1)
		error_handling("������ �źεǾ����ϴ�.");

	// ������ �ִ��� ���θ� Ȯ��
	file_location = strcat("file\\", file_name); // ��� ����. ���� �̸��� file.
	fp = fopen(file_location, "rb")
		if (!fp) {
			fclose(fp);
			error_handling("�ش� ������ �ҷ��� �� �����ϴ�. ���α׷��� ������Ͻñ� �ٶ��ϴ�.");
		}

	// ������ ����
	fputs("������ ������ �����մϴ�...\n", stdout);

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

	fputs("������ ������ �Ϸ�Ǿ����ϴ�.")
}


// 