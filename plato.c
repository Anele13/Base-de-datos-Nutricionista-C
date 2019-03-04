#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPlatoFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_plato *dsplato;
    dsplato =  &(( (t_data_set_plato *) *rw)[rowi]);
    dsplato->codigo =atoi(PQgetvalue(res,rowi,0));
    strcpy( dsplato->nombre ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsplato->detalle ,rtrim(PQgetvalue(res,rowi,2),' '));
}       
//----------------------------------------------------
void fillObjPlato(obj_plato *p,  t_data_set_plato rwitm)
{  
      p->codigo = rwitm.codigo;
      strcpy( p->nombre,rwitm.nombre);
      strcpy( p->detalle,rwitm.detalle);
}
//----------------------------------------------------
void fillRowsPlato(void **list, int size,void *data)
{
     int i;
     obj_plato *d;
    *list = (obj_plato **)malloc(sizeof(obj_plato*)* size);
    for(i=0;i<size;++i)
    {
      d = plato_new();
      fillObjPlato(d,((t_data_set_plato *)data)[i]);
      ((obj_plato **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_plato(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_plato,fillPlatoFromDB);
}
//----------------------------------------------------
void fill_dataset_plato(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_plato]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_plato *)ds->rows)[i].codigo =((t_data_set_plato *)data)[i].codigo;
      strcpy( ((t_data_set_plato *)ds->rows)[i].nombre,((t_data_set_plato *)data)[i].nombre);
      strcpy( ((t_data_set_plato *)ds->rows)[i].detalle,((t_data_set_plato *)data)[i].detalle);
     }
}
//----------------------------------------------------
int findAll_platoImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_plato, ((data_set_plato*)((obj_plato*)self)->ds)->rows,fillPlatoFromDB,fillRowsPlato);
}
//----------------------------------------------------
// implementacion de metodos para plato
int find_platoImpl(void *self, int k)
{
  int size=0;
  void *data;
  obj_plato *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " codigo = %d",k);
   sql =getFindSQL(t_plato, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_plato*)((obj_plato*)self)->ds)->rows;
 size = exec_get_plato(sql,&data);
 fill_dataset_plato(((obj_plato*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_plato*)self;
   fillObjPlato(d,((t_data_set_plato *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_platoImpl(void *self, char *nombre,char *detalle,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  obj_plato *o;
  int codigo=0;

  data = ((data_set_plato*)((obj_plato*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_plato] , nombre, detalle);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_plato],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
    // obtener ultimo codigo autogenerado
    res = PQexec(conn, "select max(codigo) codigo from platos;");
    codigo = atoi(PQgetvalue(res,0,0));
    PQclear(res);
  }
  else
  {// update
      o = (obj_plato *)self;
      codigo = o->codigo;
      sprintf(where,"codigo =%d ",codigo);
      sprintf(values, sql_update_param_str[t_plato] , nombre, detalle);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_plato],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_plato*)self)->codigo = codigo ;
       strcpy(((obj_plato*)self)->nombre,nombre);
       strcpy(((obj_plato*)self)->detalle,detalle);
       return 1;    
    }   
}
//----------------------------------------------------
void *init_plato(void *self, data_set *ds)
{
  obj_plato *obj;
  obj = (obj_plato *)self;
  obj->ds = ds;
  obj->findbykey = find_platoImpl;
  obj->findAll = findAll_platoImpl;
  obj->saveObj = saveObj_platoImpl;  
  return obj;
}
//----------------------------------------------------
obj_plato *plato_new()
{
  return (obj_plato *)init_obj(t_plato, col_plato, init_plato);
}
