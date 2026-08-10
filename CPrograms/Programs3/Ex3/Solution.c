#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void write_str(char* str){
    int len = 0;
    while(str[len] != 0)
        len++;
    write(STDOUT_FILENO, str, len);
}

void write_err(char* err){
    int len = 0;
    while(err[len] != 0)
        len++;
    write(STDERR_FILENO, err, len);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        write_err("Error: Program takes <command> [options] as arguments!\n");
        exit(1);
    }

    int pid = fork();

    if(pid == -1){
        write_err("Error: Failed to create child procces!\n");
        exit(1);
    }

    if(pid == 0){
        execvp(argv[1], argv + 1);
        write_err("Error: command failed to execute or was not found!\n");
        exit(1);
    }
    else{
        int status;

        waitpid(pid, &status, 0);

        if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
            write_str(argv[1]);
            write_str("\n");
        }
    }
    return 0;
}