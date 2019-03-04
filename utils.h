char* rtrim(char* string, char junk);
char** fStrSplit(char *str, const char *delimiters);
char* fStrJoin(char **str, const char *delimiters, int sz_opt);
void disconnectdb();
void connectdb(char *servidor,char *port,char *base,char *usuario,char *password);
