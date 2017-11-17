#include "functions.c"

int main()
{
	 /* GPIO STARTUP */
	wiringPiSetup();
	pinMode(LEDpin, OUTPUT);
	digitalWrite(LEDpin, LOW);
	
	/* Socket setup */
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(socketPort);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		  sizeof(serv_addr)) < 0) 
		  error("ERROR on binding");
	
	while(1) {
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
				 (struct sockaddr *) &cli_addr, 
				 &clilen);
		if (newsockfd < 0) 
		  error("ERROR on accept");
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
		
		if(strcmp(buffer,"on") == 0) {
			printf("sending on code\n");
			IRsendCode(onCode);
			delay(1000);
		} else if (strcmp(buffer,"off") ==0) {
			printf("sending off code\n");
			IRsendCode(offCode);
			delay(1000);
		} else if (strcmp(buffer,"toggle") == 0) {
			printf("sending toggle code\n");
			IRsendCode(toggleCode);
			delay(1000);
		} else {
			printf("unaccepted command\n");
		}
	}
	
	/* need to have a server side input to quit */
	close(newsockfd);
	close(sockfd);
	return 0;
}
