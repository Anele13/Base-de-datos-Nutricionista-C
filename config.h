#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#ifndef __DEFOPTS__
#define __DEFOPTS__
enum t_bool{ false=0,true=1};

#define MAX 50
#define MAX1 15
#define MAX2 30
#define MAX3 90
#define MAX4 2
#define MAX5 150
#define MAXOBS 200
#define CANT_TABLES 11
#define MAX_SQL 800
#define MAX_WHERE_SQL 600

typedef enum {
t_paciente=0, 
t_dieta,
t_dieta_paciente,
t_plato, 
t_plato_dieta, 
t_ingrediente, 
t_plato_ingrediente, 
t_plato_paciente , 
t_paciente_control, 
t_profesional, 
t_paciente_profesional }tipos;
//----------------------------------------------------
// estructura para paciente
typedef struct {
		int dni;
        char nombre[MAX];
        char apellido[MAX];
        char domicilio[MAX];
		char fecha_alta[MAX1];
		char fecha_nac[MAX1];
		float peso_inicial;
		float talla;
		char telefono[MAX];
        } t_data_set_paciente;
//----------------------------------------------------
// estructura para dieta
typedef struct {
		int codigo; 
        char nombre[MAX];
        char autor[MAX];
        char fecha_alta[MAX1];
        char descripcion[MAX5];
        } t_data_set_dieta;
//----------------------------------------------------
// estructura para dieta paciente
typedef struct {  
          int codigo;
          int cod_dieta;   
          int dni_paciente;   
          char fecha[MAX1];
          char fecha_fin[MAX1];
        } t_data_set_dieta_paciente;
//----------------------------------------------------  
// estructura para plato
typedef struct {
		int codigo; 
        char nombre[MAX];
        char detalle[MAXOBS];
        } t_data_set_plato;
//----------------------------------------------------  
// estructura para plato dieta
typedef struct {  
          int cod_plato;
          int cod_dieta;   
          float porcion;
        } t_data_set_plato_dieta;
//----------------------------------------------------
// estructura para ingredientes
typedef struct {
		int codigo; 
        char nombre[MAX];
        char unidadmed[MAX];
        } t_data_set_ingrediente;
//----------------------------------------------------  
// estructura para ingredientes de plato
typedef struct {  
          int cod_plato;
          int cod_ingrediente;   
          float cantidad;
        } t_data_set_plato_ingrediente;
//----------------------------------------------------
// estructura para platos de un paciente
typedef struct {  
          int cod_plato;
          int dni_paciente;   
          float cantidad;
          char fecha[MAX2];
        } t_data_set_plato_paciente;
//----------------------------------------------------
// estructura para control de paciente
typedef struct {  
          int dni_paciente;   
          char fecha[MAX2];
          float peso;
        } t_data_set_paciente_control;
//----------------------------------------------------
// estructura para profesional
typedef struct {  
          int dni;   
          char matricula[MAX1];
          char nombre[MAX];
          char apellido[MAX];
          char telefono[MAX];
          char fecha_alta[MAX2];
          int es_med;
          int es_nutri;
        } t_data_set_profesional;
//----------------------------------------------------  
// estructura para profesional que atiende a paciente
typedef struct {  
          int dni_paciente;
          int dni_medico;   
          char fecha_desde[MAX2];
          char fecha_hasta[MAX2];
        } t_data_set_paciente_profesional;
//----------------------------------------------------
//  COLECCION DE COLUMNAS DE LOS OBJETOS
const char *col_paciente[]={"dni","nombre","apellido","fecha_alta","domicilio","telefono","fecha_nac","peso_inicial","talla"};
const char *col_dieta[]={"codigo","nombre","autor","fecha_alta","descripcion"};
const char *col_dieta_paciente[]={"codigo","cod_dieta","dni_paciente","fecha","fecha_fin"};
const char *col_plato[]={"codigo","nombre","detalle"};
const char *col_plato_dieta[]={"cod_plato","cod_dieta","porcion"};
const char *col_ingrediente[]={"codigo","nombre","uni_med"};
const char *col_plato_ingrediente[]={"cod_plato","cod_ingrediente","cantidad"};
const char *col_plato_paciente[]={"cod_plato","dni_paciente","cantidad","fecha"};
const char *col_paciente_control[]={"dni_paciente","fecha","peso"};
const char *col_profesional[]={"dni","matricula","nombre","apellido","telefono","fecha_alta","es_mes","es_nutri"};
const char *col_paciente_profesional[]={"dni_paciente","dni_medico","fecha_desde","fecha_hasta"};
//----------------------------------------------------
// instrucciones sql para interaccionar con la base de datos
// seleccion
char *sql_select_str[CANT_TABLES]={
     "select dni,nombre,apellido,fecha_alta,domicilio,telefono,fecha_nac,peso_inicial,talla from paciente %s;",
     "select codigo,nombre,autor,fecha_alta,descripcion from dieta %s",
     "select codigo,cod_dieta,dni_paciente,fecha,fecha_fin from dieta_paciente %s",
     "select codigo,nombre,detalle from platos %s",
     "select cod_plato, cod_dieta,porcion from plato_dieta %s",
     "select codigo,nombre,uni_med from ingredientes %s",
     "select cod_plato, cod_ingrediente, cantidad from plato_ingrediente %s",
     "select cod_plato, dni_paciente,cantidad, to_char(fecha, 'YYYY-MM-DD HH24:MI') fecha from plato_paciente %s",
     "select dni_paciente, to_char(fecha, 'YYYY-MM-DD HH24:MI')fecha,peso from paciente_control %s",
     "select dni,matricula,nombre,apellido,telefono, to_char(fecha_alta, 'YYYY-MM-DD HH24:MI')fecha_alta,es_med,es_nutri from profesional %s;",
     "select dni_paciente, dni_medico,to_char(fecha_desde, 'YYYY-MM-DD HH24:MI') fecha_desde,fecha_hasta from paciente_profesional %s"};
//----------------------------------------------------
// parametros de insercion
char *sql_insert_param_str[CANT_TABLES]={
     "%d,'%s','%s',date_trunc('second', now()),'%s','%s','%s',%.2f,%.2f",   // insert paciente
     "'%s','%s',date_trunc('second', now()),'%s'",  //insert dieta
     "%d,%d,'%s','%s'", // insert dieta paciente
     "'%s','%s'", // insert plato
     "%d,%d,%.2f", // insert plato dieta
     "'%s','%s'", // insert ingredientes
     "%d,%d,%.2f", // insert plato ingrediente
     "%d,%d,%.2f,date_trunc('second', now())", // insert plato paciente
     "%d,'%s',%.2f", //insert paciente_control
     "%d,'%s','%s','%s','%s',date_trunc('second', now()),%d,%d", // insert profesional
     "%d,%d,date_trunc('second', now())"}; // insert paciente_profesional
//----------------------------------------------------
// parametros de actualizacion
char *sql_update_param_str[CANT_TABLES]={
     "nombre ='%s', apellido ='%s', domicilio='%s', telefono='%s', fecha_nac='%s', peso_inicial = %.2f, talla = %.2f",
     "nombre ='%s', autor ='%s', descripcion='%s'",
     "fecha='%s',fecha_fin='%s'",
     "nombre ='%s', detalle='%s'",
     "porcion=%.2f",
     "nombre ='%s', uni_med='%s'",
     "cantidad=%.2f",
     "cantidad=%.2f",
     "peso =%.2f",
     "matricula='%s',nombre='%s',apellido='%s',telefono='%s',es_med=%d,es_nutri=%d",
     "fecha_hasta='%s'"};
//----------------------------------------------------
// insercion
char *sql_insert_str[CANT_TABLES]={
     "insert into paciente(dni,nombre,apellido,fecha_alta,domicilio,telefono,fecha_nac,peso_inicial,talla)values(%s);",
     "insert into dieta(nombre,autor,fecha_alta,descripcion)values(%s);",
     "insert into dieta_paciente(cod_dieta,dni_paciente,fecha,fecha_fin)values(%s);",
     "insert into platos(nombre,detalle)values(%s);",
     "insert into plato_dieta(cod_plato,cod_dieta,porcion)values(%s);",
     "insert into ingredientes(nombre,uni_med)values(%s);",
     "insert into plato_ingrediente(cod_plato,cod_ingrediente,cantidad)values(%s);",
     "insert into plato_paciente(cod_plato,dni_paciente,cantidad,fecha)values(%s);",
     "insert into paciente_control(dni_paciente,fecha,peso)values(%s);",
     "insert into profesional(dni,matricula,nombre,apellido,telefono,fecha_alta,es_med,es_nutri)values(%s);",
     "insert into paciente_profesional(dni_paciente,dni_medico,fecha_desde)values(%s);"};
//----------------------------------------------------
// actualizacion
char *sql_update_str[CANT_TABLES]={
     "update paciente set %s where %s;",
     "update dieta set %s where %s;",
     "update dieta_paciente set %s where %s;",
     "update platos set %s where %s;",
     "update plato_dieta set %s where %s;",
     "update ingredientes set %s where %s;",
     "update plato_ingrediente set %s where %s;",
     "update plato_paciente set %s where %s;",
     "update paciente_control set %s where %s;",
     "update profesional set %s where %s;",
     "update paciente_profesional set %s where %s;"};
//----------------------------------------------------
// definicion de tamaños para comparaciones asignacion de memoria dinamica.
int size_st[CANT_TABLES]={sizeof(t_data_set_paciente),sizeof(t_data_set_dieta),sizeof(t_data_set_dieta_paciente),sizeof(t_data_set_plato),sizeof(t_data_set_plato_dieta), sizeof(t_data_set_ingrediente), sizeof(t_data_set_plato_ingrediente),sizeof(t_data_set_plato_paciente),sizeof(t_data_set_paciente_control),sizeof(t_data_set_profesional),sizeof(t_data_set_paciente_profesional)};
int size_col[CANT_TABLES]={sizeof(col_paciente),sizeof(col_dieta),sizeof(col_dieta_paciente),sizeof(col_plato),sizeof(col_plato_dieta), sizeof(col_ingrediente), sizeof(col_plato_ingrediente),sizeof(col_plato_paciente), sizeof(col_paciente_control),sizeof(col_profesional),sizeof(col_paciente_profesional)};
int cnt_cols[CANT_TABLES]={9,5,5,3,3,3,3,4,3,8,4};
//----------------------------------------------------
// dataset generico
typedef struct { char **columns;int cntCol;int cntRows; void *rows;} data_set;
// dataset tipado- especifico
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_paciente *rows;  } data_set_paciente;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_dieta *rows;  } data_set_dieta;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_dieta *rows;  } data_set_dieta_paciente;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_plato *rows;  } data_set_plato;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_plato_dieta *rows;  } data_set_plato_dieta;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_ingrediente *rows;  } data_set_ingrediente;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_plato_ingrediente *rows;  } data_set_plato_ingrediente;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_plato_paciente *rows;  } data_set_plato_paciente;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_paciente_control *rows;  } data_set_paciente_control;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_profesional *rows;  } data_set_profesional;
typedef struct { char **columns;int cntCol;int cntRows; t_data_set_paciente_profesional *rows;  } data_set_paciente_profesional;
//----------------------------------------------------
// IMPLEMENTACION DE LOS OBJETOS EN STRUCT CON "METODOS"
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int dni);
 int (*saveObj)(void *self,int dni,char *nombre,char *apellido,char *domicilio,char *telefono,char *fecha_nac,float peso_inicial,float talla,int isNew); 
/*implementacion de relaciones*/
 int dni;
 char nombre[MAX]; 
 char apellido[MAX]; 
 char fecha_alta[MAX1]; 
 char domicilio[MAX]; 
 char telefono[MAX]; 
 char fecha_nac[MAX1];
 float peso_inicial;
 float talla;
}obj_paciente;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int dni);
 int (*saveObj)(void *self, char *nombre, char *autor, char *descripcion,int isNew); 
  int codigo; 
  char nombre[MAX];
  char autor[MAX];
  char fecha_alta[MAX1];
  char descripcion[MAX5];
}obj_dieta;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int codigo);
 int (*saveObj)(void *self, char *nombre, char *detalle,int isNew); 
 int codigo; 
 char nombre[MAX];
 char detalle[MAXOBS];
}obj_plato;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int cod_plato, int cod_dieta);
 int (*saveObj)(void *self,	int cod_plato, int cod_dieta, float porcion,int isNew); 
/*implementacion de relaciones*/
  obj_dieta *(*get_dieta)(void *self); 
  obj_plato *(*get_plato)(void *self);
  int cod_plato;
  int cod_dieta;   
  float porcion;
}obj_plato_dieta;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int codigo);
 int (*saveObj)(void *self,	int cod_dieta, int dni_paciente, char *fecha, char *fecha_fin,int isNew); 
/*implementacion de relaciones*/
  obj_dieta *(*get_dieta)(void *self); 
  obj_paciente *(*get_paciente)(void *self); 
  int codigo;
  int cod_dieta;   
  int dni_paciente;   
  char fecha[MAX1];
  char fecha_fin[MAX1];
}obj_dieta_paciente;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int codigo);
 int (*saveObj)(void *self, char *nombre, char *unidadmed,int isNew); 
	int codigo; 
    char nombre[MAX];
    char unidadmed[MAX];
}obj_ingrediente;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int cod_plato, int cod_ingrediente);
 int (*saveObj)(void *self,	int cod_plato, int cod_ingrediente, float cantidad, int isNew); 
/*implementacion de relaciones*/
  obj_plato *(*get_plato)(void *self); 
  obj_ingrediente *(*get_ingrediente)(void *self); 
	int cod_plato; 
	int cod_ingrediente; 
    float cantidad;
}obj_plato_ingrediente;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int cod_plato, int dni_paciente, char *fecha);
 int (*saveObj)(void *self,	int cod_plato, int dni_paciente, float cantidad, int isNew); 
/*implementacion de relaciones*/
  obj_plato *(*get_plato)(void *self);
  obj_paciente *(*get_paciente)(void *self);
  int cod_plato;
  int dni_paciente;   
  float cantidad;
  char fecha[MAX2];
}obj_plato_paciente;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int dni_paciente, char *fecha);
 int (*saveObj)(void *self,	int dni_paciente, char *fecha, float peso, int isNew); 
/*implementacion de relaciones*/
  obj_paciente *(*get_paciente)(void *self);
  int dni_paciente;   
  char fecha[MAX2];
  float peso;
}obj_paciente_control;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int dni);
 int (*saveObj)(void *self,int dni, char *matricula, char *nombre, char *apellido, char *telefono,int es_med, int es_nutri,int isNew); 
/*implementacion de relaciones*/
  int dni;   
  char matricula[MAX1];
  char nombre[MAX];
  char apellido[MAX];
  char telefono[MAX];
  char fecha_alta[MAX2];
  int es_med;
  int es_nutri;
}obj_profesional;
//----------------------------------------------------
typedef struct
{
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria); 
 int (*findbykey)(void *self,int dni_paciente,int dni_medico, char *fecha_desde);
 int (*saveObj)(void *self,int dni_paciente,int dni_medico,char *fecha_hasta,int isNew); 
/*implementacion de relaciones*/
  obj_paciente *(*get_paciente)(void *self);
  obj_profesional *(*get_profesional)(void *self);
  //-----------------------
  int dni_paciente;
  int dni_medico;   
  char fecha_desde[MAX2];
  char fecha_hasta[MAX2];
}obj_paciente_profesional;
//----------------------------------------------------
typedef struct 
{ 
 data_set *ds;
 int (*findAll)(void *self, void **list,char *criteria);
   
}t_object;
//----------------------------------------------------
// constructores
obj_paciente *paciente_new();
obj_dieta *dieta_new();
obj_dieta_paciente *dieta_paciente_new();
obj_plato *plato_new();
obj_plato_dieta *plato_dieta_new();
obj_ingrediente *ingrediente_new();
obj_plato_ingrediente *plato_ingrediente_new();
obj_plato_paciente *plato_paciente_new();
obj_paciente_control *paciente_control_new();
obj_profesional *profesional_new();
obj_paciente_profesional *paciente_profesional_new();

// IMPLEMENTACION PARA DAR SOPORTE A COMPORTAMIENTO GENERICO - ABSTRACT.
// Implementacion en  "orm.c"
extern int exec_get_fromDB(char *sql,void **rw, tipos t, void(*fcFill)(void **,int ri,PGresult *r));
extern int findAllImpl(void *self,void **list, char *criteria,tipos t, void *data,void(*fcFill)(void **,int ri,PGresult *r),void(*fillRows)(void **list,int size,void *data));
extern char *getFindSQL(tipos t, char *where);
extern void *init_obj(tipos t, const char *col[], void*(*init)(void *o,data_set *ds));
// Implementacion en "utils.c"
extern char *getFecha();
extern char *getFechaHora();
#endif
