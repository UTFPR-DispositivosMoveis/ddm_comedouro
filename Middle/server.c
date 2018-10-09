#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_CLIENT 		1
#define MAX_MSG_LEN 	1024
#define PORTA			12345

int main(void) {
	int socket_desc, conexao, c, client_port, len;
	struct sockaddr_in server, client;
	pthread_t uart;
	
	if(pthread_create(&uart, NULL, uart_receiver, NULL) < 0) {
		perror("Erro ao criar a thread da uart");
		return -1;
	}
	
	/* Criando socket */
	if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1) {
		perror("Erro ao  criar o socket\n");
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
		if (conexao < 0) {										/* Verificar se a conexao deu certo */
			perror("Erro ao receber conexao\n");
		} else if((len = read(conexao,string, MAX_MSG_LEN)) < 0) {				/* Lendo dados recebidos */
			perror("Erro ao receber dados do cliente: ");
		} else {
		
			string[len] = '\0';
			printf("Li: %s", string);
			len = sprintf(string, "OK");
			
			/* Parse da string recebida */
			char * token = stktok(string, " ");
			char list[10][10];
			
			
			for(int i = 0; token != NULL; i++) {
				list[i] = token;
				token = strtok(NULL, " ");
			}

			
			/* Mandar um OK de resposta */
			write(conexao , string , strlen(string));

			close(conexao);
			puts("Conexao fechada");
		}
	}
	
	return 0;
}
