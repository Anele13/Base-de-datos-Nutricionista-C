#include <stdlib.h>

int esNumero(char *cadena){
    int i;
    for(i=0; i<strlen(cadena);i++){
       if(isdigit(cadena[i])==0){
         return 0;
       }
    }
    return 1;
}

void VerificarTipoListado(char* param1, char* param2){
	if(strcmp("dieta", param1) == 0){
       if(param2!=NULL){
          if(esNumero(param2)){
             printf("listar dietas con el DNI encontrado \n");
          }
          else{
               printf("AYUDA: listar dietas con dni: <-lpaciente> <numero DNI> \n");
          }                      
       }   
       else{
            printf("listar TODAS las dietas, (sin DNI) \n");
       }  
    }
	else{
         if(strcmp("paciente", param1) == 0){
             printf("listar TODOS los pacientes");
         }
         else{
              if(strcmp("ingrediente", param1) == 0){
                 if(param2!=NULL){
                   if(esNumero(param2)){
                     printf("listar ingrediente con el codigo encontrado \n");
                   }
                   else{
                       printf("AYUDA: listar ingredientes con cod plato: <-lpaciente> <numero DNI> \n");
                   }                      
                 }   
                 else{
                      printf("listar TODOS los ingredientes, sin codigo \n");
                }  
              }
              else{
                  if(strcmp("profesional",param1) == 0){
                 //listar TODOS los profesionales
              }
              else{
                   if(strcmp("plato", param1) == 0){
                      if(param2!=NULL){
                        if(esNumero(param2)){
                            printf("listar platos con el codigo dieta encontrado \n");
                        }
                        else{
                             printf("AYUDA: listar plato con codigo: <-lplato> <codigo> \n");
                        }                      
                      }   
                      else{
                           printf("listar TODOS los paltos, sin codigo \n");
                      }  
                   }
                   else{
                        printf("error de uso de programa \n");
                   }
              }      
         }
    } 
}

	
}


void VerificarTipoAlta(char* param1, char* param2){
     
     if(strcmp("paciente", param1) == 0){
        //agregar info paciente y pedir datos     
     }
     else{
          if(strcmp("paciente-control", param1) == 0){
             //agregar info paciente control
          }
          else{
               if(strcmp("paciente-profesional", param1) == 0){
                  //agregar un nuevo profesionala un paciente???
               }
               else{
                    if(strcmp("profesional", param1) == 0){
                       //agregar nuevo profesional
                    }
                    else{
                         if(strcmp("dieta", param1) == 0){
                           //una nueva dieta
                         }
                         else{
                              if(strcmp("dieta-paciente", param1) == 0){
                                 //agregar una nueva dieta a un paciente
                              }
                              else{
                                   if(strcmp("ingrediente", param1) == 0){
                                      //agregar nuevo ingrediente
                                   }
                                   else{
                                        if(strcmp("plato", param1) == 0){
                                          //agrega un nuevo plato
                                        }
                                        else{
                                             if(strcmp("plato-ingrediente", param1) == 0){
                                                //agrega un nuevo ingrediente a un plato
                                             }
                                             else{
                                                 printf("Error de uso de programa \n");
                                             }
                                        }
                                   }
                              }
                         }
                    }
               }
          }
     }
}
