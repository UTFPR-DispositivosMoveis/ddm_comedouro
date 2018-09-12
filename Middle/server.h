#ifndef __server_
#define __server_
	
typedef struct _thread_conn_t_
{
	struct sockaddr_in sock_addr;
	int conn_cli;
}thread_conn_t;
#endif
