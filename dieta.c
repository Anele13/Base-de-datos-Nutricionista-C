#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillDietaFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_dieta *dsdieta;
    dsdieta =  &(( (t_data_set_dieta *) *rw)[rowi]);
    dsdieta->codigo =atoi(PQgetvalue(res,rowi,0));
    strcpy( dsdieta->nombre ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsdieta->autor ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsdieta->fecha_alta ,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dsdieta->descripcion ,rtrim(PQgetvalue(res,rowi,4),' '));
}       
//----------------------------------------------------
void fillObjDieta(obj_dieta *p,  t_data_set_dieta rwitm)
{  
      p->codigo = rwitm.codigo;
      strcpy( p->nombre,rwitm.nombre);
      strcpy( p->autor,rwitm.autor);
      strcpy( p->fecha_alta,rwitm.fecha_alta);
      strcpy( p->descripcion,rwitm.descripcion);
}
//----------------------------------------------------
void fillRowsDieta(void **list, int size,void *data)
{
     int i;
     obj_dieta *d;
    *list = (obj_dieta **)malloc(sizeof(obj_dieta*)* size);
    for(i=0;i<size;++i)
    {
      d = dieta_new();
      fillObjDieta(d,((t_data_set_dieta *)data)[i]);
      ((obj_dieta **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_dieta(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_dieta,fillDietaFromDB);
}
//----------------------------------------------------
void fill_dataset_dieta(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_dieta]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_dieta *)ds->rows)[i].codigo =((t_data_set_dieta *)data)[i].codigo;
      strcpy( ((t_data_set_dieta *)ds->rows)[i].nombre,((t_data_set_dieta *)data)[i].nombre);
      strcpy( ((t_data_set_dieta *)ds->rows)[i].autor,((t_data_set_dieta *)data)[i].autor);
      strcpy( ((t_data_set_dieta *)ds->rows)[i].fecha_alta,((t_data_set_dieta *)data)[i].fecha_alta);
      strcpy( ((t_data_set_dieta *)ds->rows)[i].descripcion,((t_data_set_dieta *)data)[i].descripcion);
     }
}
//----------------------------------------------------
int findAll_dietaImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_dieta, ((data_set_dieta*)((obj_dieta*)self)->ds)->rows,fillDietaFromDB,fillRowsDieta);
}
//----------------------------------------------------
// implementacion de metodos para dieta
int find_dietaImpl(void *self, int k)
{
  int size=0;
  void *data;
  obj_dieta *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " codigo = %d",k);
   sql =getFindSQL(t_dieta, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_dieta*)((obj_dieta*)self)->ds)->rows;
 size = exec_get_dieta(sql,&data);
 fill_dataset_dieta(((obj_dieta*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_dieta*)self;
   fillObjDieta(d,((t_data_set_dieta *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_dietaImpl(void *self,char *nombre,char *autor,char *descripcion,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  obj_dieta *o;
  int codigo=0;
  char *fecha_alta;
  
  data = ((data_set_dieta*)((obj_dieta*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_dieta] , nombre, autor, descripcion);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_dieta],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
     // obtener ultimo codigo autogenerado
    res = PQexec(conn, "select max(codigo) codigo from dieta;");
    codigo = atoi(PQgetvalue(res,0,0));
    PQclear(res);
    // obtener fecha actual
    fecha_alta = getFecha();
  }
  else
  {// update
      o = (obj_dieta *)self;
      codigo = o->codigo;
      strcpy(fecha_alta,o->fecha_alta);
      sprintf(where,"codigo =%d ",codigo);
      sprintf(values, sql_update_param_str[t_dieta] , nombre, autor, descripcion);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_dieta],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_dieta*)self)->codigo = codigo ;
       strcpy(((obj_dieta*)self)->nombre,nombre);
       strcpy(((obj_dieta*)self)->autor,autor);
       strcpy(((obj_dieta*)self)->fecha_alta,fecha_alta);
       strcpy(((obj_dieta*)self)->descripcion,descripcion);
       return 1;    
    }   
}
//----------------------------------------------------
void *init_dieta(void *self, data_set *ds)
{
  obj_dieta *obj;
  obj = (obj_dieta *)self;
  obj->ds = ds;
  obj->findbykey = find_dietaImpl;
  obj->findAll = findAll_dietaImpl;
  obj->saveObj = saveObj_dietaImpl;  
  return obj;
}
//----------------------------------------------------
obj_dieta *dieta_new()
{
  return (obj_dieta *)init_obj(t_dieta, col_dieta, init_dieta);
}
