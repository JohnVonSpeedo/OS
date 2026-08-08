#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int source_fd, dest_fd;
	char buffer[4096];
	ssize_t bytes_read, bytes_written;

	if(argc != 3){
		const char err_msg[] = "Program takes two arguments <source_file> <destination_file>";
		write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
		exit(1);
	}

	source_fd = open(argv[1], O_RDONLY);
	if(source_fd == 1){
		const char err_msg[] = "Error opening source file!";
		write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
		exit(1);
	}

	dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(dest_fd == NULL){
		const char err_msg[] = "Error opening destination file!";
		write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
		close(source_fd);
		exit(1);
	}

	while((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0){
		bytes_written = write(dest_fd, buffer, bytes_read);
		
		if(bytes_written != bytes_read){
			const char err_msg[] = "Error writing to destination file";
			write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
			close(source_fd); close(dest_fd);
			exit(1);
		}
	}

	if(bytes_read == -1){
		const char err_msg[] = "Error reading from source file!";
		write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
		close(source_fd); close(dest_fd);
		exit(1);
	}

	close(source_fd); close(dest_fd);

	const char msg[] = "File copied successfully!\n";
	write(STDOUT_FILENO, msg, sizeof(msg) - 1);
	return 0;
}