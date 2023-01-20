#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>	// for exit()
#include <string.h> // for strlen()

#define MAX_BUFFER 1024 

#define SYSFS_FILE_ARG1 "/sys/sykom/tae3arg1"		// Register A1
#define SYSFS_FILE_ARG2 "/sys/sykom/tae3arg2"		// Register A2
#define SYSFS_FILE_RESULT "/sys/sykom/tae3result"	// Register W
#define SYSFS_FILE_STATUS "/sys/sykom/tae3status"	// Register S


// Function to read from kernel
void read_result() {
	char buffer[MAX_BUFFER]; 
	int fd_out = open(SYSFS_FILE_RESULT, O_RDONLY);
    if(fd_out < 0){
		printf("Main - open result - error: %d\n", errno); 
		exit(2); 
	}
	int n = read(fd_out, buffer, MAX_BUFFER); 
	if(n > 0) { 
		buffer[n] = '\0'; 
		printf("Main - Value from kernel: %s\n", buffer);
    } else {
		printf("Main - value not ready: %d", n);
	}
	close(fd_out);
}
void read_status() {
	char buffer[MAX_BUFFER]; 
	int fd_out = open(SYSFS_FILE_STATUS, O_RDONLY);
    if(fd_out < 0){
		printf("Main - open result - error: %d\n", errno); 
		exit(2); 
	}
	int n = read(fd_out, buffer, MAX_BUFFER); 
	if(n > 0) { 
		buffer[n] = '\0'; 
		printf("Main - Value from kernel: %s\n", buffer);
    } else {
		printf("Main - value not ready: %d", n);
	}
	close(fd_out);
}

// Functions to write
void write_a1(char* value) {
	char buffer[MAX_BUFFER]; 
    int fd_in = open(SYSFS_FILE_ARG1, O_WRONLY);
    int n = write(fd_in, buffer, strlen(buffer));
}
void write_a2(char* value) {
	char buffer[MAX_BUFFER]; 
    int fd_in = open(SYSFS_FILE_ARG2, O_WRONLY);
    int n = write(fd_in, buffer, strlen(buffer));
}




void test_GCD() {

	printf("\nGCD TEST FOR VALUES %s, %s\n", "10", "C");
	write_a1("10");
	write_a2("C");
	read_result();

	printf("\nGCD TEST FOR VALUES %s, %s\n", "3", "9");
	write_a1("3");
	write_a2("9");
	read_result();
    
    printf("\nGCD TEST FOR VALUES %s, %s\n", "2", "4");
	write_a1("2");
	write_a2("4");
	read_result();

    printf("\nGCD TEST FOR VALUES %s, %s\n", "4", "C1");
	write_a1("4");
	write_a2("C1");
	read_result();
}

int main(void){ 
	test_GCD();
	return 0; 
}

