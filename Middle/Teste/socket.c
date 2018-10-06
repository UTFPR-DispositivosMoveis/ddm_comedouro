#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CLIENT 		1
#define MAX_MSG_LEN 	1024
#define PORTA			12345

int main(void) {
	int socket_desc, conexao, c, client_port, len;
	struct sockaddr_in server, client;


	/* Criando socket */
	if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1) {
		perror("Nao foi possivel criar o socket\n");
		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORTA);

	if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("Erro ao fazer bind!!! \n");
	}

	listen(socket_desc, 1);
	char string[MAX_MSG_LEN];

	c = sizeof(struct sockaddr_in);

	/*  Chamada bloqueante para aceitar conexões */
	while((conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
		if (conexao < 0) {
			perror("Erro ao receber conexao\n");
			return -1;
		}

		/* Lendo dados recebidos */
		if((len = read(conexao,string, MAX_MSG_LEN)) < 0) {
			perror("Erro ao receber dados do cliente: ");
			return -1;
		}

		string[len] = '\0';
		if(!strcmp("getstatus", string)){
			// Um novo padrao de retorno...hora1 hora2 hora3 bateria nivelRacao
			printf("Mandando status: 1 2 3 40 50\n");
			write(conexao,"1 2 3 40 50", strlen("1 2 3 40 50"));
		}else{
			printf("Dado recebido: %s\n", string);
			write(conexao , "ok" , strlen("ok"));
		}

		close(conexao);
		puts("Conexao fechada");
	}
	return 0;
}
