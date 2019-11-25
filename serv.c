#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

struct Student {
    char *indeks;
    char *name;
};

#define numberOfStudents 3

struct Student students[numberOfStudents];

int getStudent(char *indeks) {
    for(int i=0; i < numberOfStudents; i++) {
        if(strcmp(students[i].indeks, indeks) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    struct sockaddr_in addr, client_addr;
	int fd = socket(PF_INET, SOCK_STREAM, 0);
    int on=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)); //free port number when turned off
	addr.sin_family = PF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sock_addr*) &addr, sizeof(addr));
    listen(fd, 10);
    
    const int buf_size=256;
    char buf[buf_size];
    students[0].name = "Kamil\n";
    students[0].indeks = "136780";
    students[1].name = "Daniel\n";
    students[1].indeks = "136775";
    students[2].name = "Maciej\n";
    students[2].indeks = "136698";
	while(1) {
        socklen_t l = sizeof(client_addr);
        int cfd = accept(fd, (struct sock_addr*) &client_addr, &l);
        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        
        read(cfd, buf, buf_size);
        int id = getStudent(buf);
        if(id == -1) {
            char msg[] = "Wrong index\n";
            write(cfd, msg, strlen(msg)+1);
        } else {
            write(cfd, students[id].name, strlen(students[id].name)+1);
        }
        write(cfd, "Hello\n", 7);
        close(cfd);
    }
    close(fd);
}
