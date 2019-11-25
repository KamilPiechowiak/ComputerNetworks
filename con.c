#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include"rw.h"

int main(int argc, char **argv) {
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	struct hostent *host = gethostbyname(argv[1]);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
	connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	
	char *indeks = "136780\n";
	// sleep(5);
	fullWrite(fd, indeks, strlen(indeks));
	const int buf_size = 256;
	char buf[buf_size];
	fullRead(fd, buf, buf_size);
	printf("%s\n", buf);
	close(fd);
}
