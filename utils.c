#include<stdio.h>
#include<string.h>
#include <libpq-fe.h>  /* Inclusion de la libreria de acceso a funcionalidad para manipular objetos y conectarse a PostgreSQL */
#include <time.h>
#define MAXFECHA 17
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
char* rtrim(char* string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}
//----------------------------------------------------
char** fStrSplit(char *str, const char *delimiters)
{
    char * token; 
    char **tokenArray;
    int count=0;
    token = (char *)strtok(str, delimiters); // Get the first token
    tokenArray = (char**)malloc(1 * sizeof(char*));
    tokenArray[0] = NULL;
    if (!token) {       
        return tokenArray;  
    } 
    while (token != NULL) { // While valid tokens are returned
        tokenArray[count] = (char*)strdup(token);
        //printf ("%s", tokenArray[count]);
        count++;
        tokenArray = (char **)realloc(tokenArray, sizeof(char *) * (count + 1));
        token = (char *)strtok(NULL, delimiters); // Get the next token
    }
    tokenArray[count] = NULL;  /* Terminate the array */
    return tokenArray;
}
//----------------------------------------------------
char* fStrJoin(char **str, const char *delimiters, int sz_opt)
{
    char *joinedStr;
    int i = 1, size=0;
    
    if(sz_opt!=0 ) 
    {
      size = sz_opt;
    }
    else
      size = strlen(str[0])+1;
    joinedStr = (char *)realloc(NULL, size);
    strcpy(joinedStr, str[0]);
    if (str[0] == NULL){
        return joinedStr;
    }
    while (i< size ){
        joinedStr = (char*)realloc(joinedStr, strlen(joinedStr) + strlen(str[i]) + strlen(delimiters) + 1);
        strcat(joinedStr, delimiters);
        strcat(joinedStr, str[i]);
        i++;
    }
    return joinedStr;
}
//----------------------------------------------------
void disconnectdb()
{   
   /*Cerrar conexion y liberar recurso de conexion con el servidor*/
   PQfinish(conn);
   printf("Desconectado...\n");
}
//----------------------------------------------------
void connectdb(char *servidor,char *port,char *base,char *usuario,char *password)
{
   /*Realizar la conexion al servidor*/
   conn = PQsetdbLogin(servidor, port, NULL,NULL, base,usuario,password);
   /*Consultar estado de la operacion de conexion*/

   if (PQstatus(conn) != CONNECTION_OK)
      fprintf(stderr, "La conexion a la Base de Datos ha fallado: %s", PQerrorMessage(conn) );        
   else
      fprintf(stdout, "Estado: Conectado a la Base de Datos...\n" );        
 }
//----------------------------------------------------
char *getFecha()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
     return ffecha;
}
//----------------------------------------------------
char *getFechaHora()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d %02d:%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min);
     return ffecha;
}
