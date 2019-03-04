#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillIngredienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_ingrediente *dsingrediente;
    dsingrediente =  &(( (t_data_set_ingrediente *) *rw)[rowi]);
    dsingrediente->codigo = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsingrediente->nombre ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsingrediente->unidadmed ,rtrim(PQgetvalue(res,rowi,2),' '));
}       
//----------------------------------------------------
void fillObjIngrediente(obj_ingrediente *p,  t_data_set_ingrediente rwitm)
{  
      p->codigo = rwitm.codigo;
      strcpy( p->nombre,rwitm.nombre);
      strcpy( p->unidadmed,rwitm.unidadmed);
}
//----------------------------------------------------
void fillRowsIngrediente(void **list, int size,void *data)
{
     int i;
     obj_ingrediente *d;
    *list = (obj_ingrediente **)malloc(sizeof(obj_ingrediente*)* size);
    for(i=0;i<size;++i)
    {
      d = ingrediente_new();
      fillObjIngrediente(d,((t_data_set_ingrediente *)data)[i]);
      ((obj_ingrediente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_ingrediente(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_ingrediente,fillIngredienteFromDB);
}
//----------------------------------------------------
void fill_dataset_ingrediente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_ingrediente]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_ingrediente *)ds->rows)[i].codigo =((t_data_set_ingrediente *)data)[i].codigo;
      strcpy( ((t_data_set_ingrediente *)ds->rows)[i].nombre,((t_data_set_ingrediente *)data)[i].nombre);
      strcpy( ((t_data_set_ingrediente *)ds->rows)[i].unidadmed,((t_data_set_ingrediente *)data)[i].unidadmed);
     }
}
//----------------------------------------------------
int findAll_ingredienteImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_ingrediente, ((data_set_ingrediente*)((obj_ingrediente*)self)->ds)->rows,fillIngredienteFromDB,fillRowsIngrediente);
}
//----------------------------------------------------
// implementacion de metodos para ingrediente
int find_ingredienteImpl(void *self, int k)
{
  int size=0;
  void *data;
  obj_ingrediente *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " codigo = %d",k);
   sql =getFindSQL(t_ingrediente, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_ingrediente*)((obj_ingrediente*)self)->ds)->rows;
 size = exec_get_ingrediente(sql,&data);
 fill_dataset_ingrediente(((obj_ingrediente*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_ingrediente*)self;
   fillObjIngrediente(d,((t_data_set_ingrediente *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_ingredienteImpl(void *self,char *nombre,char *unidadmed,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  obj_ingrediente *o;
  int codigo=0;
  data = ((data_set_ingrediente*)((obj_ingrediente*)self)->ds)->rows;
  if(isNew)
  {// insert    
    sprintf(values,sql_insert_param_str[t_ingrediente] , nombre, unidadmed);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_ingrediente],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
    // obtener ultimo codigo autogenerado
    res = PQexec(conn, "select max(codigo) codigo from ingredientes;");
    codigo = atoi(PQgetvalue(res,0,0));
    PQclear(res);    
  }
  else
  {// update
      o = (obj_ingrediente *)self;
      codigo = o->codigo;
      sprintf(where,"codigo =%d ",codigo);
      sprintf(values, sql_update_param_str[t_ingrediente] , nombre, unidadmed);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_ingrediente],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_ingrediente*)self)->codigo = codigo ;
       strcpy(((obj_ingrediente*)self)->nombre,nombre);
       strcpy(((obj_ingrediente*)self)->unidadmed,unidadmed);
       return 1;    
    }   
}
//----------------------------------------------------
void *init_ingrediente(void *self, data_set *ds)
{
  obj_ingrediente *obj;
  obj = (obj_ingrediente *)self;
  obj->ds = ds;
  obj->findbykey = find_ingredienteImpl;
  obj->findAll = findAll_ingredienteImpl;
  obj->saveObj = saveObj_ingredienteImpl;  
  return obj;
}
//----------------------------------------------------
obj_ingrediente *ingrediente_new()
{
  return (obj_ingrediente *)init_obj(t_ingrediente, col_ingrediente, init_ingrediente);
}
