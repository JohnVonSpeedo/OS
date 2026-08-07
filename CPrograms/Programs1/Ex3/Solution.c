#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <file_name>", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY, O_CREAT, O_TRUNC, 0644);
    if(fd == -1){
        perror("Error while opening file!");
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;

    while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
        if(write(fd, buff, bytes_read) != bytes_read){
            perror("Error while writing to file!");
            close(fd);
            exit(1);        
        }

        for(ssize_t i = 0; i < bytes_read; i++){
            if(buff[i] == '\t')
                fprintf(stderr, ">>>");
            else
                fprintf(stderr, "%c", buff[i]);
        }
    }

    if(bytes_read == -1){
        perror("Error reading from stdin!");
        close(fd);
        exit(1);
    }

    close(fd);

    return 0;
}