#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

// Usage: your_docker.sh run <image> <command> <arg1> <arg2> ...
int main(int argc, char *argv[]) {
	// Disable output buffering
	setbuf(stdout, NULL);


	enum {BUFFER_SIZE=4096};

	int fd1[2];
	int fd2[2];  
	char buffer[BUFFER_SIZE];
	pipe(fd1);
	pipe(fd2);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	//printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	char *command = argv[3];
	int child_pid = fork();
	if (child_pid == -1) {
	    printf("Error forking!");
	    return 1;
	}
	
	if (child_pid == 0) {
		   // Replace current program with calling program
		dup2(fd1[1],STDERR_FILENO);
		dup2(fd1[1],STDOUT_FILENO);
		//dup2(fd[1],2);
		// close(fd1[0]);
		// close(fd2[0]);
	    execv(command, &argv[3]);
		
	} else {
		   // We're in parent

			// close(fd1[1]);
			// close(fd2[1]);
		   dup2(fd1[0],STDIN_FILENO);
		   while(read(fd1[0], buffer, BUFFER_SIZE) != 0) {}
		  printf(buffer);
		   wait(NULL);
		  // printf("Child terminated");
	}

	return 0;
}
