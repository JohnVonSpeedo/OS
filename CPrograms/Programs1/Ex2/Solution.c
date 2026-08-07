#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 1){
        fprintf("Usage: %s <file_name>", argv[0]);
        return EXIT_FAILURE;
    }

    char* FILE_NAME = argv[1];
    const char* data = "ABCDEFGHIJKLMNOPQRSTUVWXY";
    char* buffer[10];
    ssize_t bytes_read;
    int fd;

    fd = open(FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd == -1){
        perror("Error opening file for writing!");
        return EXIT_FAILURE;
    }

    ssize_t bytes_written = write(fd, data, 25);
    if(bytes_written != 25) {
        perror("Error writing to file!");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);

    fd = open(FILE_NAME, O_RDONLY);
    if(fd == -1){
        perror("Error opening file for reading!");
        return EXIT_FAILURE;
    }

    printf("Reading from file...\n");

    while(true){
        bytes_read = read(fd, buffer, 10);

        printf("read() returned $zd\n", bytes_read);

        if(bytes_read <= 0)
            break;
    }

    if(bytes_read == -1){
        perror("Error during read!");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    return EXIT_SUCCESS;
}