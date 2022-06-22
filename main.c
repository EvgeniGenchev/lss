#include <stdio.h>
#include <dirent.h> /* return info about a dir */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> /* return info about a file: https://linux.die.net/man/2/stat */
#include <unistd.h>

/* file types */
#define BLOCK_DEV	1
#define CHAR_DEV	2
#define FIFO		3
#define SYMLINK		4
#define REG_FILE	5
#define SOCKET		6
#define UNKNOWN		0


void _ls(const char *dir){
	
	struct dirent *d;
	struct stat buf;
	
	DIR *dh = opendir(dir);

	if(!dh){
		if (errno = ENOENT){
			perror("Directory doesn't exist");
		}else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}

	while ((d = readdir(dh)) != NULL){
		if ( !strcmp(d->d_name,".") || !strcmp(d->d_name,"..")) continue;

		if(stat(d->d_name, &buf) < 0) {
			perror("stat");
			exit(EXIT_FAILURE);
		}
		
		printf("%s\t %lld bytes \n", d->d_name, (long long) buf.st_size);
	}
}

int main(int argc, const char *argv[]){

	/*TODO: store different types of files in different arrays*/
	/*TODO: sort all arrays */
	/*TODO: make colored output */
	
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
