#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPlatoPacienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_plato_paciente *dsplato_paciente;
    dsplato_paciente =  &(( (t_data_set_plato_paciente *) *rw)[rowi]);
    dsplato_paciente->cod_plato =atoi(PQgetvalue(res,rowi,0));
	dsplato_paciente->dni_paciente =atoi(PQgetvalue(res,rowi,1));
	dsplato_paciente->cantidad =atof(PQgetvalue(res,rowi,2));
	strcpy( dsplato_paciente->fecha ,rtrim(PQgetvalue(res,rowi,3),' '));
}       
//----------------------------------------------------
void fillObjPlatoPaciente(obj_plato_paciente *p,  t_data_set_plato_paciente rwitm)
{  
      p->cod_plato = rwitm.cod_plato;
	  p->dni_paciente = rwitm.dni_paciente;
	  p->cantidad = rwitm.cantidad;
	  strcpy(p->fecha ,  rwitm.fecha);
}
//----------------------------------------------------
void fillRowsPlatoPaciente(void **list, int size,void *data)
{
     int i;
     obj_plato_paciente *d;
    *list = (obj_plato_paciente **)malloc(sizeof(obj_plato_paciente*)* size);
    for(i=0;i<size;++i)
    {
      d = plato_paciente_new();
      fillObjPlatoPaciente(d,((t_data_set_plato_paciente *)data)[i]);
      ((obj_plato_paciente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_plato_paciente(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_plato_paciente,fillPlatoPacienteFromDB);
}
//----------------------------------------------------
void fill_dataset_plato_paciente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_plato_paciente]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
     ((t_data_set_plato_paciente *)ds->rows)[i].cod_plato =((t_data_set_plato_paciente *)data)[i].cod_plato;
	 ((t_data_set_plato_paciente *)ds->rows)[i].dni_paciente =((t_data_set_plato_paciente *)data)[i].dni_paciente;
	 ((t_data_set_plato_paciente *)ds->rows)[i].cantidad =((t_data_set_plato_paciente *)data)[i].cantidad;
	 strcpy( ((t_data_set_plato_paciente *)ds->rows)[i].fecha,((t_data_set_plato_paciente *)data)[i].fecha);
     }
}
//----------------------------------------------------
int findAll_plato_pacienteImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_plato_paciente, ((data_set_plato_paciente*)((obj_plato_paciente*)self)->ds)->rows,fillPlatoPacienteFromDB,fillRowsPlatoPaciente);
}
//----------------------------------------------------
// implementacion de metodos para plato_paciente
int find_plato_pacienteImpl(void *self, int cp,int cpac,char *ff)
{
  int size=0;
  void *data;
  obj_plato_paciente *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " cod_plato = %d and dni_paciente= %d and fecha='%s' ",cp,cpac,ff);
   sql =getFindSQL(t_plato_paciente, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_plato_paciente*)((obj_plato_paciente*)self)->ds)->rows;
 size = exec_get_plato_paciente(sql,&data);
 fill_dataset_plato_paciente(((obj_plato_paciente*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_plato_paciente*)self;
   fillObjPlatoPaciente(d,((t_data_set_plato_paciente *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_plato_pacienteImpl(void *self, int cod_plato,int dni_paciente, float cantidad,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  char *fecha;
  obj_plato_paciente *o;
  
  data = ((data_set_plato_paciente*)((obj_plato_paciente*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_plato_paciente] ,  cod_plato,dni_paciente,cantidad);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_plato_paciente],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
    fecha = getFechaHora();
  }
  else
  {// update
      o = (obj_plato_paciente *)self;
      strcpy(fecha,o->fecha);
      sprintf(where,"cod_plato =%d  and dni_paciente=%d and fecha = '%s'",cod_plato,dni_paciente,fecha);
      sprintf(values, sql_update_param_str[t_plato_paciente] , cantidad);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_plato_paciente],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_plato_paciente*)self)->cod_plato = cod_plato ;
	   ((obj_plato_paciente*)self)->dni_paciente = dni_paciente ;
	   ((obj_plato_paciente*)self)->cantidad = cantidad ;
	   strcpy(((obj_plato_paciente*)self)->fecha, fecha) ;
       return 1;    
    }   
}
//----------------------------------------------------
obj_paciente *get_pacienteImpl_plato_paciente(void *self)
{
   obj_plato_paciente* o;
   obj_paciente *p;
   o = (obj_plato_paciente*) self;   
   p = paciente_new();
   p->findbykey(p,o->dni_paciente);     
   return p;
}
//----------------------------------------------------
obj_plato *get_platoImpl_plato_paciente(void *self)
{
   obj_plato_paciente* o;
   obj_plato *p;
   o = (obj_plato_paciente*) self;   
   p = plato_new();
   p->findbykey(p,o->cod_plato);     
   return p;
}
//----------------------------------------------------
void *init_plato_paciente(void *self, data_set *ds)
{
  obj_plato_paciente *obj;
  obj = (obj_plato_paciente *)self;
  obj->ds = ds;
  obj->findbykey = find_plato_pacienteImpl;
  obj->findAll = findAll_plato_pacienteImpl;
  obj->saveObj = saveObj_plato_pacienteImpl;  
  // inicializar relaciones con otros objetos del modelo
  obj->get_plato = get_platoImpl_plato_paciente;
  obj->get_paciente = get_pacienteImpl_plato_paciente;
  return obj;
}
//----------------------------------------------------
obj_plato_paciente *plato_paciente_new()
{
  return (obj_plato_paciente *)init_obj(t_plato_paciente, col_plato_paciente, init_plato_paciente);
}
