#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print(int fd, char* str){
    int len = 0;
    while(str[len] != '\0')
        len++;
    if(write(fd, str, len) != len)
        exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){
    //TBD
    return 0;
}