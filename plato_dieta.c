#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPlatoDietaFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_plato_dieta *dsplato_dieta;
    dsplato_dieta =  &(( (t_data_set_plato_dieta *) *rw)[rowi]);
    dsplato_dieta->cod_plato =atoi(PQgetvalue(res,rowi,0));
	dsplato_dieta->cod_dieta =atoi(PQgetvalue(res,rowi,1));
	dsplato_dieta->porcion =atof(PQgetvalue(res,rowi,2));
    /*
	strcpy( dsplato_dieta->nombre ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsplato_dieta->autor ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsplato_dieta->fecha_alta ,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dsplato_dieta->descripcion ,rtrim(PQgetvalue(res,rowi,4),' '));
	*/
}       
//----------------------------------------------------
void fillObjPlatoDieta(obj_plato_dieta *p,  t_data_set_plato_dieta rwitm)
{  
      p->cod_plato = rwitm.cod_plato;
	  p->cod_dieta = rwitm.cod_dieta;
	  p->porcion = rwitm.porcion;
      /*
	  strcpy( p->nombre,rwitm.nombre);
      strcpy( p->autor,rwitm.autor);
      strcpy( p->fecha_alta,rwitm.fecha_alta);
      strcpy( p->descripcion,rwitm.descripcion);
	  */
}
//----------------------------------------------------
void fillRowsPlatoDieta(void **list, int size,void *data)
{
     int i;
     obj_plato_dieta *d;
    *list = (obj_plato_dieta **)malloc(sizeof(obj_plato_dieta*)* size);
    for(i=0;i<size;++i)
    {
      d = plato_dieta_new();
      fillObjPlatoDieta(d,((t_data_set_plato_dieta *)data)[i]);
      ((obj_plato_dieta **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_plato_dieta(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_plato_dieta,fillPlatoDietaFromDB);
}
//----------------------------------------------------
void fill_dataset_plato_dieta(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_plato_dieta]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_plato_dieta *)ds->rows)[i].cod_plato =((t_data_set_plato_dieta *)data)[i].cod_plato;
	 ((t_data_set_plato_dieta *)ds->rows)[i].cod_dieta =((t_data_set_plato_dieta *)data)[i].cod_dieta;
	 ((t_data_set_plato_dieta *)ds->rows)[i].porcion =((t_data_set_plato_dieta *)data)[i].porcion;
	 /*
      strcpy( ((t_data_set_plato_dieta *)ds->rows)[i].nombre,((t_data_set_plato_dieta *)data)[i].nombre);
      strcpy( ((t_data_set_plato_dieta *)ds->rows)[i].autor,((t_data_set_plato_dieta *)data)[i].autor);
      strcpy( ((t_data_set_plato_dieta *)ds->rows)[i].fecha_alta,((t_data_set_plato_dieta *)data)[i].fecha_alta);
      strcpy( ((t_data_set_plato_dieta *)ds->rows)[i].descripcion,((t_data_set_plato_dieta *)data)[i].descripcion);
	  */
     }
}
//----------------------------------------------------
int findAll_plato_dietaImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_plato_dieta, ((data_set_plato_dieta*)((obj_plato_dieta*)self)->ds)->rows,fillPlatoDietaFromDB,fillRowsPlatoDieta);
}
//----------------------------------------------------
// implementacion de metodos para plato_dieta
int find_plato_dietaImpl(void *self, int cp,int cd)
{
  int size=0;
  void *data;
  obj_plato_dieta *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " cod_plato = %d and cod_dieta= %d",cp,cd);
   sql =getFindSQL(t_plato_dieta, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_plato_dieta*)((obj_plato_dieta*)self)->ds)->rows;
 size = exec_get_plato_dieta(sql,&data);
 fill_dataset_plato_dieta(((obj_plato_dieta*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_plato_dieta*)self;
   fillObjPlatoDieta(d,((t_data_set_plato_dieta *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_plato_dietaImpl(void *self, int cod_plato, int cod_dieta, float porcion,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  obj_plato_dieta *o;
 
  
  data = ((data_set_plato_dieta*)((obj_plato_dieta*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_plato_dieta] , cod_plato, cod_dieta, porcion);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_plato_dieta],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
  }
  else
  {// update
      o = (obj_plato_dieta *)self;
      cod_plato = o->cod_plato;
	  cod_dieta = o->cod_dieta;

      sprintf(where,"cod_plato =%d and cod_dieta=%d",cod_plato,cod_dieta);
      sprintf(values, sql_update_param_str[t_plato_dieta] , porcion);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_plato_dieta],values,where);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_plato_dieta*)self)->cod_plato = cod_plato ;
	   ((obj_plato_dieta*)self)->cod_dieta = cod_dieta ;
	   ((obj_plato_dieta*)self)->porcion = porcion ;
       
       return 1;    
    }   
}
//----------------------------------------------------
obj_dieta *get_dietaImpl_plato_dieta(void *self)
{
   obj_plato_dieta* o;
   obj_dieta *d;
   o = (obj_plato_dieta*) self;   
   d = dieta_new();
   d->findbykey(d,o->cod_dieta);     
   return d;
}
//----------------------------------------------------
obj_plato *get_platoImpl_plato_dieta(void *self)
{
   obj_plato_dieta* o;
   obj_plato *p;
   o = (obj_plato_dieta*) self;   
   p = plato_new();
   p->findbykey(p,o->cod_plato);     
   return p;
}
//----------------------------------------------------
void *init_plato_dieta(void *self, data_set *ds)
{
  obj_plato_dieta *obj;
  obj = (obj_plato_dieta *)self;
  obj->ds = ds;
  obj->findbykey = find_plato_dietaImpl;
  obj->findAll = findAll_plato_dietaImpl;
  obj->saveObj = saveObj_plato_dietaImpl;  
  // inicializar relaciones con otros objetos del modelo
  obj->get_dieta = get_dietaImpl_plato_dieta;
  obj->get_plato = get_platoImpl_plato_dieta;
  return obj;
}
//----------------------------------------------------
obj_plato_dieta *plato_dieta_new()
{
  return (obj_plato_dieta *)init_obj(t_plato_dieta, col_plato_dieta, init_plato_dieta);
}
