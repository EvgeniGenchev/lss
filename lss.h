#include <argp.h>
#include <error.h>

#define H_FSIZE_LEN 7
#define H_FTIME_LEN 20


struct arguments{
	char *path;
	int all;
	int almost;
};

const static char doc[] =
  "List information about the FILEs in a given directory (by default current directory)\
\nSort by stat's st_mode by default\
\vFor further help: <evgena.genchev@tutanota.com>\n\
lss repository is hosted at: <https://github.com/EvgeniGenchev/lss>\n";

const static char args_doc [] = "PATH";

const static struct argp_option options[] = {
	{"all",			'a',	0,0,	"do not ignore entries starting with ."},
	{"almost-all",	'A',	0,0,	"do not list implied . and .."},
	{0},
};

