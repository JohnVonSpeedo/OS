#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    int fd_in = open(argv[1], O_RDONLY);
    if(fd_in != 0){
        perror("Error while opening file for reading!");
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;

    int lines = 0;
    int words = 0;
    int bytes = 0;

    int in_word = 0;

    while((bytes_read = read(fd_in, buff, sizeof(buff))) > 0){
        bytes += bytes_read;

        for(ssize_t i = 0; i < bytes_read; i++){
            if(buff[i] == '\n')
                lines++;

            if(buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\t' || buff[i] == '\r')
                in_word = 0;
            else{
                if(in_word == 0){
                    words++;
                    in_word = 1;
                }
            }
        }
    }

    if(bytes_read == -1){
        perror("Error while reading file!");
        close(fd_in);
        exit(1);
    }

    close(fd_in);

    printf("%d %d %d %s\n", lines, words, bytes, argv[1]);
    
    return 0;
}