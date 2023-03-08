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

#define EMPTY_MSG		"There is nothing to see in here :)"

// must be set as string like so: "_"
#define BORDER_CHAR		"_"

#define FORMAT			"%s%d-%02d-%02d %02d:%02d:%02d" //does nothing yet
#define ARRAY_SIZE(x)	( sizeof(x) / sizeof((x)[0]) )

const char* TXT_ARRAY[] =	{".txt", ".log", ".csv", ".md"};
const char* MEDIA_ARRAY[] =	{".img", ".png", ".jpg", ".mp3", ".mp4", ".m4v", ".MOV"};
const char* CONF_ARRAY[] =	{".yml", ".json", ".toml"};
const char* CODE_ARRAY[] =	{".c", ".py", ".lua", ".cpp", ".h", ".hpp", ".js"};

const int TXT_ARR_LEN =		ARRAY_SIZE(TXT_ARRAY);
const int MEDIA_ARR_LEN =	ARRAY_SIZE(MEDIA_ARRAY);
const int CONF_ARR_LEN =	ARRAY_SIZE(CONF_ARRAY);
const int CODE_ARR_LEN =	ARRAY_SIZE(CODE_ARRAY);

const int EMPTY_MSG_LEN =	ARRAY_SIZE(EMPTY_MSG);
