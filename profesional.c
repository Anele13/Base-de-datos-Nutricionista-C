#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillProfesionalFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_profesional *dsprofesional;
    dsprofesional = &(( (t_data_set_profesional *) *rw)[rowi]);
    dsprofesional->dni=atoi(PQgetvalue(res,rowi,0));
	strcpy( dsprofesional->matricula ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsprofesional->nombre ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsprofesional->apellido ,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dsprofesional->telefono ,rtrim(PQgetvalue(res,rowi,4),' '));
	strcpy( dsprofesional->fecha_alta ,rtrim(PQgetvalue(res,rowi,5),' '));
    dsprofesional->es_med=atoi(PQgetvalue(res,rowi,6));
    dsprofesional->es_nutri=atoi(PQgetvalue(res,rowi,7));
}       
//----------------------------------------------------
void fillObjProfesional(obj_profesional *p,  t_data_set_profesional rwitm)
{  
      p->dni = rwitm.dni;
      strcpy( p->matricula,rwitm.matricula);
	  strcpy( p->nombre,rwitm.nombre);
      strcpy( p->apellido,rwitm.apellido);
      strcpy( p->telefono,rwitm.telefono);
	  strcpy( p->fecha_alta,rwitm.fecha_alta);
      strcpy( p->telefono,rwitm.telefono);
      p->es_med = rwitm.es_med;
      p->es_nutri = rwitm.es_nutri;      
}
//----------------------------------------------------
void fillRowsProfesional(void **list, int size,void *data)
{
     int i;
     obj_profesional *d;
    *list = (obj_profesional **)malloc(sizeof(obj_profesional*)* size);
    for(i=0;i<size;++i)
    {
      d = profesional_new();
      fillObjProfesional(d,((t_data_set_profesional *)data)[i]);
      ((obj_profesional **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_profesional(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_profesional,fillProfesionalFromDB);
}
//----------------------------------------------------
void fill_dataset_profesional(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_profesional]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
      ((t_data_set_profesional *)ds->rows)[i].dni =((t_data_set_profesional *)data)[i].dni;
	  strcpy( ((t_data_set_profesional *)ds->rows)[i].matricula,((t_data_set_profesional *)data)[i].matricula);
      strcpy( ((t_data_set_profesional *)ds->rows)[i].nombre,((t_data_set_profesional *)data)[i].nombre);
      strcpy( ((t_data_set_profesional *)ds->rows)[i].apellido,((t_data_set_profesional *)data)[i].apellido);
      strcpy( ((t_data_set_profesional *)ds->rows)[i].telefono,((t_data_set_profesional *)data)[i].telefono);
	  strcpy( ((t_data_set_profesional *)ds->rows)[i].fecha_alta,((t_data_set_profesional *)data)[i].fecha_alta);
      ((t_data_set_profesional *)ds->rows)[i].es_med =((t_data_set_profesional *)data)[i].es_med;
      ((t_data_set_profesional *)ds->rows)[i].es_nutri =((t_data_set_profesional *)data)[i].es_nutri;
     }
}
//----------------------------------------------------
int findAll_profesionalImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria,t_profesional, ((data_set_profesional*)((obj_profesional*)self)->ds)->rows,fillProfesionalFromDB,fillRowsProfesional);
}
//----------------------------------------------------
// implementacion de metodos para profesional
int find_profesionalImpl(void *self, int k)
{
  int size=0;
  void *data;  
  obj_profesional *p;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, "dni = %d",k);
   sql =getFindSQL(t_profesional, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_profesional*)((obj_profesional*)self)->ds)->rows;
 size = exec_get_profesional(sql,&data);
 fill_dataset_profesional(((obj_profesional*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 {
   p = (obj_profesional*)self;
   fillObjProfesional(p,((t_data_set_profesional *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_profesionalImpl(void *self, int dni,char *matricula,char *nombre,char *apellido,char *telefono,int es_med,int es_nutri,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  char *fecha_alta;
  obj_profesional *o;
  
  data = ((data_set_profesional*)((obj_profesional*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_profesional] , dni, matricula,nombre, apellido, telefono, es_med, es_nutri);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_profesional],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
    // obtener fecha actual
    fecha_alta = getFecha();    
  }
  else
  {// update
      o = (obj_profesional *)self;
      strcpy(fecha_alta,o->fecha_alta);
      sprintf(where,"dni =%d ",dni);
      sprintf(values, sql_update_param_str[t_profesional] , matricula,nombre, apellido, telefono, es_med, es_nutri);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_profesional],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_profesional*)self)->dni = dni ;
       strcpy(((obj_profesional*)self)->matricula,matricula);
	   strcpy(((obj_profesional*)self)->nombre,nombre);
       strcpy(((obj_profesional*)self)->apellido,apellido);
       strcpy(((obj_profesional*)self)->telefono,telefono);
	   strcpy(((obj_profesional*)self)->fecha_alta,fecha_alta);
       ((obj_profesional*)self)->es_med= es_med;
       ((obj_profesional*)self)->es_nutri= es_nutri;
       return 1;    
    }   
}
/*
//----------------------------------------------------
obj_profesional *profesional_new()
{
  obj_profesional *obj = (obj_profesional *)malloc(size_st[t_profesional]);  
  tipos t=t_profesional;
  data_set *ds;
  ds = malloc(size_st[t]);
  ds->columns = (char **)malloc(size_col[t]);
  ds->cntRows=0;
  ds->cntCol = cnt_cols[t];
  memcpy(((data_set_profesional *)ds)->columns,col_profesional,sizeof(col_profesional));
  obj->ds = ds;
  obj->findbykey = find_profesionalImpl;
  obj->findAll = findAll_profesionalImpl;
  obj->saveObj = saveObj_profesionalImpl;
 return obj;
}
*/
//----------------------------------------------------
void *init_profesional(void *self, data_set *ds)
{
  obj_profesional *obj;
  obj = (obj_profesional *)self;
  obj->ds = ds;
  obj->findbykey = find_profesionalImpl;
  obj->findAll = findAll_profesionalImpl;
  obj->saveObj = saveObj_profesionalImpl;  
  return obj;
}
//----------------------------------------------------
obj_profesional *profesional_new()
{
  return (obj_profesional *)init_obj(t_profesional, col_profesional, init_profesional);
}
