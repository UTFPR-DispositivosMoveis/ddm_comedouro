#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"

#define n_connect 1


void * connection_handler(void * param);
void  getIP(struct sockaddr_in client, char * IP);

int main(){
	/**
	 *	Primeiro se cria os inteiros que será a descricao do arquivo de sockets.
	 *	 sck_ser: Descritor do socket servidor
	 *	 sck_cli: Descritor do socket cliente
	 *	 c: guarda o tamanho da estrutura sockaddr_in
	 *	 tam_lei: Tamanho da leitura recebida pelo o cliente.
	 **/
	int sck_ser, sck_cli, c, tam_lei;
	struct sockaddr_in server, client;
	char msg[100];
	pthread_t thread_conn, thread_uart;
	thread_conn_t param_thread;

	/**
	 *	Primeiro, deve-se abrir o socket com a funcao: socket, e um inteiro deve
	 *	receber sua descricao.
	 *		socket(familia, tipo, protocolo)
	 *		Familia: PD_INET - Comunicacao entre endereco de internet
	 *		Tipo: SOCK_STREAM - Orientacao a conexao
	 *		Protocolo: 0 - Padrao TCP.
	 **/
	sck_ser = socket(PF_INET, SOCK_STREAM, 0);

	if(sck_ser == -1){
		printf("Deu pau no socket \n");
		return -1;
	}

	printf("Socket Servidor Aberto \n");
	server.sin_family = PF_INET; // Protocolo de internet
	server.sin_port = htons(12345); // Atribui a porta 1080 para ser a porta do socket.
	server.sin_addr.s_addr = INADDR_ANY; // Permite qualquer um fazer a conexao no socket.

	/**
	 * Segundo, o socket deve reservar um porta para ser usada, atraves da
	 * funcao bind.
	 * bind(sockid, &addrport, size)
	 * 	sockid: Descritor do socket
	 * 	addport: e um sockaddt_in convertido para ponteiro de sockaddr.
	 * 	size: Tamanho da estrutura que quero abrir no socket, uqe no caso é
	 * 	tamanho de server.
	 **/
	if(bind(sck_ser, (struct sockaddr *)&server, sizeof(server)) == -1){
		printf("Deu pau no bind\n");
		return -1;
	}
	printf("Sucesso no bind\n");

	/**
	 * Terceiro passo, o Listen.
	 * É preciso avisar ao Socket quantas conexões ele "ira querer pra ele".
	 * Para que ele faca isso, é preciso da funcao listen (de escutar).
	 * 		listen(sockid, TamanhoConexao)
	 * 			sockid: Descricao do socket do servidor.
	 * 			n_connect: Numero de conexao.
	 **/
	if(listen(sck_ser, n_connect) == -1){
		printf("Deu pau no Listen\n");
		return -1;
	}

	/**
	 * Espera-se o cliente se conectar, com accept, neste caso só gserá permitido
	 * uma conexao. Logo que essa conexao for feita e receber uma mensagem, o
	 * socket será fechado.
	 **/
	c = sizeof(struct sockaddr_in);

	/**
	 * Sock accept para aceitar conexoes do cliente.
	 * Funcao bloqueante.
	 * Aceita conexões e cria uma thread para tratar ela.
	 **/
	 int i = 0;
	while(sck_cli = accept(sck_ser, (struct sockaddr *)&client, &c) > 0)
	{
		param_thread.sock_addr = client;
		param_thread.conn_cli = sck_cli;
		printf("Cliente %d\n", sck_cli);
		if(pthread_create(&thread_conn, NULL, connection_handler, (void*)param_thread) < 0)
			puts("deu ruim na thread de conexao");
	}
}

void * connection_handler(void * param)
{
	thread_conn_t param_thread = (thread_conn_t) param;
	//int conn = * (int *) param;
	char message[1024];
	puts("Usuário Conectado");
	if(recv(param_thread.conn_cli, message, sizeof(message), 0) > 0)
	{
		char ip[30];
		getIP(param_thread.sock_addr, ip);
		printf("%s: %s\n", ip, message);
		if(send(param_thread.conn_cli, message, sizeof(message), 0) < 0)
			puts("Falha ao enviar o echo");
			
		message[0] ='\0';
	}
	else puts("Deu ruim na recepção");
	puts("Fim da conexao");
}


void  getIP(struct sockaddr_in client, char * IP)
{
	/**
	 * Para descobrir o IP do cliente que se conectou, basta fazer uma
	 * manipulacao de deslocamento de bits na variavel de endereco no
	 * cliente.sin_addr.s_addr. Como é uma variavel de 32 bits, entao é preciso
	 * separar em lotes de 8 bits.
	 **/
	char ip[30];
	int a1, a2, a3, a4;
	a1 = client.sin_addr.s_addr >> 24;
	a2 = (client.sin_addr.s_addr << 8) >> 24;
	a3 = (client.sin_addr.s_addr << 16) >> 24;
	a4 = (client.sin_addr.s_addr << 24) >> 24;
	sprintf(ip,"O cliente conectado foi: %d.%d.%d.%d\n", a4, a3, a2, a1);
}
