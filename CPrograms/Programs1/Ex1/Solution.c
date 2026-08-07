#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE *source_file, *dest_file;
	char buffer[4096];
	size_t bytes_read;

	if(argc != 3){
		fprintf(stderr, "Usage: %s <source_file> <destination_files>\n", argv[0]);
		return EXIT_FAILURE;
	}

	source_file = fopen(argv[1], "rb");
	
	if(source_file == NULL){
		perror("Error opening source file!");
		return EXIT_FAILURE;
	}

	dest_file = fopen(argv[2], "wb");
	
	if(dest_file == NULL){
		perror("Error opening destination file!");
		fclose(source_file);
		return EXIT_FAILURE;
	}

	while((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0){
		size_t bytes_written = fwrite(buffer, 1, bytes_read, dest_file);
		if(bytes_written != bytes_read){
			perror("Error writing to destination file");
			fclose(source_file);
			fclose(dest_file);
			return EXIT_FAILURE;
		}
	}

	fclose(source_file);
	fclose(dest_file);

	printf("File copied successfully!\n");
	return EXIT_SUCCESS;
}