#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void print(char* str, int fd){
    int len = 0;
    while(str[len] != '\0')
        len++;
    if(write(fd, str, len) != len)
        exit(1);
}

void print_int(int num, int fd){
    char buff[32];
    int i = 0;

    if(num == 0){
        print("0", fd);
        return;
    }

    while(num > 0){
        buff[i++] = (num % 10) + '0';
        num /= 10;
    }

    char rev[32];
    for(int j = 0; j < i; j++)
        rev[j] = buff[i - 1 - j];
    rev[i] = '\0';

    print(rev, fd);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        print("Error: Usage: <program> <cmd1> ... <cmdN>\n", STDERR_FILENO);
        exit(1);
    }

    int failed = 0;
    int succeeded = 0;

    for(int i = 1; i < argc; i++){
        int child = fork();

        if(child == -1){
            print("Failed to create child ", STDERR_FILENO);
            print_int(i, STDERR_FILENO);
            print("\n", STDERR_FILENO);
            continue;
        }

        if(child == 0){
            execlp(argv[i], argv[i], NULL);
            print("Error: Command not found of failed to launch: ", STDERR_FILENO);
            print(argv[i], STDERR_FILENO);
            print("\n", STDERR_FILENO);
            exit(1);
        }
        else{
            int status;
            waitpid(child, &status, 0);

            if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
                succeeded++;
            else
                failed++;
        }
    }

    print("Successful commands: ", STDOUT_FILENO);
    print_int(succeeded, STDOUT_FILENO);
    print("\n", STDOUT_FILENO);

    print("Failed commands: ", STDOUT_FILENO);
    print_int(failed, STDOUT_FILENO);
    print("\n", STDOUT_FILENO);

    return 0;
}