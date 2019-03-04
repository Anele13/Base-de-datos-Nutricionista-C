#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor

int exec_get_fromDB(char *sql,void **rw, tipos t,void(*fcFill)(void **,int ri,PGresult *r))
{
    int tuplas=0,i=0;
    PGresult *res; //Instancia para contener los datos de ejecucion de consulta
    res = PQexec(conn, sql);    
    if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res))
    {
        tuplas = PQntuples(res);
        if(tuplas!=0)
        {   
          *rw = malloc(size_st[t]*tuplas);
          for (i = 0; i<tuplas; i++)
          {
            fcFill(rw,i,res);
          }          
        }
        PQclear(res);
    }
    return tuplas;;
}
//----------------------------------------------------
/* interface generica para poder ejecuar fidall obj */
int findAllImpl(void *self,void **list, char *criteria,tipos t, void *data,void(*fcFill)(void **,int ri,PGresult *r),void(*fillRows)(void **list,int size,void *data))
{
  int size=0, i=0;
  obj_dieta *d;
  int len=0;
  char *sql, *str_where=NULL;
   if(criteria!=NULL)
   {
     str_where = (char*)malloc(sizeof(char)*MAX_WHERE_SQL);
     memset(str_where, 0, MAX_WHERE_SQL);                 
     str_where = strcat(str_where,"where ");
     str_where = strcat( str_where , criteria );
   }
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( sql, MAX_SQL, sql_select_str[t],(str_where==NULL?"":str_where ));      

  size = exec_get_fromDB(sql,&data,t,fcFill);
  if(size>0)
    fillRows(list,size, data);
  return size;
}
//----------------------------------------------------
char *getFindSQL(tipos t, char *where)
{
  //char *where;
  //where = (char*)malloc(sizeof(char)*MAX_SQL);
  //snprintf( where, MAX_SQL, where_str,k);
  char *sql, *str_where=NULL;
   // verificar si requiere configurar la condicion del where de la sentencia SQL
     str_where = (char*)malloc(sizeof(char)*MAX_WHERE_SQL);
     memset(str_where, 0, MAX_WHERE_SQL);                 
     str_where = strcat(str_where,"where ");
     str_where = strcat( str_where , where );
   
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( sql, MAX_SQL, sql_select_str[t],(str_where==NULL?"":str_where ));      
   return sql;
}
//----------------------------------------------------
void *init_obj(tipos t, const char *col[], void*(*init)(void *o,data_set *ds))
{     
     data_set *ds;
     // pedir memoria para el objeto gral
     void *obj = malloc(size_st[t]);
     
     ds = malloc(size_st[t]);
     ds->columns = (char **)malloc(size_col[t]);
     ds->cntRows=0;
     ds->cntCol = cnt_cols[t];
     memcpy(ds->columns,col,size_col[t]);
     return init(obj,ds);      
}
