#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MSG 1024
#define PORTA 12345

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

	// Associar (bind) o socket a todos IPs locais
        // e a PORTA ESPECIFICA para os clientes se conectarem
	server.sin_family = AF_INET; // AF_INET = ARPA INTERNET PROTOCOLS
	server.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
	server.sin_port = htons(PORTA);

	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		puts("Erro ao fazer bind!!! \n");
	}


        // Passo 2: aguarda por conexões de clientes
        // O segundo parametro indica quantos conexoes podem aguardar no buffer
	listen(socket_desc, 3);
	/*********************************************************/

	//Aceitando e tratando conexoes
	c = sizeof(struct sockaddr_in);
	// Fica esperando por conexoes
	while( (conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){;
		if (conexao<0){
			perror("Erro ao receber conexao\n");
			return -1;
		}

		//Passo 3: realizar a comunicação
		// pegando IP e porta do cliente
		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);

		// lendo dados enviados pelo cliente
		if((tamanho = read(conexao,resposta, MAX_MSG)) < 0){
			perror("Erro ao receber dados do cliente: ");
			return -1;
		}

		// Coloca terminador de string
		resposta[tamanho] = '\0';
		printf("%s[%d]: %s \n", client_ip, client_port, resposta);

		// Enviando resposta para o cliente
		write(conexao , resposta , strlen(resposta));


		//Passo 4: Encerrar conexão
		close(conexao);
	}//fim do while

}
