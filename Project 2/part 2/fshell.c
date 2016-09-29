	
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "fshell.h"

int main(int argc, char *argv[]){
	//do some stuff
	
	return 0;

}

/***	print a list of supported commands and their descriptions	***/
void help(){
	printf("********************************************************\n");
	printf("\t\t\t FAT12 SHELL HELP\n");
	printf("help:\t show commands in floppy console\n");
	printf("quit:\t exit floppy shell  \n");
	printf("fmount:\t mount a floppy image  \n");
	printf("fumount:\t unmount a mounted image  \n");
	printf("structure:\t define the structure of a floppy image  \n");
	printf("traverse:\t list contents of the floppy filesystem  \n");
	printf("showsector:\t show a hex dump of the specified sector  \n");
	printf("showfat:\t show the contents of the FAT table  \n");
}

/***	mount a floppy image.  Returns file descriptor.	***/
int fmount(char *filename){
	int fd = open(filename, O_RDONLY);
	return fd;
}

/***	unmount a floppy image.		***/
void fumount(int fd){

	close(fd);
}

/***	helper func: converts a buffer of binary data to hex.  Useful for hex dumps.	***/
char * to_hexbuf(char *b_data, int size){
	char h_data[size/4];		//bin-to-hex ratio is 4 chars to one char
	//char *h_ptr = &(h_data[0]);	//return this later
	char b_temp[size/4][4];		//several short binary strings
	int i, j;
	
	for(i=0; i<size; i = i+4){	
		for(j=0; j<4; j++){
			b_temp[i][j] = b_data[i+j];
		}
	}
	
	for(i=0; i<size; i++){		//conversion goes here
		h_data[i] = to_hex(b_temp[i]);
	}	
	
	return h_data;
}

/***	helper func: convert a short binary string to a single hex char.  Returns 'Z' on error.	***/
char to_hex(char *b_string){
	char hex;
	
	if(strcmp(b_string, "0000") == 0){
		hex = '0';
	}else if(strcmp(b_string, "0001") == 0){
		hex = '1';
	}else if(strcmp(b_string, "0010") == 0){
		hex = '2';
	}else if(strcmp(b_string, "0011") == 0){
		hex = '3';
	}else if(strcmp(b_string, "0100") == 0){
		hex = '4';
	}else if(strcmp(b_string, "0101") == 0){
		hex = '5';
	}else if(strcmp(b_string, "0110") == 0){
		hex = '6';
	}else if(strcmp(b_string, "0111") == 0){
		hex = '7';
	}else if(strcmp(b_string, "1000") == 0){
		hex = '8';
	}else if(strcmp(b_string, "1001") == 0){
		hex = '9';
	}else if(strcmp(b_string, "1010") == 0){
		hex = 'A';
	}else if(strcmp(b_string, "1011") == 0){
		hex = 'B';
	}else if(strcmp(b_string, "1100") == 0){
		hex = 'C';
	}else if(strcmp(b_string, "1101") == 0){
		hex = 'D';
	}else if(strcmp(b_string, "1110") == 0){
		hex = 'E';	
	}else if(strcmp(b_string, "1111") == 0){
		hex = 'F';
	}else{
		hex = 'Z';
	}
	
	return hex;
}









