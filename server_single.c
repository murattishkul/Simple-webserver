#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <string.h>

#define PORT 5000

char *ROOT;//ROOT is just a string
void respond (int sock); // function to implement

void sendall(int sock, char* msg) { //sending bytes to client
  int length = strlen(msg);
  int bytes;
  while(length > 0) {
     printf("send bytes : %d\n", bytes); 
    bytes = send(sock, msg, length, 0);
    length = length - bytes;
  }
}



char * trueurl (char * url){
	char ans[500];
        strcpy(ans, ROOT);
        if(strcmp(url,"/")==0) strcat(ans, "/index.html");
        else strcat(ans, url);
	return ans;
}
 

char * typeOfFile(char * url){
        char ans[500];
	strcpy(ans, ROOT);
        if(strcmp(url,"/")==0) strcat(ans, "/index.html");
        else strcat(ans, url);
	char type[6];int flag = 0;int count = 0;int i=0;
	for(i;i<strlen(ans);i++){
		if(flag==1){
			type[count] = ans[i];
			count++;
		}
		if(ans[i]=='.')
			flag = 1;
	}
	type[count] = '\0';
	if(strcmp(type,"html") == 0 || strcmp(type,"js") == 0 || strcmp(type,"css") == 0 || strcmp(type,"jpeg")==0 || strcmp(type,"jpg")==0 ){
	} else strcpy(type,"none");	
	return type;
}


void bad(int sock, char * msg){
    char text[] = "<html> <head> <title> 404 Not Found </title> </head> <body> <h1>Not Found </h1> <p> The requested URL was not found. </p> </body> </html>";
    int lenr = strlen(text);
    char header[1000];
strcpy( header,"HTTP/1.1 404 Not Found\r\nConnection: closed\r\nContent-Type: text/html;\r\nContent-length: ");
    char lenstr[40];
    sprintf(lenstr, "%d", lenr);
    strcat(header, lenstr);
    strcat(header, "\r\n\r\n");
    sendall(sock, header);
    sendall(sock, text);
}

int main( int argc, char *argv[] ) {
  int newsockfd[50];
  int sockfd, portno = PORT;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  clilen = sizeof(cli_addr);
  ROOT = getenv("PWD"); //root is where we now

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);//initate the socket

  if (sockfd < 0) { //socket error handling
    perror("ERROR opening socket");
    exit(1);
  }

  // port reusable
  int tr = 1;//fiction
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* TODO : Now bind the host address using bind() call.*/
  if ( bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 ){
    perror("bind error");
    exit(1);
  }

  /* TODO : listen on socket you created */

  if ( listen(sockfd, 20) == -1 ){
    perror("listen error");
    exit(1);
  }

  printf("Server is running on port %d\n", portno);
  int client_count = 0;
  while (1) {
    newsockfd[client_count] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if ( newsockfd[client_count] == -1 ){
      perror("accept error");
      exit(1);
    }
    respond(newsockfd[client_count]);
    client_count++;
  }

  return 0;
}

void respond(int sock) {
int offset, bytes;
  char buffer[9000];
  bzero(buffer,9000);

  offset = 0;
  bytes = 0;
  do {
    // bytes < 0 : unexpected error
    // bytes == 0 : client closed connection
    bytes = recv(sock, buffer + offset, 1500, 0);
    offset += bytes;
    // this is end of http request
    if (strncmp(buffer + offset - 4, "\r\n\r\n", 4) == 0) break;
  } while (bytes > 0);

  if (bytes < 0) {
    printf("recv() error\n");
    return;
  } else if (bytes == 0) {
    printf("Client disconnected unexpectedly\n");
    return;
  }

  buffer[offset] = 0;
  printf("%s\n", buffer);

  char * GET  = strtok(buffer, " \t"); // get request
  char * url = strtok(NULL, " \t"); // URL
  char * type = "html";
  //url = trueurl(url);
  printf("this is  a url: %s\n", url);  
  printf("this is a type: %s\n", type);
  FILE * file;
  char msg[10000000];
  bzero(msg, strlen(msg));
char ans[500];
        strcpy(ans, ROOT);
        if(strcmp(url,"/")==0) strcat(ans, "/index.html");
        else strcat(ans, url);  
file = fopen(ans, "rb");
  size_t nread;
  long int len;
  int fileclosed = 0;
  if(file){
printf("[yes]\n");
     if(fseek(file,0,SEEK_END) == -1) {
	printf("[tes\n\n]");bad(sock, msg);goto L3;}
     len = ftell(file);
     if(len==-1) {bad(sock, msg);goto L3;}
     rewind(file);
printf("[yes]\n");
int cnt = 0;
while(!feof(file)) {
char ch;
ch = fgetc(file);
msg[cnt++] = ch;
printf("%c", ch);
}

     /*while((nread = fread(msg,strlen(msg), 1, file))>0){ if(ferror(file))exit(0); }
        */fclose(file);fileclosed = 1;
	printf("[yes]\n");    
} else {bad(sock, msg); goto L3;}
  if(strcmp(type,"none")==0){
printf("[tes]\n");    
bad(sock, msg); goto L3; 
  }
  else{ 
  char header[1000];
bzero(header, 1000);
printf("[yes]\n");
strcpy(header, "HTTP/1.1 200 OK\r\nConnection: closed\r\nContent-Type: text/");
  strcat(header, type);
  strcat(header, ";\r\nContent-length: ");
  char lenstr[40];
  sprintf(lenstr, "%d", len);
  strcat(header, lenstr);
  strcat(header, "\r\n\r\n");
  printf("[header] is: %s\n\n", header);
  sendall(sock, header);
  sendall(sock, msg);
  }
  L3:
  if(fileclosed == 0) fclose(file);
  printf("close\n");
  shutdown(sock, SHUT_RDWR);
  close(sock);
  }
