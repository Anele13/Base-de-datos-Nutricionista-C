#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPacienteProfesionalFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_paciente_profesional *dspaciente_profesional;
    dspaciente_profesional =  &(( (t_data_set_paciente_profesional *) *rw)[rowi]);
    dspaciente_profesional->dni_paciente =atoi(PQgetvalue(res,rowi,0));
	dspaciente_profesional->dni_medico =atoi(PQgetvalue(res,rowi,1));
	strcpy( dspaciente_profesional->fecha_desde ,rtrim(PQgetvalue(res,rowi,2),' '));
	strcpy( dspaciente_profesional->fecha_hasta ,rtrim(PQgetvalue(res,rowi,3),' '));
}       
//----------------------------------------------------
void fillObjPacienteProfesional(obj_paciente_profesional *p,  t_data_set_paciente_profesional rwitm)
{  
      p->dni_paciente = rwitm.dni_paciente;
	  p->dni_medico = rwitm.dni_medico;
	  strcpy(p->fecha_desde ,  rwitm.fecha_desde);
	  strcpy(p->fecha_hasta ,  rwitm.fecha_hasta);
}
//----------------------------------------------------
void fillRowsPacienteProfesional(void **list, int size,void *data)
{
     int i;
     obj_paciente_profesional *d;
    *list = (obj_paciente_profesional **)malloc(sizeof(obj_paciente_profesional*)* size);
    for(i=0;i<size;++i)
    {
      d = paciente_profesional_new();
      fillObjPacienteProfesional(d,((t_data_set_paciente_profesional *)data)[i]);
      ((obj_paciente_profesional **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_paciente_profesional(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_paciente_profesional,fillPacienteProfesionalFromDB);
}
//----------------------------------------------------
void fill_dataset_paciente_profesional(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_paciente_profesional]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_paciente_profesional *)ds->rows)[i].dni_paciente =((t_data_set_paciente_profesional *)data)[i].dni_paciente;
	 ((t_data_set_paciente_profesional *)ds->rows)[i].dni_medico =((t_data_set_paciente_profesional *)data)[i].dni_medico;
	 strcpy( ((t_data_set_paciente_profesional *)ds->rows)[i].fecha_desde,((t_data_set_paciente_profesional *)data)[i].fecha_desde);
	 strcpy( ((t_data_set_paciente_profesional *)ds->rows)[i].fecha_hasta,((t_data_set_paciente_profesional *)data)[i].fecha_hasta);
     }
}
//----------------------------------------------------
int findAll_paciente_profesionalImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_paciente_profesional, ((data_set_paciente_profesional*)((obj_paciente_profesional*)self)->ds)->rows,fillPacienteProfesionalFromDB,fillRowsPacienteProfesional);
}
//----------------------------------------------------
// implementacion de metodos para paciente_profesional
int find_paciente_profesionalImpl(void *self, int cp,int cpac,char *fd)
{
  int size=0;
  void *data;
  obj_paciente_profesional *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " dni_paciente = %d and dni_medico= %d and fecha_desde='%s' ",cp,cpac,fd);
   sql =getFindSQL(t_paciente_profesional, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_paciente_profesional*)((obj_paciente_profesional*)self)->ds)->rows;
 size = exec_get_paciente_profesional(sql,&data);
 fill_dataset_paciente_profesional(((obj_paciente_profesional*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_paciente_profesional*)self;
   fillObjPacienteProfesional(d,((t_data_set_paciente_profesional *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_paciente_profesionalImpl(void *self, int dni_paciente,int dni_medico, char *fecha_hasta,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql; 
  void *data;
  char *fecha_desde;
  obj_paciente_profesional *o;
  
  data = ((data_set_paciente_profesional*)((obj_paciente_profesional*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_paciente_profesional] ,  dni_paciente,dni_medico);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_paciente_profesional],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
    fecha_desde = getFecha();
  }
  else
  {// update
      o = (obj_paciente_profesional *)self;
      dni_paciente = o->dni_paciente;
      dni_medico = o->dni_medico;
      strcpy(fecha_desde ,o->fecha_desde);
      sprintf(where,"dni_paciente =%d  and dni_medico=%d and fecha_desde = '%s'",dni_paciente,dni_medico,fecha_desde);
      sprintf(values, sql_update_param_str[t_paciente_profesional] , fecha_hasta);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_paciente_profesional],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_paciente_profesional*)self)->dni_paciente = dni_paciente ;
	   ((obj_paciente_profesional*)self)->dni_medico = dni_medico ;
	   strcpy(((obj_paciente_profesional*)self)->fecha_desde, fecha_desde) ;
	   strcpy(((obj_paciente_profesional*)self)->fecha_hasta, fecha_hasta) ;
	   
       return 1;    
    }   
}
//----------------------------------------------------
obj_paciente *get_pacienteImpl_paciente_profesional(void *self)
{
   obj_paciente_profesional* o;
   obj_paciente *p;
   o = (obj_paciente_profesional*) self;   
   p = paciente_new();
   p->findbykey(p,o->dni_paciente);     
   return p;
}
//----------------------------------------------------
obj_profesional *get_profesionalImpl_paciente_profesional(void *self)
{
   obj_paciente_profesional* o;
   obj_profesional *p;
   o = (obj_paciente_profesional*) self;   
   p = profesional_new();
   p->findbykey(p,o->dni_medico);     
   return p;
}
//----------------------------------------------------
void *init_paciente_profesional(void *self, data_set *ds)
{
  obj_paciente_profesional *obj;
  obj = (obj_paciente_profesional *)self;
  obj->ds = ds;
  obj->findbykey = find_paciente_profesionalImpl;
  obj->findAll = findAll_paciente_profesionalImpl;
  obj->saveObj = saveObj_paciente_profesionalImpl;  
  // inicializar relaciones con otros objetos del modelo
  obj->get_paciente = get_pacienteImpl_paciente_profesional;
  obj->get_profesional = get_profesionalImpl_paciente_profesional;
  return obj;
}
//----------------------------------------------------
obj_paciente_profesional *paciente_profesional_new()
{
  return (obj_paciente_profesional *)init_obj(t_paciente_profesional, col_paciente_profesional, init_paciente_profesional);
}
