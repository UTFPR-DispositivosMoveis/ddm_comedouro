#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define n_connect 1

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
	 **/
	sck_cli = accept(sck_ser, (struct sockaddr *)&client, &c);

	if(sck_cli < 0){
		printf("Deu ruim\n");
		return -1;
	}

	/**
	 * Para descobrir o IP do cliente que se conectou, basta fazer uma
	 * manipulacao de deslocamento de bits na variavel de endereco no
	 * cliente.sin_addr.s_addr. Como é uma variavel de 32 bits, entao é preciso
	 * separar em lotes de 8 bits.
	 **/
	int a1, a2, a3, a4;
	a1 = client.sin_addr.s_addr >> 24;
	a2 = (client.sin_addr.s_addr << 8) >> 24;
	a3 = (client.sin_addr.s_addr << 16) >> 24;
	a4 = (client.sin_addr.s_addr << 24) >> 24;
	printf("O cliente conectado foi: %d.%d.%d.%d\n", a4, a3, a2, a1);

	/** Fase de envio e recebimento */
	while(tam_lei = recv(sck_cli, msg, sizeof(msg), 0) > 0){
		printf("Mensagem recebida pelo cliente %d.%d.%d.%d: %s\n", a4, a3, a2, a1, msg);
		msg[0] = '\0';
	}

	if(close(sck_ser) == -1){
		printf("Deu pau pra fechar o socket\n");
	}

	printf("Servidor Fechado\n");
	return 0;
}
