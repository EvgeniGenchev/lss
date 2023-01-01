#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

#define COLOR_RESET		"\x1b[0m"

#define COLOR_DIR		"#8EC07C"
#define COLOR_DATE		"#83a598"
#define COLOR_SIZE		"#fabd2f"
#define COLOR_REG		"#ebdbb2"
#define COLOR_LNK		"#076678"
#define COLOR_FIFO		"#98971a"
#define COLOR_SOCK		"#a89984"
#define COLOR_BLK		"#b16286"
#define COLOR_CHR		"#FCFFF0"
#define COLOR_SH		"#AFFFFF"

#define COLOR_TXT		"#9D0006"
#define COLOR_CODE		"#B57614"
#define COLOR_MEDIA		"#EABFFF"
#define COLOR_CONF		"#835194"

#define FORMAT			"%s%d-%02d-%02d %02d:%02d:%02d"
#define ARRAY_SIZE(x)	( sizeof(x) / sizeof((x)[0]) )

char* TXT_ARRAY[] =		{".txt", ".log", ".csv", ".md"};
char* MEDIA_ARRAY[] =	{".img", ".png", ".jpg", ".mp3", ".mp4", ".m4v", ".MOV"};
char* CONF_ARRAY[] =	{".yml", ".json", ".toml"};
char* CODE_ARRAY[] =	{".c", ".py", ".lua", ".cpp", ".h", ".hpp", ".js"};

const int TXT_ARR_LEN =		ARRAY_SIZE(TXT_ARRAY);
const int MEDIA_ARR_LEN =	ARRAY_SIZE(MEDIA_ARRAY);
const int CONF_ARR_LEN =	ARRAY_SIZE(CONF_ARRAY);
const int CODE_ARR_LEN =	ARRAY_SIZE(CODE_ARRAY);

unsigned long max=0;


int typeChecker(char* str, char* typeArray[], int arrayLen){

	int strLength = strlen(str);

	for(int i=0; i < arrayLen; i++){
		int len = strlen(typeArray[i]);

		//check if the string ends with the current string
		//if(strcmp(str + strlen(str) - len, typeArray[i])==0){
        if(strLength >= len && strcmp(str + strLength - len, typeArray[i])==0){
			return 1;
		}
	}
	return 0;
}


void print_colored (char* str, char* color){
	unsigned int r,g,b;
	sscanf(color, "#%2x%2x%2x", &r, &g, &b);
	printf("\033[38;2;%d;%d;%dm%s\033[0m",r,g,b, str);
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

	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", yyyy, mm, dd, h, min, sec);
}


/*
 *	Function: print_underscroe
 *	---------------------
 *	Prints n number of underscores
 * 
 *	n: the number of underscores to be printed
 * */
void print_underscore(int n){
	char buffer[n+1];

	memset(buffer, '_', n);

	buffer[n] = '\0';
	
	printf("%s", buffer);
}

/*
 *	Function: update_max
 *	---------------------
 *	Update the global max value
 * 
 *	n: the number to compare the current max value to
 * */
void update_max(unsigned long n){
	if (n > max){
		max = n;
	}
	return;
}

//int compare_entries(const void *a, const void *b) {
int compare_entries(const struct dirent **a, const struct dirent **b) {
  struct dirent **da = (struct dirent **)a;
  struct dirent **db = (struct dirent **)b;
  struct stat st_a, st_b;

  // Get the stat struct for each entry
  stat((*da)->d_name, &st_a);
  stat((*db)->d_name, &st_b);

  update_max(strlen((*da)->d_name));
  update_max(strlen((*db)->d_name));

  // Compare the types of the entries
  int type_cmp = (S_IFMT & st_b.st_mode) - (S_IFMT & st_a.st_mode);
  if (type_cmp != 0) {
    // If one is a directory and the other is not, the directory comes first
    if ((S_IFMT & st_a.st_mode) == S_IFDIR) {
      return -1;
    }
    if ((S_IFMT & st_b.st_mode) == S_IFDIR) {
      return 1;
    }
    // Otherwise, use the comparison of the types
    return type_cmp;
  }

  // If the types are the same, compare the names
  return strcmp((*da)->d_name, (*db)->d_name);
}


void _ls(){

	const char *dirPath = ".";
	DIR *dir = opendir(dirPath);
	
	char fsize[12];
	char ftime[24];

	if(!dir){
		if (errno == ENOENT){
			perror("Directory doesn't exist");
		}else{
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	
	struct dirent **entries;
	int num_entries = scandir(dirPath,
			&entries,
			NULL,
			compare_entries);

	if (num_entries < 0) {
		perror("Error reading directory");
		closedir(dir);
		return;
	}

	print_underscore((int) max);
	printf("  ___________________   ______\n");

	for (int i = 0; i < num_entries; i++) {
		struct stat st;

		if (strcmp(entries[i]->d_name, ".") == 0
			|| strcmp(entries[i]->d_name, "..") == 0) continue;
		
		if(stat(entries[i]->d_name, &st) < 0){
			printf("%s", entries[i]->d_name);
			
			for(int j=0; j < (int)(max - strlen(entries[i]->d_name)); j++){
				printf("   ");
			}
			printf("   ???????????????????   ??????\n");
			continue;
		}

		readable_fs((long double) st.st_size, fsize);
		readable_tm(localtime(&st.st_mtime), ftime);

		switch((S_IFMT & st.st_mode)){
			case S_IFDIR:
				print_colored(entries[i]->d_name, COLOR_DIR);
				break;

			case S_IFIFO:
				print_colored(entries[i]->d_name, COLOR_FIFO);
				break;

			//TODO: IT DOES NOT WORK		
			case S_IFLNK:
				print_colored(entries[i]->d_name, COLOR_LNK);
				break;
			
			case S_IFCHR:
				print_colored(entries[i]->d_name, COLOR_CHR);
				break;

			case S_IFBLK:
				print_colored(entries[i]->d_name, COLOR_BLK);
				break;
			
			case S_IFSOCK:
				print_colored(entries[i]->d_name, COLOR_SOCK);
				break;

			default:
				if (st.st_mode & S_IXUSR){
					print_colored(entries[i]->d_name, COLOR_SH);
				}
				else{
					if (typeChecker(entries[i]->d_name, CODE_ARRAY, CODE_ARR_LEN)){
						print_colored(entries[i]->d_name, COLOR_CODE);
					}
					else if (typeChecker(entries[i]->d_name, TXT_ARRAY, TXT_ARR_LEN)){
						print_colored(entries[i]->d_name, COLOR_TXT);
					}
					else if (typeChecker(entries[i]->d_name, MEDIA_ARRAY, MEDIA_ARR_LEN)){
						print_colored(entries[i]->d_name, COLOR_MEDIA);
					}
					else if (typeChecker(entries[i]->d_name, CONF_ARRAY, CONF_ARR_LEN)){
						print_colored(entries[i]->d_name, COLOR_CONF);
					
					}else print_colored(entries[i]->d_name, COLOR_REG);
				}
				break;


			}

			for(int j=0; j < (int)(max - strlen(entries[i]->d_name)); j++){
				printf(" ");
			}

			printf("  ");
			print_colored(ftime, COLOR_DATE);
			printf("   ");
			print_colored(fsize, COLOR_SIZE);
			printf("\n");
		}

	print_underscore((int) max);
	printf("  ___________________   ______\n");

	// Clean up
	for (int i = 0; i < num_entries; i++) {
		free(entries[i]);
	}
	
	free(entries);
	closedir(dir);
}

int main(int argc, char **argv) {
	switch (argc){
		case 1:
			_ls();
			break;

		case 2:

			if(chdir(argv[1]) != 0){
				perror("chdir error");
				return 1;
			}
			_ls();
			break;

		default:
			break;
	}
	return 0;
}
