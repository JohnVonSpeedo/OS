#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 4){
        const char err_msg[] = "Error: Program takes 3 parameters: <m> <n> <file_name>\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    if(m <= 0 || n < m){
        const char err_msg[] = "Error: Invalid m and n values!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    int fd_in = open(argv[3], O_RDONLY);
    if(fd_in == -1){
        const char err_msg[] = "Error opening input file!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    char buff[4096];
    ssize_t bytes_read;

    int char_pos = 1;

    while((bytes_read = read(fd_in, buff, sizeof(buff))) > 0){
        for(int i = 0; i < bytes_read; i++){
            if(buff[i] == '\n'){
                if(write(STDOUT_FILENO, &buff[i], 1) != 1){
                    const char err_msg[] = "Error while writing to standard output!\n";
                    write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
                    close(fd_in);
                    exit(1);
                }
                char_pos = 1;
            }
            else{
                if(char_pos >= m && char_pos <= n){
                    if(write(STDOUT_FILENO, &buff[i], 1) != 1){
                        const char err_msg[] = "Error while writing to standard output!\n";
                        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
                        close(fd_in);
                        exit(1);
                    }
                }
                char_pos++;
            }
        }
    }

    if(bytes_read == -1){
        const char err_msg[] = "Error while reading from file!\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        close(fd_in);
        exit(1);
    }

    close(fd_in);
    
    return 0;
}