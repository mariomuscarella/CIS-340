
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void help();
int fmount(char *filename);
void fumount();
int getCommand(char *argv[]);
void traverse(char* argv);
void structure();
void showsector(char* argv);
void showfat();
void setGlobalVar();
int check(char* argv);
void list_subdir(int start, char* pwd, char* argv);




#define QUIT		 "quit"
#define HELP		 "help"
#define FMOUNT		 "fmount"
#define FUMOUNT		 "fumount"
#define STRUCTURE	 "structure"
#define TRAVERSE	 "traverse"
#define SHOWSECTOR	 "showsector"
#define SHOWFAT		 "showfat"

//global variable for FAT operating functions

int num_of_FAT;
int num_of_sector_per_FAT;
int num_of_sector_per_cluster;
int num_of_ROOT_entries;
int num_of_bytes_per_sector;
int num_of_total_sector;

int BOOT_sector = 0;
int FAT1_start_sector = 1;
int FAT1_end_sector;
int FAT2_start_sector;
int FAT2_end_sector;
int ROOT_start_sector;
int ROOT_end_sector;
int DATA_start_sector;
int no_subdir = 0;



int fd = 0;

int main(int argc, char* argv[]){
	//do some stuff
	
while(1){
printf("flop: ");

argc = getCommand(argv);

	if(argc == 0)continue;
	else if(argc == 1){
		// check argv[1] to get the command
		
		if(strcmp(argv[1],QUIT) == 0)break;
		else if(strcmp(argv[1],HELP) == 0)help();
		else if(strcmp(argv[1],FUMOUNT) == 0)fumount();
		else if(strcmp(argv[1],TRAVERSE) == 0)traverse(NULL);
		else if(strcmp(argv[1],SHOWFAT) == 0)showfat();
		else if(strcmp(argv[1],STRUCTURE) == 0)structure();
		else if(strcmp(argv[1],SHOWSECTOR) == 0)printf("command \"showsector\" missing sector number\n");
		else if(strcmp(argv[1],FMOUNT) == 0)printf("command \"fmount\" missing img file name\n");
		else printf("invalid command, try \'help\' for more information\n");
	}
	else if(argc == 2){
		// check argv[1] to get the command
		// and check argv[2] to get the option for command
			
		if(strcmp(argv[1], FMOUNT) == 0)fd = fmount(argv[2]);
		else if(strcmp(argv[1],TRAVERSE) == 0)traverse(argv[2]);
		else if(strcmp(argv[1],SHOWSECTOR) == 0){
		if(check(argv[2]) < 0){
			printf("invalid sector number\n");
			}
		else showsector(argv[2]);
		}
		else if(strcmp(argv[1],QUIT) == 0)printf("no options for command \"quit\" \n");
		else if(strcmp(argv[1],HELP) == 0)printf("no options for command \"help\" \n");
		else if(strcmp(argv[1],FUMOUNT) == 0)printf("no options for command \"fumount\" \n");
		else if(strcmp(argv[1],SHOWFAT) == 0)printf("no options for command \"showfat\" \n");
		else if(strcmp(argv[1],STRUCTURE) == 0)printf("no options for command \"showfat\" \n");
		else printf("command not found\n");
	}
	else printf("invalid command, try \'help\' for more information\n");
}

return 0;

}

/***	print a list of supported commands and their descriptions	***/
void help(){
	printf("**********************************************************************\n");
	printf("\t\t\t\t FAT12 SHELL HELP\n");
	printf("**********************************************************************\n");
	printf("help:\t\t\t\t show commands in floppy console\n");
	printf("quit:\t\t\t\t exit floppy shell  \n");
	printf("fmount<filename>:\t\t\t mount a floppy image  \n");
	printf("fumount:\t\t\t unmount a mounted image  \n");
	printf("structure:\t\t\t define the structure of a floppy image  \n");
	printf("traverse[-l]:\t\t\t list contents of the floppy filesystem  \n");
	printf("\t\t\t\t -l:	list all detailed information \n");
	printf("\t\t\t\t    	including file attribution, last modified time \n");
	printf("\t\t\t\t    	size, full pathname and starting cluster ID \n");
	printf("showsector[sector number]:\t show a hex dump of the specified sector  \n");
	printf("showfat:\t\t\t show the contents of the FAT table  \n\n\n");
}

/***	mount a floppy image.  Returns file descriptor.	***/
int fmount(char *filename){

	fd = open(filename, O_RDONLY);
	if (fd < 0){
	printf("failed to open image file\n");
	return -1;
	}
	else printf("%s successfully mounted\n",filename);
	setGlobalVar(fd);
	return fd;
}

/***	unmount a floppy image.		***/
void fumount(){
if(fd <= 0){
	printf("no imagefile to unmount\n");
	return;
}
int i;	
	i = close(fd);
	if(i < 0){
		printf("failed to close imagefile\n");
	}
	else printf("successfully unmounted\n");
	fd = 0;
}


/*** traverse function with option reading from argv[2](data type char*)  ***/
void traverse(char* argv){
if(fd <= 0){
	printf("no imagefile to read\n");
	return;
}
if(argv == NULL);
else if(strcmp(argv, "-l") == 0){
// -l option for traverse, long list
printf("        *****************************\n");
printf("        ** FILE ATTRIBUTE NOTATION **\n");
printf("        **                         **\n");
printf("        ** R ------ READ ONLY FILE **\n");
printf("        ** S ------ SYSTEM FILE    **\n");
printf("        ** H ------ HIDDEN FILE    **\n");
printf("        ** A ------ ARCHIVE FILE   **\n");
printf("        *****************************\n");
printf("\n");
}
else {
printf("invalid option for traverse, try \'help\' for more information\n");
return;
}
int size = num_of_bytes_per_sector;
int num = ROOT_start_sector;
unsigned char buf[size];
char pwd_buf[1024];
char *pwd = pwd_buf;
char temp_buf[1024];
char *t = temp_buf;

char file_attribute[5] = {'-','-','-','-','-'};
int *time;
int hour, minute,second;
int year, month, day;

int *date;
int *file_size;
int *p_cluster;
int starting_cluster;


lseek(fd,num*size,SEEK_SET);
int num_of_ROOT_entry_read = 0;

int n;

if(n = read(fd, buf, 32) < 0){
	printf("error in reading sector\n");
	return;
	}
char* isDIR = "<DIR>";



while(num_of_ROOT_entry_read <= num_of_ROOT_entries){
	if((buf[11] == 0x0F) || (buf[0] == 0xE5)){
		memset(buf,0,32);
		num_of_ROOT_entry_read++;
		if(n = read(fd, buf, 32) < 0){
			printf("error in reading sector\n");
			return;
			}
		continue;
	}
	else{
		if((buf[0]!= 0x2E)&&(buf[0x1A] == 0x00))break;
// get file attribute notation
		if(((buf[11] & 0x20) >> 5) == 0x01)file_attribute[1] = 'A';
		if(((buf[11] & 0x02) >> 1) == 0x01)file_attribute[2] = 'H';
		if(((buf[11] & 0x04) >> 2) == 0x01)file_attribute[3] = 'S';
		if((buf[11] & 0x01) == 0x01)file_attribute[0] = 'R';

// get file last modification date and time
		time = (int*)(&buf[22]);
		date = (int*)(&buf[24]);
		second = (*time & 0x0000001F);
		minute = (*time & 0x000007E0)>> 5;
		hour = (*time & 0x0000F800) >> 11;
		day = (*date & 0x0000001F);
		month = (*date & 0x000001E0) >> 5;
		year = ((*date & 0x0000FE00) >> 9 )+ 1980;

// get the file size
		file_size = (int*)(&buf[28]);

// get the first cluster
		p_cluster = (int *)(&buf[26]);
		starting_cluster = (*p_cluster)& 0x0000FFFF;

		int a = 0;
		pwd = pwd_buf;
		t = temp_buf;
		strcpy(pwd, "/");
		
		while(a < 11){
			if(buf[a] != 0x20){
				*t = buf[a];
				t++;
				}
				a++;
				if(a == 8 && (buf[9]!= 0x20) && (buf[11] != 0x10)){
					*t = '.';
					t++;
				}		
			}
		*t = '\0';
		strcat(pwd,temp_buf);	
		if(argv == NULL){
			if((buf[11] == 0x10))printf("%-32s%24s\n",pwd,isDIR);
			if((buf[11] != 0x10))printf("%-32s\n",pwd);
			}
		else{
			if((buf[11] == 0x10)){
				printf("%-10s%2d/%2d/%4d/ %2d:%2d:%02d \t   %-15s\t  %-24s%15d\n",file_attribute,month,day,year,hour,minute,second,isDIR,pwd,starting_cluster);
				}
			if((buf[11] != 0x10)){
				printf("%-10s%2d/%2d/%4d/ %2d:%2d:%02d \t\t\t%-10d%-24s%15d\n",file_attribute,month,day,year,hour,minute,second,*file_size,pwd,starting_cluster);
				}
		}
			if((buf[11] == 0x10) && (buf[0] != 0x2E)){
					
					int subdir_sector = buf[26] + (buf[27] << 8);
					int start = (DATA_start_sector + subdir_sector - 2)* num_of_sector_per_cluster *num_of_bytes_per_sector;
					list_subdir(start, pwd_buf,argv);
//					memset(temp_buf,0,1024);
//					memset(pwd_buf,0,1024);
				}
	
	memset(buf,0,32);
	num_of_ROOT_entry_read++;
	lseek(fd, (num*size + (num_of_ROOT_entry_read)*32) ,SEEK_SET);
	memset(temp_buf,0,1024);
	memset(pwd_buf,0,1024);
	if(n = read(fd, buf, 32) < 0){
				printf("error in reading sector\n");
				return;
				}
	
		}
	}

memset(buf,0,size);
return;
}

/*** structure function  ***/
void structure(){
if(fd <= 0){
	printf("no imagefile to read\n");
	return;
}
printf("structure\n");
printf("\tnumber of FAT:\t\t\t%5d\n", num_of_FAT);
printf("\tnumber of sectors used by FAT:\t%5d\n", num_of_sector_per_FAT);
printf("\tnumber of sectors per cluster:\t%5d\n", num_of_sector_per_cluster);
printf("\tnumber of ROOT entries:\t\t%5d\n", num_of_ROOT_entries);
printf("\tnumber of bytes per sector:\t%5d\n", num_of_bytes_per_sector);
printf("\t---Sector #---\t   ---Sector Types---\n");
printf("\t     %2d\t\t\tBOOT\n",BOOT_sector);
printf("\t   %02d -- %02d\t\tFAT1\n", FAT1_start_sector, FAT1_end_sector);
if(num_of_FAT == 2)
	printf("\t   %02d -- %02d\t\tFAT2\n", FAT2_start_sector, FAT2_end_sector);
printf("\t   %02d -- %02d\t\tROOT DIRECTORY\n\n\n", ROOT_start_sector, ROOT_end_sector);
return;
}

/***  showsector function ***/
void showsector(char*argv){
if(fd <= 0){
	printf("no imagefile to read\n");
	return;
}
int num;
if((num = atoi(argv))< 0){
	printf("sector number error\n");
	return;
	}
if((num = atoi(argv))> num_of_total_sector){
	printf("exceed maximum sector number: %d\n", num_of_total_sector);
	return;
	}
int n;
int size = num_of_bytes_per_sector;
unsigned char buf[size];

lseek(fd, size*num, SEEK_SET);
if(n = read(fd, buf, size) < 0){
	printf("error in reading sector\n");
	return;
	}
int element = 0;

printf("showsector %d\n\n", num);

printf("   %3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X%3X\n\n",0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

printf("%3X", element);

while(element < size){

	printf("%3X",buf[element++]);

	if(element%16 == 0){

		printf("\n");

		if(element < size)printf("%3X", element);

		}
}

printf("\n\n");
return;
}

/***  showfat function ***/
void showfat(){
if(fd <= 0){
	printf("no imagefile to read\n");
	return;
}
int n;
int size = num_of_sector_per_FAT * num_of_bytes_per_sector;
unsigned char buf[size];
lseek(fd, num_of_bytes_per_sector, SEEK_SET);
if((n = read(fd, buf, num_of_bytes_per_sector)) < 0){
	printf("error in reading FAT sector\n");
	return;
	}

int element = 0;
int *entry;
int odd_entry;
int even_entry;

printf("showfat \n\n");
printf("    %5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x%5x\n\n",0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
printf("             ");

element = element + 3;

while(element < (size)){
	entry = (int*) (&buf[element]);
	odd_entry = (*entry & 0x00000FFF);
	even_entry = ((*entry & 0x00FFF000) >> 12);

	(odd_entry != 0) ? printf("%5x",odd_entry): printf(" FREE");
	(even_entry != 0) ? printf("%5x",even_entry): printf(" FREE");
	
	element = element + 3;
	if((element % 24) == 0 && element < size){
		printf("\n%5x", element);
		}
	
	}

printf("\n\n");
memset(buf,0,size);
return;
}

/*** read command line arguments into argv[], return the number of how many arguments read  ***/
int getCommand(char* argv[]){
int i = 0;
int j = 0;
char input[100];
char *buf; 
char *start; 
buf = start = input;
char ch;


while(1){
	if(j == 100){
	printf("exceed command line limit, maximum 100 characters\n");
	return 0;
	}
	ch = getchar();
	if(ch == '\n'){
	input[j] = '\0';
	
	break;
	}
	if(j == 0 && ch == ' ')continue;
	else {
		if(input[j-1] == ' '&& ch == ' ')continue;
		else {
			if(ch == ' '){
				input[j++] = '\0';
				
			}	
			input[j++] = ch;
			
		}
	}
}

argv[++i] = (char*)malloc(sizeof(char*));
memcpy(argv[i],start,strlen(start)+1);

while(1){
	if(strlen(input) == 0)return 0;
	if(buf == &input[j]){
	break;
	}
	if(i == 3)break;
	else if(*buf == '\0'){

		if(*(buf+1)!= '\0')
			start = buf + 2;

			argv[++i] = (char*)malloc(sizeof(char*));
			memcpy(argv[i],start,strlen(start)+1);
				
	}
	buf++;

}

return i;
}

/**  function to set the global variables **/
void setGlobalVar(int fd){

unsigned char buf[1024];
int n;

lseek(fd, 0, SEEK_SET);
if((n=read(fd,buf,62)) < 0){
	printf("error in reading BOOT sector, can not set global variables\n");
	return;
	}
// byte 16, Number of FATs

num_of_FAT = buf[16];

// byte 22, 23, Number of sectors used by FAT

num_of_sector_per_FAT = buf[22] + buf[23]*256;

// byte 13, Number of sector per cluster

num_of_sector_per_cluster = buf[13];

// byte 17, 18, maximum Number of ROOT entries

num_of_ROOT_entries = buf[17] + buf[18]*256;


// byte 11, 12, number of bytes per sector

num_of_bytes_per_sector = buf[11] + buf[12]*256;

// byte 19, 20, number of total sector on this volume
num_of_total_sector = buf[19] + buf[20]*256;


FAT1_end_sector = BOOT_sector + num_of_sector_per_FAT;
FAT2_start_sector = (num_of_FAT - 1)*num_of_sector_per_FAT + FAT1_start_sector;
FAT2_end_sector = (num_of_FAT - 1)*num_of_sector_per_FAT + FAT1_end_sector;
ROOT_start_sector = (num_of_FAT)* num_of_sector_per_FAT + FAT1_start_sector;
ROOT_end_sector = ROOT_start_sector + 13;
DATA_start_sector = ROOT_end_sector + 1;
return;
}

int check(char* string){
int length = 0;
int size = (int)strlen(string);

while(length < size){
	if((*string < '0') || (*string > '9')){
		return -1;
		}
	else {
		string++;
		length++;
		}	
	}

return 1;
}

void list_subdir(int start, char* pwd, char* argv){
if(no_subdir == 1)return;
char* isDIR = "<DIR>";
int n;
unsigned char buf[32];
char parent_buf[1024];
char pwd_buf[1024];
strcpy(pwd_buf,pwd);
strcpy(parent_buf,pwd);
char temp_buf[1024];
char *t = temp_buf;
int num_of_entry_read = 0;

char file_attribute[5] = {'-','-','-','-','-'};
int *time;
int hour, minute,second;
int year, month, day;

int *date;
int *file_size;
int *p_cluster;
int starting_cluster;

lseek(fd, start, SEEK_SET);
if(n = read(fd, buf, 32) < 0){
	printf("error in reading sector\n");
	return;
	}
num_of_entry_read++;	
while(1){
// get file attribute notation
		if(((buf[11] & 0x20) >> 5) == 0x01)file_attribute[1] = 'A';
		if(((buf[11] & 0x02) >> 1) == 0x01)file_attribute[2] = 'H';
		if(((buf[11] & 0x04) >> 2) == 0x01)file_attribute[3] = 'S';
		if((buf[11] & 0x01) == 0x01)file_attribute[0] = 'R';

// get file last modification date and time
		time = (int*)(&buf[22]);
		date = (int*)(&buf[24]);
		second = (*time & 0x0000001F);
		minute = (*time & 0x000007E0)>> 5;
		hour = (*time & 0x0000F800) >> 11;
		day = (*date & 0x0000001F);
		month = (*date & 0x000001E0) >> 5;
		year = ((*date & 0x0000FE00) >> 9 )+ 1980;

// get the file size
		file_size = (int*)(&buf[28]);

// get the first cluster
		p_cluster = (int *)(&buf[26]);
		starting_cluster = (*p_cluster)& 0x0000FFFF;

		if((buf[11] == 0x0F) || (buf[0] == 0xE5)){
			memset(buf,0,32);
			if(n = read(fd, buf, 32) < 0){
				printf("error in reading sector\n");
				return;
				}
			num_of_entry_read++;	
			continue;
		}
		else{
		if((buf[0]!= 0x2E)&&(buf[0x1A] == 0x00))break;
		int a = 0;
		pwd = pwd_buf;
		strcpy(pwd_buf,parent_buf);

		t = temp_buf;
		strcat(pwd, "/");
		
		while(a < 11){
			if(buf[a] != 0x20){
				*t = buf[a];
				t++;
				}
				a++;
				if(a == 8 && (buf[9]!= 0x20) && (buf[11] != 0x10)){
					*t = '.';
					t++;
				}		
			}
		*t = '\0';
		strcat(pwd,temp_buf);	
			if(argv == NULL){
			if((buf[11] == 0x10))printf("%-32s%24s\n",pwd,isDIR);
			if((buf[11] != 0x10))printf("%-32s\n",pwd);
			}
		else{
			if((buf[11] == 0x10)){
				printf("%-10s%2d/%2d/%4d/ %2d:%2d:%02d \t   %-15s\t  %-24s%15d\n",file_attribute,month,day,year,hour,minute,second,isDIR,pwd,starting_cluster);
				}
			if((buf[11] != 0x10)){
				printf("%-10s%2d/%2d/%4d/ %2d:%2d:%02d \t\t\t%-10d%-24s%15d\n",file_attribute,month,day,year,hour,minute,second,*file_size,pwd,starting_cluster);
				}
		}

if((buf[11] == 0x10) && (buf[0] != 0x2E)){
					
					int subdir_sector = buf[26] + (buf[27] << 8);
					int start = (DATA_start_sector + subdir_sector - 2)* num_of_sector_per_cluster *num_of_bytes_per_sector;
					list_subdir(start, pwd_buf, argv);
//					memset(temp_buf,0,1024);
//					memset(pwd_buf,0,1024);
				}

lseek(fd, (start + (num_of_entry_read)*32) ,SEEK_SET);

					memset(temp_buf,0,1024);
					memset(pwd_buf,0,1024);
	
	memset(buf,0,32);
	if(n = read(fd, buf, 32) < 0){
				printf("error in reading sector\n");
				return;
				}	
	num_of_entry_read++;
		}
	}
return;
}
