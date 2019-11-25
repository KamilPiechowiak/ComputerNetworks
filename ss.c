#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include"rw.h"

struct Student {
    char *indeks;
    char *name;
};

#define numberOfStudents 3
#define idsSize 1024

int ids[idsSize];

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
    int fmax = fd;
    fd_set read_mask, read_cur_mask, write_mask, write_cur_mask;
    FD_SET(fd, &read_mask);
    struct timeval timeout; 
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
        read_cur_mask = read_mask;
        write_cur_mask = write_mask;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int rc = select(fmax+1, &read_cur_mask, &write_cur_mask, (fd_set*)0, &timeout);
        if(rc == 0) {
            printf("timed out\n");
            continue;
        }
        if(FD_ISSET(fd, &read_cur_mask)) {
            socklen_t l = sizeof(client_addr);
            int cfd = accept(fd, (struct sock_addr*) &client_addr, &l);
            FD_SET(cfd, &read_mask);
            if(cfd > fmax) {
                fmax = cfd;
            }
            printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        }
        for(int i=fd+1; i <= fmax; i++) {
            if(FD_ISSET(i, &read_cur_mask)) {
                fullRead(i, buf, buf_size);
                ids[i] = getStudent(buf);
                FD_CLR(i, &read_mask);
                FD_SET(i, &write_mask);
            } else if(FD_ISSET(i, &write_cur_mask)) {
                if(ids[i] == -1) {
                    char msg[] = "Wrong index\n";
                    fullWrite(i, msg, strlen(msg));
                } else {
                    fullWrite(i, students[ids[i]].name, strlen(students[ids[i]].name));
                }
                FD_CLR(i, &write_mask);
                close(i);
            } 
        }
        while(fmax > fd && FD_ISSET(fmax, &read_mask) == 0 && FD_ISSET(fmax, &write_mask) == 0) {
            fmax-=1;
        }
        printf("Current fmax: %d\n", fmax);
    }
    close(fd);
}
