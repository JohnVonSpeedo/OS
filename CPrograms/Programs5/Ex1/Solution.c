#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void print(int fd, char* str){
	int len = 0;
	while(str[len] != '\0')
		len++;
	if(write(fd, str, len) != len)
		exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){
	if(argc != 3){
		print(STDERR_FILENO, "Usage: <program> <command1> <command2>\n");
		exit(EXIT_FAILURE);
	}

	int pipefd[2];
	if(pipe(pipefd) == -1){
		print(STDERR_FILENO, "Error: Failed to create pipe!\n");
		exit(EXIT_FAILURE);
	}

	int child1 = fork();
	if(child1 == -1){
		print(STDERR_FILENO, "Error: Failed to create first child!\n");
		exit(EXIT_FAILURE);
	}

	if(child1 == 0){
		close(pipefd[0]);

		if(dup2(pipefd[1], STDOUT_FILENO) == -1){
			print(STDERR_FILENO, "Error: Failed while performing dup2()\n");
			close(pipefd[1]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);

		execlp("sh", "sh", "-c", argv[1], NULL);
		print(STDERR_FILENO, "Error: First command failed or wasn't found!\n");
		exit(EXIT_FAILURE);
	}

	int child2 = fork();
	if(child2 == -1){
		print(STDERR_FILENO, "Error: Failed to create second child!\n");
		exit(EXIT_FAILURE);
	}

	if(child2 == 0){
		close(pipefd[1]);

		if(dup2(pipefd[0], STDIN_FILENO) == -1){
			print(STDERR_FILENO, "Error: Failed while performing dup2()\n");
			close(pipefd[0]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);

		execlp("sh", "sh", "-c", argv[2], NULL);
		print(STDERR_FILENO, "Error: Second command failed or wasn't found!\n");
		exit(EXIT_FAILURE);
	}

	close(pipefd[0]);
	close(pipefd[1]);

	int status1, status2;
	waitpid(child1, &status1, 0);
	waitpid(child2, &status2, 0);
	
	return 0;
}
