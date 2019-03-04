#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#include <string.h>
#include "orm.c"
#include "paciente.c"
#include "dieta.c"
#include "dieta_paciente.c"
#include "plato.c"
#include "plato_dieta.c"
#include "ingrediente.c"
#include "plato_ingrediente.c"
#include "plato_paciente.c"
#include "paciente_control.c"
#include "profesional.c"
#include "paciente_profesional.c"

#include "config.h"

PGconn *conn; 

typedef struct{
     char *nombre;
     int   anexo; 
     FILE *archivo;
     int codigo;  
}t_info;

typedef struct{
     t_info parametro[14];  
}t_comandos;


void pedirEntero(char *mensaje, int *numero){
    fflush(stdin);
    printf("%s\n", mensaje);
    scanf("%d",numero);
    fflush(stdin);    
}

void pedirDouble (char *mensaje, float *numero){
    fflush(stdin);
    printf("%s\n", mensaje);
    scanf("%f",numero);
    fflush(stdin); 
}

char * pedirDatos( char* mensaje, int max){
      char * cadena = (char*)malloc(sizeof(char)* MAX1);
      int longitud;
      fflush(stdin);
      printf("%s \n", mensaje);
      fgets(cadena, max, stdin);
      longitud=strlen(cadena);
      cadena[longitud-1]= '\0';
      fflush(stdin);
      return cadena;  
}


int confirma(char * mensaje){
    fflush(stdin);
    while(true){
        printf("%s [S/N] \n", mensaje);
        switch(getchar()){
            case 'S': return true;
            case 's': return true; 
            case 'N': return false;  
            case 'n': return false;  
            default : printf("Respuesta incorrecta. \n"); 
                      fflush( stdin );
                      break;
        }
    }
}

int validarProfesion(char * mensaje){
    fflush(stdin);
    while(true){
        printf("%s [S/N] \n", mensaje);
        switch(getchar()){
            case 'S': return true;
            case 's': return true; 
            case 'N': return false;  
            case 'n': return false;  
            default : printf("Respuesta incorrecta. \n"); 
                      fflush( stdin );
                      break;
        }
    }
}

/*int abrir_archivo(FILE **archivo, char *nombre){
    
	*archivo = fopen ( nombre, "w" );        
	if (*archivo==NULL) {fputs ("ERROR DE ARCHIVO",stderr); exit (1);}
}


void escribir_archivo(FILE *archivo, char *msj){
		if(fprintf(archivo, msj) == -1)
			printf("Error al escribir en el archivo");
}

*/

//BIEN. no presenta errores
void agregarPaciente(){

  obj_paciente *paciente = paciente_new(); 
  char *nombre, *apellido,*domicilio,*telefono, *fecha_nac;
  float peso_inicial, talla;
  int dni;

  do{
        printf("respuesta %f\n", peso_inicial);
        pedirEntero("Ingrese el dni del paciente a registrar", &dni);
        if(paciente->findbykey(paciente,dni)==-1){
              nombre= pedirDatos("Ingrese el nombre del paciente a registrar", MAX1);
              apellido= pedirDatos("Ingrese el apellido del paciente a registrar", MAX1);
              domicilio= pedirDatos("Ingrese el domicilio", MAX1);
              telefono= pedirDatos("Ingrese el telefono", MAX1);
              fecha_nac= pedirDatos("Ingrese la fecha de nacimiento: anio-mes-dia",MAX1);
              pedirDouble("Ingrese el peso inicial del paciente", &peso_inicial);
              pedirDouble("Ingrese el la talla del paciente ", &talla);
              
              //printf("ingresadas dni:%d, nombre:%s, apell:%s, domicilio:%s, telefono:%s, fehca1:%s, peso:%f, talla%f \n",dni,nombre,apellido,domicilio, telefono, fecha_nac, peso_inicial,talla);
              if(paciente->saveObj(paciente,dni,nombre,apellido,domicilio, telefono, fecha_nac, peso_inicial,talla,true))
                 printf("Paciente agregado existosamente \n");
              else
                  printf("ERROR en la BD. ¿¿Ingreso una enie?? \n"); //no ingresar ñ. da error!
    }
    else{
         printf("ERROR. El paciente ya esta cargado.\n");
    } 
  }while(confirma("Desea registrar otro paciente?"));
}


//BIEN. no tiene errores
void agregarPacienteControl(){
     
     obj_paciente *paciente = paciente_new();
     obj_paciente_control *pc = paciente_control_new();
     char* fecha;
     int dni;
     float peso;     
     
     do{
        pedirEntero("Ingrese el dni del paciente", &dni);
        if(paciente->findbykey(paciente,dni)!=-1){
            pedirDouble("Ingrese el peso del paciente", &peso);
            fecha=pedirDatos("Ingrese la fecha del control: en formato aaaa-m-d", MAX1);
            if(pc->saveObj(pc,dni,fecha,peso,true))
                              printf("Control de paciente agregado. \n");
            else
                printf("ERROR. El paciente ya tiene un control en la fecha. \n");
         }
         else{
             printf("ERROR. El paciente ingresado no existe. \n");
         }
   }while(confirma("Desea registrar otro control a un paciente?"));
}



//ERROR. pide la fecha del sistema como clave y puede tomarla repetida aunque no porque toma segundos de diferencia enre ingresos
void agregarPacienteProfesional(){
     
     obj_paciente *paciente = paciente_new();
     obj_profesional * profesional= profesional_new();
     obj_paciente_profesional *pp = paciente_profesional_new();
     char* fecha_hasta;
     int DNI_paciente, DNI_profesional;
     
     do{    
         pedirEntero("Ingrese el dni del paciente", &DNI_paciente);
         if(paciente->findbykey(paciente,DNI_paciente)!=-1){
            pedirEntero("Ingrese el dni del profesional", &DNI_profesional);
            if(profesional->findbykey(profesional,DNI_profesional)!=-1){                                      
                fecha_hasta=pedirDatos("Ingrese la fecha del control: en formato aaaa-m-d", MAX1);
                if(pp->saveObj(pp, DNI_paciente, DNI_profesional, fecha_hasta, true))
                    printf("Paciente agregado al profesional ....\n");
                else
                    printf("ERROR. el paciente ya se atiende con este profesional \n");
            }
            else{
                printf("ERROR. El profesional ingresado no existe. \n");
            }
         }
         else{
             printf("ERROR. El paciente ingresado no existe.\n");
         }
  }while(confirma("Desea registrar otro paciente a un profesional?"));
}


//BIEN. no presentar errores.
void agregarProfesional(){
   
    obj_profesional *profesional = profesional_new();  
    char* nombre, *apellido, *matricula, *telefono;
    int dni, es_med, es_nutri;
    
    do{
        pedirEntero("Ingrese el dni del profesional a registrar", &dni);
        if(profesional->findbykey(profesional,dni)==-1){
            nombre=pedirDatos("Ingrese el nombre del prfesional", MAX1);
            apellido=pedirDatos("Ingrese el apellido del prfesional", MAX1);
            matricula=pedirDatos("Ingrese el numero de matricula del prfesional", MAX1);
            telefono= pedirDatos("Ingrese el telefono", MAX2);
            es_med=validarProfesion("Es medico?");
            es_nutri=validarProfesion("Es nutricionista?");
            if(profesional->saveObj(profesional, dni, matricula, nombre, apellido, telefono, es_med, es_nutri, true))
                printf("Nuevo profesional agregado. \n");
            else
                printf("ERROR en BD.\n");
        }
        else{
            printf("ERROR. El profesional ingresado ya existe.\n");
        }
  }while(confirma("Desea registrar otro profesional?"));
}


//CLAVE AUTOGENERADA: NO PEDIR CLAVE!!
void agregarDieta(){
    
    obj_dieta *dieta = dieta_new(); 
    char *nombre, *autor, *descripcion;
    int codigo;
    
    do{
        pedirEntero("Ingrese el codigo de la dieta", &codigo);
        if(dieta->findbykey(dieta,codigo)==-1){
            nombre=pedirDatos("Ingrese el nombre de la dieta", MAX1);
            autor= pedirDatos("Ingrese el autor", MAX1);
            descripcion= pedirDatos("Ingrese una breve descripcion", MAX3);
            if(dieta->saveObj(dieta, nombre, autor, descripcion, true))
                printf("Nueva dieta agregada.\n");
            else
                printf("ERROR en BD. no se pudo crear la dieta \n");
        }
        else{
            printf("ERROR. La dieta ingresada ya fue cargada.\n");
        }
    }while(confirma("Desea registrar otra dieta?"));
}


//BIEN. no encuentra errores.
void agregarDietaPaciente(){
    
    obj_dieta *dieta = dieta_new();
    obj_paciente *paciente = paciente_new();
    obj_dieta_paciente *dp = dieta_paciente_new(); 
    char* fecha, *fecha_fin;
    int codigoDieta, dni;

    do{
        pedirEntero("Ingrese el codigo de la dieta", &codigoDieta);
        if(dieta->findbykey(dieta,codigoDieta)!=-1){
            fecha=pedirDatos("Ingrese la fecha de inicio en formato aaaa-m-d", MAX1); //verificar que pasa con esa fecha
            fecha_fin=pedirDatos("Ingrese la fecha hasta que rige la dieta en formato aaaa-m-d", MAX1);
            pedirEntero("Ingrese el dni del paciente", &dni);
            if(paciente->findbykey(paciente, dni)!=-1){
                if(dp->saveObj(dp,codigoDieta, dni, fecha, fecha_fin, true))
                    printf("Dieta agregada a paciente exitosamente.. \n");
                else
                    printf("ERROR. La dieta ingresada ya fue cargada al paciente.\n");
            }
            else{
                printf("ERROR. El paciente ingresado no existe.\n");
            }    
        }
        else{
            printf("ERROR. La dieta ingresada no existe.\n");
        }
  }while(confirma("Desea registrar otra dieta a un paciente? "));
}



//CLAVE AUTOGENERADA: NO PEDIR CLAVE!!
void agregarIngrediente(){
    
    obj_ingrediente *ingrediente = ingrediente_new(); 
    char* nombre, *unidad_medida;
    int codigo;
    
    do{
        pedirEntero("Ingrese el codigo del ingrediente", &codigo);
        if(ingrediente->findbykey(ingrediente,(codigo))==-1){ 
            nombre=pedirDatos("Ingrese el nombre de ingrediente", MAX1);            
            unidad_medida=pedirDatos("Ingrese la unidad de medida", MAX1);
            if(ingrediente->saveObj(ingrediente, nombre, unidad_medida, true))
                printf("Ingrediente agregado.. \n");
            else
                printf("Error en BD. reintente \n");   
        }
        else{
            printf("ERROR. El ingrediente ya fue cargado.\n");
        }   
  }while(confirma("Desea registrar otro ingrediente?"));
}


//CODIGO AUTOGENERADO: NO PEDIR CLAVE!!
void agregarPlato(){
    
    obj_plato *plato = plato_new(); 
    char *nombre, *detalle;
    int codigo;
  
    do{
        pedirEntero("Ingrese el codigo del plato", &codigo);
        if(plato->findbykey(plato,codigo)==-1){
            nombre=pedirDatos("Ingrese el nombre del plato", MAX1);
            detalle=pedirDatos("Ingrese un detalle", MAX3);
            if(plato->saveObj(plato, nombre, detalle, true))
                printf("Plato agregado.. \n");
            else
                printf("Error en BD. reintente \n");   
        }
        else{
            printf("ERROR. El plato ya fue cargado.\n");
        }  
  }while(confirma("Desea registrar otro plato?"));
}


//BIEN. no tiene errores.
void agregarPlatoIngrediente(){
    
    obj_plato *plato= plato_new();
    obj_plato_ingrediente * pi = plato_ingrediente_new();
    int codigoPlato, codigoIngrediente;
    float cantidad;
    
    do{
        pedirEntero("Ingrese el codigo del plato", &codigoPlato);
        if(plato->findbykey(plato,codigoPlato)!=-1){
            pedirDouble("Ingrese la cantidad", &cantidad);
            pedirEntero("Ingrese el codigo del ingrediente", &codigoIngrediente);
            if(pi->saveObj(pi,codigoPlato, codigoIngrediente ,cantidad, true))
                printf("Ingrediente agregado al plato.. \n");
            else
                printf("ERROR. El plato ya posee ese ingrediente. \n"); 
        }
        else{
            printf("ERROR. El plato ingresado no existe.\n");
        } 
  }while(confirma("Desea registrar otro ingrediente a un plato?"));
}

/*******************************************************************************
                                    LISTADOS
********************************************************************************/

void listarPLATO(FILE *archivo){

  char* cadena= (char*)malloc(sizeof(char)*300);
  void * list;
  int i=0, tam=0, j;
  obj_plato *plato = plato_new();  
  tam = plato->findAll(plato,&list,NULL); 
  
    if(archivo==NULL){
        for(i=0;i<tam;++i){
           plato = ((obj_plato**)list)[i];
           printf("cod plato: %d nombre: %s  descripcion: %s \n",plato->codigo, plato->nombre, plato->detalle);
        }
    }
    else{
        for(i=0;i<tam; ++i){
            plato = ((obj_plato**)list)[i];
            sprintf(cadena,"codigo: %d nombre: %s detalle: %s\n",plato->codigo, plato->nombre, plato->detalle);
            fprintf(archivo,"%s",cadena);  
        }
    }
    free(list);
}



void listarPP(){

  void * list;
  int i=0, size=0, j;
  obj_paciente_profesional *pp = paciente_profesional_new();  
  
  size = pp->findAll(pp,&list,NULL); 
  
  for(i=0;i<size;++i){
      pp = ((obj_paciente_profesional**)list)[i];
      printf("co%d): dniPACI:%d   dniMEDICO:%d  fechaDESDE:%s   fechaHASTA:%s \n",i+1,pp->dni_paciente, pp->dni_medico, pp->fecha_desde, pp->fecha_hasta);
  }
  free(list);   
}



void listarINGREDIENTE(){

  void * list;
  int i=0, size=0, j;
  obj_ingrediente *ingrediente = ingrediente_new();  
  size = ingrediente->findAll(ingrediente,&list,NULL); // se invoca sin criterio - listar todos...
  
  for(i=0;i<size;++i){
      ingrediente = ((obj_ingrediente**)list)[i];
      printf("elemento %d) codigo: %d nombre:%s   unidad mediad:%s \n",i,ingrediente->codigo, ingrediente->nombre, ingrediente->unidadmed);
  }
  free(list);   
}


void listarPLATOINGREDIENTE(){
     void * list;
     int i=0, size=0, j;
     obj_plato_ingrediente *PI = plato_ingrediente_new();  
     size = PI->findAll(PI,&list,NULL); 
  
      for(i=0;i<size;++i){
          PI = ((obj_plato_ingrediente**)list)[i];
          printf("elemento %d) codPlato: %d  codIngrediente: %d  cantidad: %d \n",i,PI->cod_plato, PI->cod_ingrediente, PI->cantidad);
      }
      free(list); 
}

void listarDIETA(){

  void * list;
  int i=0, size=0, j;
  obj_dieta *dieta = dieta_new();  
  
  size = dieta->findAll(dieta,&list,NULL); 
  
  for(i=0;i<size;++i){
      dieta = ((obj_dieta**)list)[i];
      printf("cod dieta: %d nombre: %s  autor: %s fechaAlta: %s descripcion: %s \n",dieta->codigo, dieta->nombre, dieta->autor, dieta->fecha_alta,dieta->descripcion);
  }
  free(list);   
}

void listarPACIENTE(){

  void * list;
  int i=0, size=0, j;
  obj_paciente *paciente = paciente_new();  
  
  size = paciente->findAll(paciente,&list,NULL); 
  
  for(i=0;i<size;++i){
      paciente = ((obj_paciente**)list)[i];
      printf("cod: %d nombre: %s  \n",paciente->dni, paciente->nombre);
  }
  free(list);   
}

void listarPROFESIONAL(){

  void * list;
  int i=0, size=0, j;
  obj_profesional *profesional = profesional_new(); 
  char *es_med=(char*)malloc(sizeof(char)*3); 
  char *es_nutri=(char*)malloc(sizeof(char)*3); 
  
  size = profesional->findAll(profesional,&list,NULL); 
  
  for(i=0;i<size;++i){
      profesional = ((obj_profesional**)list)[i];
        if(profesional->es_med)
            strcpy(es_med,"si");
        else
            strcpy(es_med,"no");

        if(profesional->es_nutri)
            strcpy(es_nutri,"si");
        else
            strcpy(es_nutri,"no");

      printf("DNI: %d matricula:%d  nombre: %s apellido:%s telefono: %s fechaAlta: %s es medico?:%s esNutri?:%s \n",profesional->dni, profesional->matricula, profesional->nombre,profesional->apellido,profesional->matricula,profesional->fecha_alta, es_med,es_nutri); 
}
  free(list);   
}


void listarPACIENTECONTROL(){

  void * list;
  int i=0, size=0, j;
  obj_paciente_control *PC = paciente_control_new();  
  
  size = PC->findAll(PC,&list,NULL); 
  
  for(i=0;i<size;++i){
      PC= ((obj_paciente_control**)list)[i];
      printf("Dni paciente: %d, fecha: %s, peso: %f \n", PC->dni_paciente, PC->fecha, PC->peso);
  }
  free(list);   
}

void listarDIETAPACIENTE(){

  void * list;
  int i=0, size=0, j;
  obj_dieta_paciente *DP = dieta_paciente_new();  
  
  size = DP->findAll(DP,&list,NULL); 
  
  for(i=0;i<size;++i){
      DP = ((obj_dieta_paciente**)list)[i];
      printf("codigo: %d codDieta: %d  dniPaciente: %d fechaAlta: %s fechaFin: %s \n",DP->codigo, DP->cod_dieta, DP->dni_paciente, DP->fecha,DP->fecha_fin);
  }
  free(list);   
}

/****************************************************************************
                                    NUEVO
****************************************************************************/

int es_numero(char *cadena){
    int i;
    for(i=0; i<strlen(cadena);i++){
       if(isdigit(cadena[i])==0){
         return 0;
       }
    }
    return 1;
}

void inicializar_programa(t_comandos *comando){
  
    int i;
    for(i=0;i<14;i++){
        (*comando).parametro[i].anexo = 0;
        (*comando).parametro[i].codigo = 0;
    }
    (*comando).parametro[0].nombre = "-ldieta";
    (*comando).parametro[1].nombre = "-lpaciente";
	(*comando).parametro[2].nombre = "-lingrediente";
	(*comando).parametro[3].nombre = "-lprofesional";
	(*comando).parametro[4].nombre = "-lplato";
	(*comando).parametro[5].nombre = "-apaciente";
	(*comando).parametro[6].nombre = "-apaciente-control";
    (*comando).parametro[7].nombre = "-apaciente-profesional";
    (*comando).parametro[8].nombre = "-aprofesional";                                     
    (*comando).parametro[9].nombre = "-adieta";
    (*comando).parametro[10].nombre= "-adieta-paciente";
    (*comando).parametro[11].nombre= "-ingrediente";
    (*comando).parametro[12].nombre= "-plato";
    (*comando).parametro[13].nombre= "-plato-ingrediente"; 
}

void abrir_archivo(char* nombre, FILE **destino){
    
	*destino = fopen ( nombre, "w" );        
	if (*destino==NULL) {fputs ("ERROR DE ARCHIVO",stderr); exit (1);}
}


int terminar(int tope, char ** argumentos, t_info *registro){
    
    if(argumentos[2]!=NULL){
        if(es_numero(argumentos[2])){
           (*registro).anexo=atoi(argumentos[2]);
           if(argumentos[3]!=NULL){
             if(strcmp(argumentos[3],"-f")==0){
                 if(argumentos[4]!=NULL){
                    abrir_archivo(argumentos[4],(&(*registro).archivo));
                 }
                 else{
                    return 1; 
                 } 
              }
              else{
                   return 1; 
              }
           }
        }
        else{
            if(strcmp(argumentos[2],"-f")==0){
                 if(argumentos[3]!=NULL){
                     abrir_archivo(argumentos[3],(&(*registro).archivo));
                 }
                 else{
                     return 1;
                 }  
            }
            else{
                 return 1; 
            }
        }   
    }   
    return 0;     
}

void hacia_donde_apunto(t_comandos comando){
    
    if(comando.parametro[0].codigo){
        if(comando.parametro[0].anexo)
            printf("listar dieta con codigo\n");
        else
            printf("listar dieta\n");
         
    }  
    if(comando.parametro[1].codigo)
        printf("Listar paciente\n");
    if(comando.parametro[2].codigo){
        if(comando.parametro[2].anexo)
            printf("listar ingrediente con codigo plato\n");
        else
            printf("listar ingrediente\n"); 
    }   
    if(comando.parametro[3].codigo)
        printf("Listar profesional\n");
    if(comando.parametro[4].codigo){
        if(comando.parametro[4].anexo)
            printf("listar plato con codigo\n");
        else
            //printf("listar plato\n"); 
            listarPLATO(comando.parametro[4].archivo);
    }
    /****************************************/
	if(comando.parametro[5].codigo)
	   printf("alta paciente\n"); 
	if(comando.parametro[6].codigo)
	   printf("alta paciente control\n"); 
	if(comando.parametro[7].codigo)
	   printf("alta paciente profesional\n"); 
	if(comando.parametro[8].codigo)
	   printf("alta profesional\n"); 
	if(comando.parametro[9].codigo)
	   printf("alta dieta\n");
    if(comando.parametro[10].codigo)
	   printf("alta dieta paciente\n"); 
    if(comando.parametro[11].codigo)
	   printf("alta ingrediente\n");  
	if(comando.parametro[12].codigo)
	   printf("alta plato\n"); 
	if(comando.parametro[13].codigo)
	   printf("alta plato ingrediente\n"); 
}

/*********************************************************************
                        PROGRAMA PRINCIPAL
************************************************************************/

int main(int argc, char *argv[]){  

    char *port="5432",*servidor="localhost",*base="nutricion", *usuario="postgres", *password="postgres";
    connectdb(servidor,port,base,usuario,password);
    t_comandos comando;
    inicializar_programa(&comando);
    int i;
    int encontrado=0;
    
    if(argv[1]!=NULL){
        for(i=0; i<13; i++){
            if(strcmp(comando.parametro[i].nombre, argv[1]) == 0){
                //printf("encontrado \n");
                comando.parametro[i].codigo=1;
                encontrado=1;
                break;
            }
        }
        if(!encontrado){
            printf("error de uso de programa \n");
        }
        else{
             if(!terminar(argc, argv, &(comando.parametro[i])))
                hacia_donde_apunto(comando);
             else
                printf("error de uso de programa\n");
        }
    }
    else{
        printf("Error. debe ingresar al menos un comando\n");
    }
  disconnectdb();
  system("PAUSE");	
  return 0;
}
