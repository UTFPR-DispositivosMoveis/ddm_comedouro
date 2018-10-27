#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wiringSerial.h>
#include <wiringPi.h>

/*compilação
	gcc -Wall -o server server.c -lwiringPi
*/

#define MAX_CLIENT 		1
#define MAX_MSG_LEN 	1024
#define PORTA			12345

int fd; /* Porta serial */
char buffer[100]; /* buffer da serial*/
int ind; /* Controle de indice do buffer*/


void send_uart(char * data, int size);

int main(void)
{
	int socket_desc, conexao, c, len;
	struct sockaddr_in server, client;

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
			char * token = strtok(string, " ");
			char list[10][10];


			for(int i = 0; token != NULL; i++)
			{
				sprintf(list[i], "%s", token);
				token = strtok(NULL, " ");
			}

			char send[20]; /* buffer para mandar dados via UART */

			sprintf(string, "ERRO"); /* Caso a string não seja modificada deu erro*/

			/* verificar qual é a função solicitada pela APP*/
			if(!strcmp(list[0], "getStatus"))  /* Função de pegar valores do micro */
			{
				send[0] = 1;
				send[2] = ';';

				send_uart(send, 2);
			}
			else if(!strcmp(list[0], "alarme"))
			{
				char param1 = atoi(list[1]);
				char param2 = atoi(list[2]);
				send[0] = 2;
				send[1] = param1;
				send[2] = param2;
				send[3] = ';';

				send_uart(send, 4);
			}
			else if(!strcmp(list[0], "manual"))
			{
				char param = atoi(list[1]);
				send[0] = 3;
				send[1] = param;
				send[2] = ';';

				send_uart(send, 3);
			}
			else if(!strcmp(list[0], "buzzer"))
			{
				char param = atoi(list[1]);
				send[0] = 4;
				send[1] = param;
				send[2] = ';';

				send_uart(send, 3);				
			}



			/* Mandar um OK ou estado de erro de resposta */
			write(conexao , buffer , ind);

			close(conexao);
			puts("Conexao fechada");
		}
	}

	return 0;
}


void send_uart(char * data, int size)
{

	for(int i = 0; i < size; i++)
	{
		serialPutchar(fd, data[i]);
	}


	ind = 0;
	while(serialDataAvail(fd))
	{
		buffer[ind++] = serialGetchar(fd);
	}
	buffer[ind] = '\0';
}
