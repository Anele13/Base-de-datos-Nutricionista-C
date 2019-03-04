#include <string.h>
#include "config.h"
#include "utils.h"
extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillPacienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_paciente *dspaciente;
    dspaciente = &(( (t_data_set_paciente *) *rw)[rowi]);
    dspaciente->dni=atoi(PQgetvalue(res,rowi,0));
    strcpy( dspaciente->nombre ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dspaciente->apellido ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dspaciente->fecha_alta ,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dspaciente->domicilio ,rtrim(PQgetvalue(res,rowi,4),' '));
    strcpy( dspaciente->telefono ,rtrim(PQgetvalue(res,rowi,5),' '));
    strcpy( dspaciente->fecha_nac ,rtrim(PQgetvalue(res,rowi,6),' '));
    dspaciente->peso_inicial=atof(PQgetvalue(res,rowi,7));
    dspaciente->talla=atof(PQgetvalue(res,rowi,8));
}       
//----------------------------------------------------
void fillObjPaciente(obj_paciente *p,  t_data_set_paciente rwitm)
{  
      p->dni = rwitm.dni;
      strcpy( p->nombre,rwitm.nombre);
      strcpy( p->apellido,rwitm.apellido);
      strcpy( p->fecha_alta,rwitm.fecha_alta);
      strcpy( p->domicilio,rwitm.domicilio);
      strcpy( p->telefono,rwitm.telefono);
      strcpy( p->fecha_nac,rwitm.fecha_nac);
      p->peso_inicial = rwitm.peso_inicial;
      p->talla = rwitm.talla;      
}
//----------------------------------------------------
void fillRowsPaciente(void **list, int size,void *data)
{
     int i;
     obj_paciente *d;
    *list = (obj_paciente **)malloc(sizeof(obj_paciente*)* size);
    for(i=0;i<size;++i)
    {
      d = paciente_new();
      fillObjPaciente(d,((t_data_set_paciente *)data)[i]);
      ((obj_paciente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_paciente(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_paciente,fillPacienteFromDB);
}
//----------------------------------------------------
void fill_dataset_paciente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_paciente]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
      ((t_data_set_paciente *)ds->rows)[i].dni =((t_data_set_paciente *)data)[i].dni;
      strcpy( ((t_data_set_paciente *)ds->rows)[i].nombre,((t_data_set_paciente *)data)[i].nombre);
      strcpy( ((t_data_set_paciente *)ds->rows)[i].apellido,((t_data_set_paciente *)data)[i].apellido);
      strcpy( ((t_data_set_paciente *)ds->rows)[i].fecha_alta,((t_data_set_paciente *)data)[i].fecha_alta);
      strcpy( ((t_data_set_paciente *)ds->rows)[i].domicilio,((t_data_set_paciente *)data)[i].domicilio);
      strcpy( ((t_data_set_paciente *)ds->rows)[i].telefono,((t_data_set_paciente *)data)[i].telefono);
      strcpy( ((t_data_set_paciente *)ds->rows)[i].fecha_nac,((t_data_set_paciente *)data)[i].fecha_nac);
      ((t_data_set_paciente *)ds->rows)[i].peso_inicial =((t_data_set_paciente *)data)[i].peso_inicial;
      ((t_data_set_paciente *)ds->rows)[i].talla =((t_data_set_paciente *)data)[i].talla;
     }
}
//----------------------------------------------------
int findAll_pacienteImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria,t_paciente, ((data_set_paciente*)((obj_paciente*)self)->ds)->rows,fillPacienteFromDB,fillRowsPaciente);
}
//----------------------------------------------------
// implementacion de metodos para paciente
int find_pacienteImpl(void *self, int k)
{
  int size=0;
  void *data;  
  obj_paciente *p;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, "dni = %d",k);
   sql =getFindSQL(t_paciente, where);
 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_paciente*)((obj_paciente*)self)->ds)->rows;
 size = exec_get_paciente(sql,&data);
 fill_dataset_paciente(((obj_paciente*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 {
   p = (obj_paciente*)self;
   fillObjPaciente(p,((t_data_set_paciente *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_pacienteImpl(void *self, int dni,char *nombre,char *apellido,char *domicilio,char *telefono,char *fecha_nac,float peso_inicial,float talla,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  char *fecha_alta;
  obj_paciente *o;
  
  data = ((data_set_paciente*)((obj_paciente*)self)->ds)->rows;
  if(isNew)
  {// insert
    sprintf(values,sql_insert_param_str[t_paciente] , dni, nombre, apellido, domicilio,telefono, fecha_nac, peso_inicial, talla);
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_paciente],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
    // obtener fecha actual
    fecha_alta = getFecha();
  }
  else
  {// update
      o = (obj_paciente *)self;
      strcpy(fecha_alta,o->fecha_alta);
      sprintf(where,"dni =%d ",o->dni);
      sprintf(values, sql_update_param_str[t_paciente] , nombre, apellido, domicilio, telefono, fecha_nac,peso_inicial,talla);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_paciente],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
      
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_paciente*)self)->dni = dni ;
       strcpy(((obj_paciente*)self)->nombre,nombre);
       strcpy(((obj_paciente*)self)->apellido,apellido);
       strcpy(((obj_paciente*)self)->fecha_alta,fecha_alta);
       strcpy(((obj_paciente*)self)->domicilio,domicilio);
       strcpy(((obj_paciente*)self)->telefono,telefono);
       strcpy(((obj_paciente*)self)->fecha_nac,fecha_nac);
       ((obj_paciente*)self)->peso_inicial= peso_inicial;
       ((obj_paciente*)self)->talla= talla;
       return 1;    
    }   
}
//----------------------------------------------------
void *init_paciente(void *self, data_set *ds)
{
  obj_paciente *obj;
  obj = (obj_paciente *)self;
  obj->ds = ds;
  obj->findbykey = find_pacienteImpl;
  obj->findAll = findAll_pacienteImpl;
  obj->saveObj = saveObj_pacienteImpl;  
  return obj;
}
//----------------------------------------------------
obj_paciente *paciente_new()
{   
 return (obj_paciente *)init_obj(t_paciente, col_paciente, init_paciente);
}
