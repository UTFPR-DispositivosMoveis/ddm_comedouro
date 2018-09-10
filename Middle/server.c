#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include<pthread.h>

#define MAX_MSG 1024
#define PORTA 12345

 
 
void * conection_handler(void * x);

int main(void){
	//variaveis
	int socket_desc , conexao , c;
	struct sockaddr_in server,client;
	char *mensagem;   
    char resposta[MAX_MSG];
	int tamanho, count;

	// destinado a armazenar o IP e porta do cliente  
	char *client_ip;
	int client_port;

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) {
		printf("Nao foi possivel criar o socket\n");
		return -1;
	}
     
	server.sin_family = AF_INET; // AF_INET = ARPA INTERNET PROTOCOLS
	server.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
	server.sin_port = htons(PORTA);
     
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		puts("Erro ao fazer bind!!! \n");
	}
	puts("Bind efetuado com sucesso!!! \n");

        // Passo 2: aguarda por conexões de clientes
        // O segundo parametro indica quantos conexoes podem aguardar no buffer
	listen(socket_desc, 1);
	/*********************************************************/
	pthread_t thread_id;
	
	//Aceitando e tratando conexoes
	puts("Aguardando por conexoes...");
	c = sizeof(struct sockaddr_in);
	// Fica esperando por conexoes
	while( (conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){;
		if (conexao<0){
			perror("Erro ao receber conexao\n");
			return -1;
		}

		if(pthread_create(&thread_id, NULL, conection_handler, (void*)&conexao) < 0)
		{
			puts("Thred faio");
			return -1;
		}
/*
	}//fim do while

	/*********************************************************/


	printf("Servidor finalizado...\n");
	return 0;
}




void * conection_handler(void * x)
{
	int conexao = *(int *) x;
	char mensagem[1024];
	while(recv(conexao, mensagem, sizeof(mensagem), 0) > 0)
	{
		printf("%s\n", mensagem);
		mensagem[0] ='\0';
	}
}
