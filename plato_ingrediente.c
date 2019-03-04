#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPlatoIngredienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_plato_ingrediente *dsplato_ingrediente;
    dsplato_ingrediente =  &(( (t_data_set_plato_ingrediente *) *rw)[rowi]);
    dsplato_ingrediente->cod_plato =atoi(PQgetvalue(res,rowi,0));
	dsplato_ingrediente->cod_ingrediente =atoi(PQgetvalue(res,rowi,1));
	dsplato_ingrediente->cantidad =atof(PQgetvalue(res,rowi,2));
}       
//----------------------------------------------------
void fillObjPlatoIngrediente(obj_plato_ingrediente *p,  t_data_set_plato_ingrediente rwitm)
{  
      p->cod_plato = rwitm.cod_plato;
	  p->cod_ingrediente = rwitm.cod_ingrediente;
	  p->cantidad = rwitm.cantidad;
}
//----------------------------------------------------
void fillRowsPlatoIngrediente(void **list, int size,void *data)
{
     int i;
     obj_plato_ingrediente *d;
    *list = (obj_plato_ingrediente **)malloc(sizeof(obj_plato_ingrediente*)* size);
    for(i=0;i<size;++i)
    {
      d = plato_ingrediente_new();
      fillObjPlatoIngrediente(d,((t_data_set_plato_ingrediente *)data)[i]);
      ((obj_plato_ingrediente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_plato_ingrediente(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_plato_ingrediente,fillPlatoIngredienteFromDB);
}
//----------------------------------------------------
void fill_dataset_plato_ingrediente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_plato_ingrediente]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_plato_ingrediente *)ds->rows)[i].cod_plato =((t_data_set_plato_ingrediente *)data)[i].cod_plato;
	 ((t_data_set_plato_ingrediente *)ds->rows)[i].cod_ingrediente =((t_data_set_plato_ingrediente *)data)[i].cod_ingrediente;
	 ((t_data_set_plato_ingrediente *)ds->rows)[i].cantidad =((t_data_set_plato_ingrediente *)data)[i].cantidad;
     }
}
//----------------------------------------------------
int findAll_plato_ingredienteImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_plato_ingrediente, ((data_set_plato_ingrediente*)((obj_plato_ingrediente*)self)->ds)->rows,fillPlatoIngredienteFromDB,fillRowsPlatoIngrediente);
}
//----------------------------------------------------
// implementacion de metodos para plato_ingrediente
int find_plato_ingredienteImpl(void *self, int cod_plato, int cod_ingrediente)
{
  int size=0;
  void *data;
  obj_plato_ingrediente *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " cod_plato = %d and cod_ingrediente =%d",cod_plato, cod_ingrediente);
   sql =getFindSQL(t_plato_ingrediente, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_plato_ingrediente*)((obj_plato_ingrediente*)self)->ds)->rows;
 size = exec_get_plato_ingrediente(sql,&data);
 fill_dataset_plato_ingrediente(((obj_plato_ingrediente*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_plato_ingrediente*)self;
   fillObjPlatoIngrediente(d,((t_data_set_plato_ingrediente *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_plato_ingredienteImpl(void *self, int cod_plato,int cod_ingrediente, float cantidad,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  data = ((data_set_plato_ingrediente*)((obj_plato_ingrediente*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_plato_ingrediente] , cod_plato, cod_ingrediente,cantidad);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_plato_ingrediente],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
  }
  else
  {// update
      sprintf(where,"cod_plato =%d and cod_ingrediente=%d ",cod_plato,cod_ingrediente);
      sprintf(values, sql_update_param_str[t_plato_ingrediente] , cantidad);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_plato_ingrediente],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_plato_ingrediente*)self)->cod_plato = cod_plato ;
	   ((obj_plato_ingrediente*)self)->cod_ingrediente = cod_ingrediente ;
	   ((obj_plato_ingrediente*)self)->cantidad = cantidad ;
       return 1;    
    }   
}
//----------------------------------------------------
obj_plato *get_platoImpl_plato_ingrediente(void *self)
{
   obj_plato_ingrediente* o;
   obj_plato *p;
   o = (obj_plato_ingrediente*) self;   
   p = plato_new();
   p->findbykey(p,o->cod_plato);     
   return p;
}
//----------------------------------------------------
obj_ingrediente *get_ingredienteImpl_plato_ingrediente(void *self)
{
   obj_plato_ingrediente* o;
   obj_ingrediente *i;
   o = (obj_plato_ingrediente*) self;   
   i = ingrediente_new();
   i->findbykey(i,o->cod_ingrediente);     
   return i;
}
//----------------------------------------------------
void *init_plato_ingrediente(void *self, data_set *ds)
{
  obj_plato_ingrediente *obj;
  obj = (obj_plato_ingrediente *)self;
  obj->ds = ds;
  obj->findbykey = find_plato_ingredienteImpl;
  obj->findAll = findAll_plato_ingredienteImpl;
  obj->saveObj = saveObj_plato_ingredienteImpl;  
  // inicializar relaciones con otros objetos del modelo
  obj->get_plato = get_platoImpl_plato_ingrediente;
  obj->get_ingrediente = get_ingredienteImpl_plato_ingrediente;  
  return obj;
}
//----------------------------------------------------
obj_plato_ingrediente *plato_ingrediente_new()
{
  return (obj_plato_ingrediente *)init_obj(t_plato_ingrediente, col_plato_ingrediente, init_plato_ingrediente);
}
