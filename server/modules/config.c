char *CONNECT_MESSAGE =
"\n ### Hello! Welcome to chat. ### \n\n"
"List of commands:\n"
"/NICK [nickname] - change your nickname\n"
"/LIST            - List all channels\n"
"/JOIN [channel]  - Join channel\n"
"/PART            - Leave your current channel\n";

char *DEFAULT_NAME = "NONAME";
char *DEFAULT_CHANNEL = "NOCHANNEL";
int MAX_CHANNELS = 1000;

char INITIAL_CHANNELS[][20] = {"Kanava3", "HappyGarden"};
int LEN_INITIAL_CHANNELS = 2;

char *RESPONSE_OK = "RESPONSE_OK";
