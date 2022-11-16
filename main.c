#include <stdio.h>
#include <dirent.h> /* return info about a dir */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> /* return info about a file: https://linux.die.net/man/2/stat */
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h> /* for window size */

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

#define MAX_DIR_SIZE 100
#define DATE_LEN 19
#define FSIZE_LEN 7


/*
 *	Function: readable_tm
 *	---------------------
 *	Converts a tm structure into a string formated like so:
 *  %s%d-%02d-%02d %02d:%02d:%02d
 *  
 *  local: a pointer to localtime() tm structure containing the time values (hours,minutes,secs,day of the month, etc)
 *  buf: a pointer to be populated with the formated string
 * */
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


/*
 *	Function: readable_fs
 *	---------------------
 *	Converts a floating number into the highest order of bytes it equals to
 *  
 *  size: a double representation of number of bytes
 *  buf: a pointer to be populated with the highest order number and its order
 * */
void readable_fs(double size, char* buf){
	int i = 0;
	const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

	while (size > 1000){
		size /=1000;
		i++;
	}
	sprintf(buf, "%.1f%s", size, units[i]);
	return;
}

void _ls(const char *dir, unsigned short term_size){
	
	struct dirent *d;
	struct stat buf;
	
	char fsize[10]; // file size
	char ftime[24]; // the time of the last modification to the file data.
	//TODO path needs to be dynamically allocated
	char path[200]; //path of the file


	int dsize=0;
	
	DIR *dh = opendir(dir); // open the the given directory
	
	if(!dh){
		if (errno == ENOENT){
			perror("Directory doesn't exist");
		}else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}

	printf("---------------  -------------------   	 -------\n");
	/*TODO: collect all the data into an array of struct that looks kinda like:
	 *  file_name: char* 
	 *  file_path: char*
	 *  file_type: (look into stat attributes)
	 *  file_mdate: tm structure
	 *  file_size: double | size_t
	 *
	 *  Than reorder the array based on the type and then based on the alphabetical order
	 *  of the filenames.
	 *
	 *  Colour filenames based on type
	 *
	 *  Keep the biggest string value
	 *
	 *  Finally print the "-"
	 *  followed by each file from the struct*
	 *  and finish with the according "-"
	 */
	while ((d = readdir(dh)) != NULL){
		/* check if the file name is "." and ".." */
		if (!strcmp(d->d_name,".") || !strcmp(d->d_name,"..")) continue;
		
		sprintf(path, "%s/%s", dir, d->d_name);
		
		//if (stat(d->d_name, &buf) < 0){
		if (stat(path, &buf) < 0){
			perror("stat");
			exit(EXIT_FAILURE);
		}
				
		readable_fs((long double) buf.st_size, fsize);
		readable_tm(localtime(&buf.st_mtime), ftime);
		

		printf("%-10s\t %s \t%s %s %s \n", 
				d->d_name, ftime ,COLOR_YELLOW, fsize, COLOR_RESET);
				
	}
	printf("---------------  -------------------   	 -------\n");
}

int main(int argc, const char *argv[]){

	/*TODO: store different types of files in different arrays*/
	/*TODO: sort all arrays */
	/*TODO: make colored output */
	/*TODO: file separet*/
	/*TODO: better path manipulation*/

	struct winsize size;
	
	/*get the terminal windows size*/
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	/* check if there is argument or not */
	switch (argc){
		case 1:
			_ls(".", size.ws_col);
			break;
		case 2:
			_ls(argv[1], size.ws_col);
			break;
		default:
			break;
	}
}
