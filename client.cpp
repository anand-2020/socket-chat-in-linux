/* 
open two terminals
terminal 1: enter following command
tty

terminal 2: enter following commands
g++ client.cpp -o client -lpthread
./client <host_name> <port_no> > <output-of-tty-in-terminal-1>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 256
pthread_mutex_t stdout_mutex;

void *recieve_msg(void *arg){
    int sock_fd = *(int*)arg;
    char buffer[BUFF_SIZE];
    int msg_status;
    
    while(1){
    	bzero(buffer, BUFF_SIZE);
		msg_status = read(sock_fd, buffer, BUFF_SIZE);
		if(msg_status < 0){
			perror("ERROR: on reading\n");
		}		

		
		if(strncmp(buffer, "SERVER: DISCONNECTED", 20) == 0) {
		printf("\033[1;31m");
            msg_status = write(sock_fd, "### dummy message\n", BUFF_SIZE);
		    if(msg_status < 0){
			    perror("ERROR: on writing\n");
		    }
        }

		if(strncmp(buffer, "SERVER: CONNECTED", 17) == 0) {
			printf("\033[1;32m");
            msg_status = write(sock_fd, "### dummy message\n", BUFF_SIZE);
		    if(msg_status < 0){
			    perror("ERROR: on writing\n");
		    }
        }

		pthread_mutex_lock(&stdout_mutex);
		printf("%s\033[0m\n", buffer);
		pthread_mutex_unlock(&stdout_mutex);
		
    }
    
    return 0;
}

void *send_msg(void *arg){
    int sock_fd = *(int*)arg;
    char buffer[BUFF_SIZE];
    int msg_status;
    
    while(1){

    	bzero(buffer, BUFF_SIZE);
    	fgets(buffer, BUFF_SIZE, stdin);

		msg_status = write(sock_fd, buffer, BUFF_SIZE);
		if(msg_status < 0){
			perror("ERROR: on writing\n");
		}

		pthread_mutex_lock(&stdout_mutex);
		printf("YOU: %s\n", buffer);
		pthread_mutex_unlock(&stdout_mutex);
		
	 	if(strcmp(buffer, "close\n") == 0) break;
    }
    
    return 0;
}

int socket_setup(char *arguments[]){
	int sock_fd, port_no ;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	port_no = atoi(arguments[2]);

	server = gethostbyname(arguments[1]);
	if(server == NULL){
		perror("ERROR: host does not exists\n");
		exit(1);
	}
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0){
		perror("ERROR: on opening socket\n");
		exit(1);
	}
	

	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port_no);
	
	if(connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR: on connection\n");
		exit(1);
	}

	printf("\n\033[1;31m---- IMPORTANT INSTRUCTIONS ----\033[0m\n");
	printf("Do NOT type anything in this terminal.\nThis terminal is for viewing chat nessages\n");
	printf("To send any command/message use the other terminal through which you executed client program\n");
	printf("\033[1;31m------------------------------------\033[0m\n\n");

	return sock_fd ;
}

int main(int argc , char *argv[]){
	if(argc < 3){
		printf("Enter %s <host_name> <port_no>\n", argv[0]);
		exit(1);
	}

	int sock_fd = socket_setup(argv);
	
    printf("\033[1;32mConnected to server.\033[0m\n\n" );

	pthread_mutex_init(&stdout_mutex, NULL); 
        
    pthread_t th_read, th_write;
	 
    if( pthread_create(&th_read, NULL, &recieve_msg, &sock_fd) != 0 ){
		perror("ERROR: Thread creation for recieving messages failed\n");
		exit(1);
	}

    if( pthread_create(&th_write, NULL, &send_msg, &sock_fd) != 0 ){
		perror("ERROR: Thread creation for sending messages failed\n");
		exit(1);
	}
        
    if( pthread_join(th_write, NULL) != 0 ){
		perror("ERROR: Thread joining of sending messages failed\n");
		exit(1);
	}

	/* 
	thread for sending messages has been joined. It means client wants to end connection.
	So forcefully cancel the thread for recieving messages.
	*/
    if( pthread_cancel(th_read) != 0 ){
		perror("ERROR: Thread cancellation of recieving messages failed\n");
		exit(1);
	}
	printf("\033[1;31mSession Ended.\033[0m\n" );
        
	close(sock_fd);
	
	return 0;
}

