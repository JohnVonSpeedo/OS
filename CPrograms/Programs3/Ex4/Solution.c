#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void write_str(char* str){
    int len = 0;
    while(str[len] != '\0')
        len++;
    write(STDOUT_FILENO, str, len);
}

void write_err(char* err){
    int len = 0;
    while(err[len] != '\0')
        len++;
    write(STDERR_FILENO, err, len);
}

void write_pid(int pid){
    char buff[32];
    int i = 0;
    
    if(pid == 0){
        write(STDOUT_FILENO, "0", 1);
        return;
    }

    while(pid > 0){
        buff[i++] = (pid % 10) + '0';
        pid /= 10;
    }

    char rev[32];
    for(int j = 0; j < i; j++)
        rev[j] = buff[i - 1 - j];
    
    write(STDOUT_FILENO, rev, i);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        write_err("Error: Program takes <command> [options] as parameters!\n");
        exit(1);
    }

    int pid = fork();

    if(pid == -1){
        write_err("Error: Failed to create child process!\n");
        exit(1);
    }

    if(pid == 0){
        execvp(argv[1], argv + 1);
        write_err("Error: Failed to execute or command wasn't found!\n");
        exit(1);
    }
    else{
        int status;

        waitpid(pid, &status, 0);

        write_str("Child PID: ");
        write_pid(pid);
        write_str("\n");

        if(WIFEXITED(status)){
            int exit_code = WEXITSTATUS(status);
            write_str("Exit code: ");
            write_pid(exit_code);
            write_str("\n");
        }
        else
            write_str("Child did not terminate normally.\n");
    }

    return 0;
}