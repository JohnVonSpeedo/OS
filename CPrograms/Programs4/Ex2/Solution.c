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
    if(argc < 3){
        print("Error: Usage: <program> <file_name> <command> [options]\n", STDERR_FILENO);
        exit(1);
    }

    int child1 = fork();

    if(child1 == -1){
        print("Error: Failed to create first child!\n", STDERR_FILENO);
        exit(1);
    }

    if(child1 == 0){
        execlp("grep", "grep", "int", argv[1], NULL);

        print("Error: Failed to execute grep command!\n", STDERR_FILENO);
        exit(1);
    }
    else{
        int status1;
        waitpid(child1, &status1, 0);

        int child2 = fork();

        if(child2 == -1){
            print("Error: Failed to create second child!\n", STDERR_FILENO);
            exit(1);
        }

        if(child2 == 0){
            execvp(argv[2], argv + 2);

            print("Error: Error while excuting the provided command!\n", STDERR_FILENO);
            exit(1);
        }
        else{
            int status2;
            waitpid(child2, &status2, 0);

            if(WIFEXITED(status2)){
                int exit_code = WEXITSTATUS(status2);
                print("Exit code: ", STDOUT_FILENO);
                print_int(exit_code, STDOUT_FILENO);
                print("\n", STDOUT_FILENO);
            }
            else{
                print("Error: Command did not terminate normally!\n", STDERR_FILENO);
                exit(1);
            }
        }
    }
    return 0;
}
