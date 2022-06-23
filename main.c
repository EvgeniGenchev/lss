#include <stdio.h>
#include <dirent.h> /* return info about a dir */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> /* return info about a file: https://linux.die.net/man/2/stat */
#include <unistd.h>
#include <time.h>

/* file types */
#define BLOCK_DEV	1
#define CHAR_DEV	2
#define FIFO		3
#define SYMLINK		4
#define REG_FILE	5
#define SOCKET		6
#define UNKNOWN		0

#define COLOR_RED		"\x1b[91m"
#define COLOR_GREEN		"\x1b[92m"
#define COLOR_YELLOW	"\x1b[93m"
#define COLOR_BLUE		"\x1b[94m"
#define COLOR_MAGENTA	"\x1b[95m"
#define COLOR_CYAN		"\x1b[96m"
#define COLOR_RESET		"\x1b[0m"

void readable_tm(struct tm *local, char* buf){
	
	int h, min, sec, dd, mm, yyyy;
	
	h = local->tm_hour;
	min = local->tm_min;
	sec = local->tm_sec;

	dd = local->tm_mday;
	mm = local->tm_mon + 1;
	yyyy = local->tm_year + 1900; 

	sprintf(buf, "%s%d-%02d-%02d %02d:%02d:%02d",COLOR_BLUE, yyyy, mm, dd, h, min, sec);
}

void readable_fs(double size, char* buf){
	int i = 0;
	const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

	while (size > 1000){
		size /=1000;
		i++;
	}
	sprintf(buf, "%.1f %s", size, units[i]);
	return;
}

void _ls(const char *dir){
	
	struct dirent *d;
	struct stat buf;
	char fsize[10];
	char ftime[24];
	
	DIR *dh = opendir(dir);

	if(!dh){
		if (errno == ENOENT){
			perror("Directory doesn't exist");
		}else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	
	printf("---------------  -------------------   	 -------\n");
	while ((d = readdir(dh)) != NULL){
		if ( !strcmp(d->d_name,".") || !strcmp(d->d_name,"..")) continue;

		stat(d->d_name, &buf);
		
		readable_fs((long double) buf.st_size, fsize);
		readable_tm(localtime(&buf.st_mtime), ftime);

		printf("%-10s\t %s \t%s %5s %s\n", 
				d->d_name, ftime ,COLOR_YELLOW, fsize, COLOR_RESET);
	}
	printf("---------------  -------------------   	 -------\n");
}

int main(int argc, const char *argv[]){

	/*TODO: store different types of files in different arrays*/
	/*TODO: sort all arrays */
	/*TODO: make colored output */
	/*TODO: file separet*/
	
	switch (argc){
		case 1:
			_ls(".");
			break;
		case 2:
			_ls(argv[1]);
			break;
		default:
			break;
	}
}
