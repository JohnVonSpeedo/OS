#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 1){
        const char msg[] = "Error: This implementation takes no parameters!\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;

    while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
        if(write(STDIN_FILENO, buff, bytes_read) != bytes_read){
            const char err_msg[] = "Error: Failed to write to standard output!\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
            exit(1);
        }
    }

    if(bytes_read == -1){
        const char err_msg[] = "Error: Error while reading from standard input!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }
    return 0;
}