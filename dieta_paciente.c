#include <string.h>
#include "config.h"
#include "utils.h"

extern PGconn *conn; //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
void fillDietaPacienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_data_set_dieta_paciente *dsdietapaciente;
    dsdietapaciente =  &(( (t_data_set_dieta_paciente *) *rw)[rowi]);
    dsdietapaciente->codigo=atoi(PQgetvalue(res,rowi,0));
    dsdietapaciente->cod_dieta=atoi(PQgetvalue(res,rowi,1));
    dsdietapaciente->dni_paciente=atoi(PQgetvalue(res,rowi,2));
    strcpy( dsdietapaciente->fecha ,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dsdietapaciente->fecha_fin ,rtrim(PQgetvalue(res,rowi,4),' '));
}
//----------------------------------------------------
void fillObjDietaPaciente (obj_dieta_paciente *p,  t_data_set_dieta_paciente rwitm)
{  
      p->codigo = rwitm.codigo;
      p->cod_dieta = rwitm.cod_dieta;
      p->dni_paciente = rwitm.dni_paciente;
      strcpy( p->fecha,rwitm.fecha);
      strcpy( p->fecha_fin,rwitm.fecha_fin);
}
//----------------------------------------------------
void fillRowsDietaPaciente(void **list, int size,void *data)
{
     int i;
     obj_dieta_paciente *d;
    *list = (obj_dieta_paciente **)malloc(sizeof(obj_dieta_paciente*)* size);
    for(i=0;i<size;++i)
    {
      d = dieta_paciente_new();
      fillObjDietaPaciente(d,((t_data_set_dieta_paciente *)data)[i]);
      ((obj_dieta_paciente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
int exec_get_dieta_paciente(char *sql,void **rw)
{
    return exec_get_fromDB(sql,rw,t_dieta,fillDietaPacienteFromDB);
}
//----------------------------------------------------
void fill_dataset_dieta_paciente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(size_st[t_dieta_paciente]* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     { 
      ((t_data_set_dieta_paciente *)ds->rows)[i].codigo =((t_data_set_dieta_paciente *)data)[i].codigo;
      ((t_data_set_dieta_paciente *)ds->rows)[i].cod_dieta =((t_data_set_dieta_paciente *)data)[i].cod_dieta;
      ((t_data_set_dieta_paciente *)ds->rows)[i].dni_paciente =((t_data_set_dieta_paciente *)data)[i].dni_paciente;
      strcpy( ((t_data_set_dieta_paciente *)ds->rows)[i].fecha,((t_data_set_dieta_paciente *)data)[i].fecha);
      strcpy( ((t_data_set_dieta_paciente *)ds->rows)[i].fecha_fin,((t_data_set_dieta_paciente *)data)[i].fecha_fin);
     }
}
//----------------------------------------------------
int findAll_dieta_pacienteImpl(void *self,void **list, char *criteria)
{
    return findAllImpl(self,list, criteria,t_dieta_paciente, ((data_set_dieta_paciente*)((obj_dieta_paciente*)self)->ds)->rows,fillDietaPacienteFromDB,fillRowsDietaPaciente);
}
//----------------------------------------------------
// implementacion de metodos para dieta_paciente
int find_dieta_pacienteImpl(void *self, int k)
{
  int size=0;
  void *data;
  obj_dieta_paciente *d;
  char *where;
  char *sql, *str_where=NULL;
   where = (char*)malloc(sizeof(char)*MAX_SQL);
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( where, MAX_SQL, " codigo = %d",k);
   sql =getFindSQL(t_paciente, where);

 //ejecutar consulta sql de seleccion, con criterio where
 data = ((data_set_dieta_paciente*)((obj_dieta_paciente*)self)->ds)->rows;
 size = exec_get_dieta_paciente(sql,&data);
 fill_dataset_dieta_paciente(((obj_dieta_paciente*)self)->ds,data,size);
 // setear datos a la instancia....
 if(size>0)
 { 
   d = (obj_dieta_paciente*)self;
   fillObjDietaPaciente(d,((t_data_set_dieta_paciente *)data)[0]);
 }
 else
   size = -1;   
 return size;
}
//----------------------------------------------------
int saveObj_dieta_pacienteImpl(void *self,  int cod_dieta, int dni_paciente, char *fecha, char *fecha_fin,int isNew)
{
  PGresult *res; 
  int code=0;
  char values[MAX_WHERE_SQL];  
  char where[MAX_WHERE_SQL];  
  char *sql;
  void *data;
  obj_dieta_paciente *o;
  int codigo=0;
  int fecha_nula=false;
  data = ((data_set_dieta_paciente*)((obj_dieta_paciente*)self)->ds)->rows;
  if(isNew)
  {// insert
    if(fecha_fin==NULL)
      {        
        fecha_nula=true;
      }
    else   
      if(fecha_fin=="")
       {
         fecha_nula=true;
       }  
    sprintf(values,sql_insert_param_str[t_dieta_paciente] , cod_dieta, dni_paciente, fecha, (fecha_nula?"null":fecha_fin));
    sql = (char*)malloc(sizeof(char)*MAX_SQL);
    snprintf( sql, MAX_SQL, sql_insert_str[t_dieta_paciente],values);    
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);   
     // obtener ultimo codigo autogenerado
    res = PQexec(conn, "select max(codigo) codigo from dieta;");
    codigo = atoi(PQgetvalue(res,0,0));
    PQclear(res); 
  }
  else
  {// update
      o = (obj_dieta_paciente *)self;
      codigo = o->codigo;
      sprintf(where,"codigo =%d ",codigo);
      sprintf(values, sql_update_param_str[t_dieta_paciente] , fecha, fecha_fin);
      sql = (char*)malloc(sizeof(char)*MAX_SQL);
      snprintf( sql, MAX_SQL, sql_update_str[t_dieta_paciente],values,where);
      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)       
       return 0;
    else
    {
       ((obj_dieta_paciente*)self)->codigo = codigo ;
       ((obj_dieta_paciente*)self)->cod_dieta = cod_dieta;
       ((obj_dieta_paciente*)self)->dni_paciente = dni_paciente;
       strcpy(((obj_dieta_paciente*)self)->fecha,fecha);
       strcpy(((obj_dieta_paciente*)self)->fecha_fin,fecha_fin);
       return 1;    
    }   
}
//----------------------------------------------------
obj_dieta *get_dietaImpl_dieta_paciente(void *self)
{
   obj_dieta_paciente* o;
   obj_dieta *p;
   o = (obj_dieta_paciente*) self;   
   p = dieta_new();
   p->findbykey(p,o->cod_dieta);     
   return p;
}
//----------------------------------------------------
obj_paciente *get_pacienteImpl_dieta_paciente(void *self)
{
   obj_dieta_paciente* o;
   obj_paciente *p;
   o = (obj_dieta_paciente*) self;   
   p = paciente_new();
   p->findbykey(p,o->dni_paciente);     
   return p;
}
//----------------------------------------------------
void *init_dieta_paciente(void *self, data_set *ds)
{
  obj_dieta_paciente *obj;
  obj = (obj_dieta_paciente *)self;
  obj->ds = ds;
  obj->findbykey = find_dieta_pacienteImpl;
  obj->findAll = findAll_dieta_pacienteImpl;
  obj->saveObj = saveObj_dieta_pacienteImpl;
  // inicializar relaciones con otros objetos del modelo
  obj->get_dieta = get_dietaImpl_dieta_paciente;
  obj->get_paciente = get_pacienteImpl_dieta_paciente;
  return obj;
}
//----------------------------------------------------
obj_dieta_paciente *dieta_paciente_new()
{
  return (obj_dieta_paciente *)init_obj(t_dieta_paciente, col_dieta_paciente, init_dieta_paciente);
}
