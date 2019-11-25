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

void fullRead(int fd, char *data, int n) {
    while(1) {
        int d = read(fd, data, n);
        int fin=0;
        for(int i=0; i < d; i++) {
            if(data[i] == '\n') {
               data[i] = '\0'; 
               fin = 1;
               break; 
            }
        }
        if(fin) {
            break;
        }
        data+=d;
        n-=d;
    }
}

void fullWrite(int fd, char *data, int n) {
    while(n) {
        int d = write(fd, data, n);
        n-=d;
        data+=d;
    }
}