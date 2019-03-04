#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPacienteControlFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_paciente_control *dspaciente_control;
    dspaciente_control =  &(( (t_data_set_paciente_control *) *rw)[rowi]);
	dspaciente_control->dni_paciente =atoi(PQgetvalue(res,rowi,0));
	strcpy( dspaciente_control->fecha ,rtrim(PQgetvalue(res,rowi,1),' '));
	dspaciente_control->peso =atof(PQgetvalue(res,rowi,2));	
}       
//----------------------------------------------------
void fillObjPacienteControl(obj_paciente_control *p,  t_data_set_paciente_control rwitm)
{  
	  p->dni_paciente = rwitm.dni_paciente;
	  strcpy(p->fecha ,  rwitm.fecha);
	  p->peso = rwitm.peso;	  
}
//----------------------------------------------------
void fillRowsPacienteControl(void **list, int size,void *data)
{
     int i;
     obj_paciente_control *d;
    *list = (obj_paciente_control **)malloc(sizeof(obj_paciente_control*)* size);
    for(i=0;i<size;++i)
    {
      d = paciente_control_new();
      fillObjPacienteControl(d,((t_data_set_paciente_control *)data)[i]);
      ((obj_paciente_control **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_paciente_control(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_paciente_control,fillPacienteControlFromDB);
}
//----------------------------------------------------
void fill_dataset_paciente_control(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_paciente_control]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
	 ((t_data_set_paciente_control *)ds->rows)[i].dni_paciente =((t_data_set_paciente_control *)data)[i].dni_paciente;	 
	 strcpy( ((t_data_set_paciente_control *)ds->rows)[i].fecha,((t_data_set_paciente_control *)data)[i].fecha);
	 ((t_data_set_paciente_control *)ds->rows)[i].peso =((t_data_set_paciente_control *)data)[i].peso;
     }
}
//----------------------------------------------------
int findAll_paciente_controlImpl(void *self,void **list, char *criteria)
{
   return findAllImpl(self,list, criteria,t_paciente_control, ((data_set_paciente_control*)((obj_paciente_control*)self)->ds)->rows,fillPacienteControlFromDB,fillRowsPacienteControl);
}
//----------------------------------------------------
// implementacion de metodos para paciente_control
int find_paciente_controlImpl(void *self, int dnipac,char *fecha)
{
  int size=0;
  void *data;
  obj_paciente_control *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " dni_paciente= %d and fecha ='%s'",dnipac,fecha);
   sql =getFindSQL(t_paciente_control, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_paciente_control*)((obj_paciente_control*)self)->ds)->rows;
 size = exec_get_paciente_control(sql,&data);
 fill_dataset_paciente_control(((obj_paciente_control*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_paciente_control*)self;
   fillObjPacienteControl(d,((t_data_set_paciente_control *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_paciente_controlImpl(void *self, int dni_paciente, char * fecha,float peso,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  data = ((data_set_paciente_control*)((obj_paciente_control*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_paciente_control] ,  dni_paciente,fecha, peso);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_paciente_control],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);    
  }
  else
  {// update
      sprintf(where,"dni_paciente=%d and fecha = '%s'",dni_paciente,fecha);
      sprintf(values, sql_update_param_str[t_paciente_control] , peso);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_paciente_control],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
	   ((obj_paciente_control*)self)->dni_paciente = dni_paciente ;
	   ((obj_paciente_control*)self)->peso = peso ;
	   strcpy(((obj_paciente_control*)self)->fecha , fecha );
       return 1;    
    }   
}
//----------------------------------------------------
obj_paciente *get_pacienteImpl_paciente_control(void *self)
{
   obj_paciente_control* o;
   obj_paciente *p;
   o = (obj_paciente_control*) self;   
   p = paciente_new();
   p->findbykey(p,o->dni_paciente);     
   return p;
}
//----------------------------------------------------
void *init_paciente_control(void *self, data_set *ds)
{
  obj_paciente_control *obj;
  obj = (obj_paciente_control *)self;
  obj->ds = ds;
  obj->findbykey = find_paciente_controlImpl;
  obj->findAll = findAll_paciente_controlImpl;
  obj->saveObj = saveObj_paciente_controlImpl;  
  obj->get_paciente = get_pacienteImpl_paciente_control;
  return obj;
}
//----------------------------------------------------
obj_paciente_control *paciente_control_new()
{
  return (obj_paciente_control *)init_obj(t_paciente_control, col_paciente_control, init_paciente_control);
}
