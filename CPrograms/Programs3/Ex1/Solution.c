#include <unistd.h>
#include <stdlib.h>

void write_str(const char *str){
    ssize_t len = 0;
    while(str[len] != '\0')
        len++;
    write(STDOUT_FILENO, str, len);
}

void write_err(const char *err){
    ssize_t len = 0;
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
    if(argc != 1){
        write_err("Error: Program takes no arguments!\n");
        exit(1);
    }

    int pid = fork();

    if(pid == -1){
        write_err("Error: Failed to create child process!\n");
        exit(1);
    }

    if(pid == 0){
        write_str("[Child] Started running.\n");
        write_str("[Child] My PID is: ");
        write_pid(getpid());
        write_str("\n[Child] My Parent's PID is: ");
        write_pid(getppid());
        write_str("\n[Child] Going to sleep for 3 seconds...\n");

        sleep(3);

        write_str("[Child] Woke up! My Parent's PID is now: ");
        write_pid(getppid());
        write_str("\n[Child] Exiting.\n");
        exit(0);
    }
    else{
        write_str("[Parent] Started running.\n");
        write_str("[Parent] My PID is: ");
        write_pid(getpid());
        write_str("\n[Parent] I created a child with PID: ");
        write_pid(pid);
        write_str("\n[Parent] I am finishing execution and dying now.\n");
        exit(0);
    }
    return 0;
}