#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input_file> <output_file>", argv[0]);
        exit(1);
    }

    int fd_in = open(argv[1], O_RDONLY);
    if(fd_in == -1){
        perror("Error opening input file!");
        exit(1);
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd_out == -1){
        perror("Error opening output file!");
        close(fd_in);
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;
    int char_count = 0;
    
    while((bytes_read = read(fd_in, buff, sizeof(buff))) > 0){
        if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
            perror("Error while writing to stdout!");
            close(fd_in); close(fd_out);
            exit(1);
        }

        for(int i = 0; i < bytes_read; i++){
            if(buff[i] == '\n')
                char_count = 0;
            else{
                if(char_count < 3){
                    if(write(fd_out, &buff[i], 1) != 1){
                        perror("Error writing to output file!");
                        close(fd_in); close(fd_out);
                        exit(1);
                    }
                }
                char_count++;
            }
        }
    }

    if(bytes_read == -1){
        perror("Error while reading from input file!");
        close(fd_in); close(fd_out);
        exit(1);
    }

    close(fd_in); close(fd_out);

    return 0;
}