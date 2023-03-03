#include <argp.h>
#include <error.h>

struct arguments{
	char *path;
};

const static char doc[] =
  "List information about the FILEs in a given directory (by default current directory)\
\nSort by stat's st_mode by default\
\vFor further help: <evgena.genchev@tutanota.com>\n\
lss repository is hosted at: <https://github.com/EvgeniGenchev/lss>\n";

const static char args_doc [] = "PATH";

const static struct argp_option options[] = {{0},};
