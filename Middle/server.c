/*
 * gcc -Wall -o server.c server.run -pthread -lwiringPi
 * 
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX_MSG 1024
#define PORTA 12345

pthread_mutex_t lock_file;
char buffer_uart[MAX_MSG];
int serial_port, buffer_index;
FILE *configs;

void * thread_conexao_recebe(void * param);
void * thread_uart(void * param);

int main(void)
{
	pthread_t conn, uart;


	/*  Criar a thread de comunicação com o APP via sockets TCP/IP   */
	if (pthread_create(&conn, NULL, thread_conexao_recebe, NULL) < 0) {
		perror("Não foi possível criar a  thread de communicação");
	}
	
	if (pthread_create(&uart, NULL, thread_uart, NULL) < 0) {
		perror("Não foi possível criar a  thread da uart");
	}

	configs = fopen("config.base", "r+");

	if(pthread_mutex_init(&lock_file, NULL) != 0) {
		perror("Não foi possível criar o mutex");
	}

	if ((serial_port = serialOpen ("/dev/ttyS0", 115200)) < 0)	/* open serial port */
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}

	

	while(1)
	{
		//puts("Main thread rodando");
		//sleep(30);
	}
	
	pthread_join(conn, NULL);
	return 0;
		
}


void * thread_conexao_recebe(void * param)
{

	int socket_desc , conexao , c, client_port, tamanho, count;
	struct sockaddr_in server,client;
	char *client_ip;

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) {
		perror("Nao foi possivel criar o socket\n");
		return NULL;
	}

	server.sin_family = AF_INET; // AF_INET = ARPA INTERNET PROTOCOLS
	server.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
	server.sin_port = htons(PORTA);

	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("Erro ao fazer bind!!! \n");
	}

	listen(socket_desc, 1);
	char string[MAX_MSG];
	//Aceitando e tratando conexoes
	c = sizeof(struct sockaddr_in);

	/*schema: Conecta, recebe parâmetros e ai fora*/
	while( (conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
		if (conexao<0) {
			perror("Erro ao receber conexao\n");
			return NULL;
		}

		//Passo 3: realizar a comunicação
		// pegando IP e porta do cliente
		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);

		// lendo dados enviados pelo cliente
		if((tamanho = read(conexao,string, MAX_MSG)) < 0) {
			perror("Erro ao receber dados do cliente: ");
			return NULL;
		}
		
		string[tamanho] = '\0';
		/*
		char * tokens[40];
		int i = 0, tam = 0;
		
		/*Começa o tratameno da requisição mandada*/
		/*
		tokens[0] = strtok(string, " ");
		printf("%s", tokens[0]);
		while(tokens[i] != NULL)
		{
			i++;
			tokens[i] = strtok(NULL, " ");
			printf("%s", tokens[i]);
		}

		tam = i;

		if(!strcmp("set", tokens[0])) 
		{
			
			for(i = 1; i < tam; i++)
				printf("\nSetando parametro %s", tokens[i]);
		}
		else if(!strcmp("get", tokens[0])) 
		{
			
			printf("\nGetando parâmetros\n");
		}
		else puts("Deu ruim");
		*/
		
		
		
		int i = 0;
		/* Envia a string recebida para o microcontrolador */
		serialPuts(serial_port, string);
		/* E recebe a resposta do microcontrolador */
		//while(buffer_uart[i++] = serialGetchar(serial_port)) {}
		
		/* Enviando resposta para o cliente */
		write(conexao , string , strlen(string));


		/*****termina a conexão com o cliente*/
		close(conexao);
		puts("Conexao fechada");
	}

}

void * thread_uart(void * param) {

	while(1) {
		while(serialDataAvail(serial_port) == -1) {}
		while(buffer_uart[buffer_index++] = serialGetchar(serial_port)) {}
		
		/* Deixa travado pra somente essa thread poder utilizar o arquivo */
		pthread_mutex_lock(&lock_file);
		configs
	}
}
