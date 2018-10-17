#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringSerial.h>

/*compilação 
	gcc -Wall server server.c -o -lwiringPi -lpthread
*/

#define MAX_CLIENT 		1
#define MAX_MSG_LEN 	1024
#define PORTA			12345

int fd; /* Porta serial */
FILE * configs; /* Ponteiro para o arquivo de configurações */


char uart_send(char * data, int size);
void uart_receiver(void * param);

int main(void)
{
	int socket_desc, conexao, c, client_port, len;
	struct sockaddr_in server, client;
	pthread_t uart;
	

	/* Thread de recepção dos dados da uart */
	if(pthread_create(&uart, NULL, uart_receiver, NULL) < 0) 
	{
		perror("Erro ao criar a thread da uart");
		return -1;
	}

	/* Inicializando comunicação serial */
	if((fd = serialOpen("/dev/ttyS0", 115200)) < 0) 
	{
		perror("Erro ao abrir serial");
	}
	
	/* Criando socket */
	if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1) 
	{
		perror("Erro ao  criar o socket\n");
		return -1;
	}	

	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(PORTA);

	if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) 
	{
		perror("Erro ao fazer bind!!! \n");
	}

	listen(socket_desc, 1);
	char string[MAX_MSG_LEN];
	
	c = sizeof(struct sockaddr_in);

	/*  Chamada bloqueante para aceitar conexões */
	while((conexao = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) 
	{
		if (conexao < 0) 
		{										/* Verificar se a conexao deu certo */
			perror("Erro ao receber conexao\n");
		} 
		else if((len = read(conexao,string, MAX_MSG_LEN)) < 0) 
		{				/* Lendo dados recebidos */
			perror("Erro ao receber dados do cliente: ");
		} 
		else 
		{
			string[len] = '\0';
			printf("Li: %s", string);
			len = sprintf(string, "OK");
			
			/* Parse da string recebida */
			char * token = stktok(string, " ");
			char list[10][10];
			
			
			for(int i = 0; token != NULL; i++) 
			{
				list[i] = token;
				token = strtok(NULL, " ");
			}

			char send[20]; /* buffer para mandar dados via UART */
			char answer = 0; /* Resposta  da uart*/

			sprintf(string, "ERRO"); /* Caso a string não seja modificada deu erro*/

			/* verificar qual é a função solicitada pela APP*/
			if(!strcmp(list[0], "get"))  /* Função de pegar valores do micro */
			{
				if(!strcmp(list[1], "1"))  /*  Pegar os valores dos parâmtros que estão salvos no arquivo*/
				{  
					/* Semafaro e */
				} 
				else /* Pegar os valores dos sensores do microcontrolador */
				{ 
					send[0] = 1;
					send[1] = 2;
					send[2] = ';';

					if(send_uart(send, 3)) 
					{
						sprintf(string, "OK");
					}
				}
			} 
			else if(!strcmp(list[0], "hora")) 
			{
				char param1 = atoi(list[1]);
				char param2 = atoi(list[2]);
				send[0] = 2;
				send[1] = param1;
				send[2] = param2;
				send[3] = ';';

				if(send_uart(send, 4)) 
				{
					sprintf(string, "OK");
				}
			} 
			else if(!strcmp(list[0], "tempo")) 
			{
				char param = atoi(list[1]);
				send[0] = 3;
				send[1] = param;
				send[2] = ';';
				
				if(send_uart(send, 3)) 
				{
					sprintf(string, "OK");
				}
			} 
			else if(!strcmp(list[0], "manual")) 
			{
				char param = atoi(list[1]);
				send[0] = 4;
				send[1] = param;
				send[2] = ';';
				
				if(send_uart(send, 3)) 
				{
					sprintf(string, "OK");
				}
			} 
			else if(!strcmp(list[0], "buzzer")) 
			{
				char param = atoi(list[1]);
				send[0] = 5;
				send[1] = param;
				send[2] = ';';

				if(send_uart(send, 3)) 
				{
					sprintf(string, "OK");
				}
			}

			/* Mandar um OK ou estado de erro de resposta */
			write(conexao , string , strlen(string));

			close(conexao);
			puts("Conexao fechada");
		}
	}
	
	return 0;
}


char uart_send(char * data, int size) 
{

	for(int i = 0; i < size; i++) 
	{
		serialPutc(fd, data[i]);
	}

	return serialGet(fd);
}

void uart_receiver(void * param)
{

	while(1)
	{
		/*Se dados disponíveis atualiza o arquivo, utilizar semáforo para controle de acesso ao arquivo*/
	}
}