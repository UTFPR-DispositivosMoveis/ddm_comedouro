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
#include <signal.h>

/*compilação
	gcc -Wall -o server server.c -lwiringPi
*/

#define MAX_CLIENT 		1
#define MAX_MSG_LEN 	1024
#define PORTA			12345

int fd; /* Porta serial */
char buffer[100]; /* buffer da serial*/
int ind; /* Controle de indice do buffer*/

int socket_desc;
int conexao;

int send_uart(char * data, int size);

int main(void)
{
	int conexao, c, len;
	struct sockaddr_in server, client;

	/* Inicializando comunicação serial */
	if((fd = serialOpen("/dev/ttyS0", 9600)) < 0)
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
			printf("Li: %s\n", string);

			/* Parse da string recebida */
			char * token = strtok(string, " ");
			char list[10][10];


			for(int i = 0; token != NULL; i++)
			{
				sprintf(list[i], "%s", token);
				token = strtok(NULL, " ");
			}

			printf("%s \n", list[0]);

			char send[20]; /* buffer para mandar dados via UART */

			sprintf(string, "ERRO"); /* Caso a string não seja modificada deu erro*/

			/* verificar qual é a função solicitada pela APP*/
			if(!strcmp(list[0], "getstatus"))  /* Função de pegar valores do micro */
			{
				send[0] = 1;
				send[1] = ';';
//				write(conexao, "1 2 3 40 50", strlen("1 2 3 40 50"));
				printf("Requisicao de Status Recebida!!!\n");

				int x = send_uart(send, 2);
				if(x == -1){
					write(conexao, "fail", strlen("fail"));
					printf("Requisicao de Status falhou!!\n");
				}else{
					printf("Requisicao de Status com resposta: \n");
					int i;
					char ret[ind];
					for(i = 0; i < ind; i++){
						if(buffer[i] == 32){
							sprintf(ret,"%s ",ret);
						}else{
							sprintf(ret,"%s%d",ret, buffer[i]);
						}
					}
					printf("%s\n\n\n", ret);
					write(conexao, ret, strlen(ret));
				}
			}
			else if(!strcmp(list[0], "hora"))
			{
				char param1 = atoi(list[1]);
				char param2 = atoi(list[2]);
				char param3 = atoi(list[3]);
				send[0] = 2;
				send[1] = param1;
				send[2] = param2;
			//	send[3] = param3;
				send[3] = ';';

				printf("Requisicao de alteração de hora: hora: %d, indice: %d, tempo: %d\n", param1, param2, param3);

				int x = send_uart(send, 4);
				if(x == -1){
					write(conexao, "fail", strlen("fail"));
					printf("UART desconectada!!! \n");
				}else{
					if(strcmp(buffer, "1")){
						write(conexao, "ok", strlen("ok"));
						printf("Alteracao de hora realizada com sucesso!!!!\n");
					}else{
						write(conexao, "fail", strlen("fail"));
						printf("Erro ao configurar hora!!\n");
					}
				}
			}
			else if(!strcmp(list[0], "tratar"))
			{
				char param = atoi(list[1]);
				send[0] = 3;
				send[1] = param;
				send[2] = ';';
				int x =	send_uart(send, 3);

				printf("Requisicao para tratamento manual \n");

				if(x == -1){
					write(conexao, "fail", strlen("fail"));
					printf("UART não conectada\n");
				}else{
					if(strcmp(buffer, "1")){
						write(conexao, "ok", strlen("ok"));
						printf("Foi possivel tratar os peixes\n");
					}else{
						write(conexao, "fail", strlen("fail"));
						printf("Motor já ocupado!!\n");
					}
				}
			}
			else if(!strcmp(list[0], "buzzer"))
			{
				char param = atoi(list[1]);
				send[0] = 4;
				send[1] = param;
				send[2] = ';';
				int x;
				x = send_uart(send, 3);

				printf("Requisição para ligar o buzzer recebida \n");

				if(x == -1){
					write(conexao, "fail", strlen("fail"));
					printf("UART não conectada!! \n");
				}else{
					if(strcmp(buffer, "1")){
						write(conexao, "ok", strlen("ok"));
						printf("Requisicao para ligar o buzzer Ok\n");
					}else{
						printf("Buzzer ja ocupado!!\n");
					}
				}

			}

			/* Mandar um OK ou estado de erro de resposta */
//			write(conexao , buffer , ind);

			close(conexao);
			puts("Conexao fechada");
		}
	}
	serialClose(fd);
	return 0;
}


int send_uart(char * data, int size)
{

	for(int i = 0; i < size; i++)
	{
		serialPutchar(fd, data[i]);
		printf("data[%d]: %d\n",i,data[i]);
	}
		
	ind = 0;
	int x = 0;

/*	while(serialDataAvail(fd) == -1 && x < 5){
		sleep(1);
		x++;
	}

	if(x == 5){
		printf("Timeout de 5 sec na UART\n");
		return -1;
	}
*/
	do{
		buffer[ind] = serialGetchar(fd);
		printf("recebimento: %d, %d\n", buffer[ind], ind);
		if(buffer[ind] == -1){
			return -1;
		}
		ind++;
	}while(buffer[ind - 1] != ';');

	ind--;
	buffer[ind] = '\0';
		
	return 1;
}
