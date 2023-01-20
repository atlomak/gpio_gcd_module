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
	int fd_in = 0;
	char buffer[MAX_BUFFER]; 
    fd_in = open(SYSFS_FILE_ARG1, O_WRONLY | O_APPEND);
	if(fd_in < 0){ 
		printf("Main write_a1 - error: %d\n", errno); 
		exit(1); 
	}
	snprintf(buffer, MAX_BUFFER, value);
    int n = write(fd_in, buffer, strlen(buffer));
	close(fd_in);
}
void write_a2(char* value) {
	int fd_in = 0;
	char buffer[MAX_BUFFER]; 
    fd_in = open(SYSFS_FILE_ARG2, O_WRONLY | O_APPEND);
	if(fd_in < 0){ 
		printf("Main write_a2 - error: %d\n", errno); 
		exit(1); 
	}
	snprintf(buffer, MAX_BUFFER, value);
    int n = write(fd_in, buffer, strlen(buffer));
	close(fd_in);
}




void test_GCD() {

	char *a1 = "34";
	char *a2 = "66";
	printf("\nGCD TEST FOR VALUES %s, %s\n", a1, a2);
	read_status();
	write_a1(a1);
	write_a2(a2);
	sleep(5);
	read_result();
	char *a3 = "39";
	char *a4 = "3";
	printf("\nGCD TEST FOR VALUES %s, %s\n", a3, a4);
	read_status();
	write_a1(a3);
	write_a2(a4);
	sleep(5);
	read_result();
	char *a5 = "120";
	char *a6 = "93";
    printf("\nGCD TEST FOR VALUES %s, %s\n", a5, a6);
	read_status();
	write_a1(a5);
	write_a2(a6);
	sleep(5);
	read_result();
	char *a7 = "21";
	char *a8 = "3";
    printf("\nGCD TEST FOR VALUES %s, %s\n", a7, a8);
	read_status();
	write_a1(a7);
	write_a2(a8);
	sleep(5);
	read_result();
	char *a9 = "21";
	char *a10 = "77";
    printf("\nGCD TEST FOR VALUES %s, %s\n", a9, a10);
	read_status();
	write_a1(a9);
	write_a2(a10);
	sleep(5);
	read_result();
}

int main(void){ 
	test_GCD();
	return 0; 
}

