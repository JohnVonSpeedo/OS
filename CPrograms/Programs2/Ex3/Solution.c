#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        const char err_msg[] = "Error: command takes only one parameter <file_name>\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    int fd_out = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd_out == -1){
        const char err_msg[] = "Error while opening output file!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;

    while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
        if(write(fd_out, buff, bytes_read) != bytes_read){
            const char err_msg[] = "Error while writing to file!\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
            close(fd_out);
            exit(1);
        }
    }

    if(bytes_read == -1){
        const char err_msg[] = "Error while reading from standard input!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        close(fd_out);
        exit(1);
    }

    close(fd_out);

    return 0;
}