create table paciente( dni integer, nombre varchar(40), apellido varchar(50), domicilio varchar(50), telefono varchar(15), fecha_alta date, fecha_nac date, peso_inicial float, talla float, CONSTRAINT pk_paciente PRIMARY KEY (dni));	

create table dieta (codigo serial, nombre varchar(40), autor varchar(80), descripcion varchar(150), fecha_alta date , CONSTRAINT pk_dieta PRIMARY KEY (codigo));

create table dieta_paciente (codigo serial, cod_dieta int, dni_paciente int , fecha date, fecha_fin date,CONSTRAINT pk_dieta_paciente PRIMARY KEY(codigo),  CONSTRAINT fk_dieta_paciente_dieta FOREIGN KEY(cod_dieta) REFERENCES dieta(codigo), CONSTRAINT fk_dieta_paciente_paciente FOREIGN KEY(dni_paciente) REFERENCES paciente(dni));

create table platos (codigo serial,nombre varchar(150), detalle varchar(350) , CONSTRAINT pk_platos PRIMARY KEY (codigo) );

create table ingredientes(codigo serial, nombre varchar(50), uni_med varchar(20), CONSTRAINT pk_ingrediente PRIMARY KEY (codigo));

create table plato_dieta(cod_plato int, cod_dieta int, porcion float, CONSTRAINT pk_plato_dieta PRIMARY KEY (cod_plato,cod_dieta), CONSTRAINT fk_dieta_plato_dieta FOREIGN KEY(cod_dieta) REFERENCES dieta(codigo), CONSTRAINT fk_plato_plato_dieta FOREIGN KEY(cod_plato) REFERENCES platos(codigo));

create table plato_ingrediente(cod_plato int,cod_ingrediente int,cantidad float, CONSTRAINT pk_plato_ingrediente PRIMARY KEY (cod_plato ,cod_ingrediente) , CONSTRAINT fk_plato_plato_ingrediente FOREIGN KEY(cod_plato) REFERENCES platos(codigo), CONSTRAINT fk_ingrediente_plato_ingrediente FOREIGN KEY(cod_ingrediente) REFERENCES ingredientes(codigo));

create table plato_paciente(cod_plato int, dni_paciente int, cantidad float , fecha timestamp without time zone NOT NULL DEFAULT now()
, CONSTRAINT pk_plato_paciente PRIMARY KEY (cod_plato ,dni_paciente,fecha) , CONSTRAINT fk_plato_plato_paciente FOREIGN KEY(cod_plato) REFERENCES platos(codigo),CONSTRAINT fk_paciente_plato_paciente FOREIGN KEY(dni_paciente) REFERENCES paciente(dni) );

create table paciente_control(dni_paciente int, fecha timestamp without time zone NOT NULL DEFAULT date_trunc('second', now()), peso float, CONSTRAINT pk_paciente_control PRIMARY KEY (dni_paciente ,fecha), CONSTRAINT fk_paciente_paciente_control FOREIGN KEY(dni_paciente) REFERENCES paciente(dni) );

create table profesional(dni int,matricula varchar(10),nombre varchar(50),apellido varchar(60),telefono varchar(15),fecha_alta timestamp without time zone NOT NULL DEFAULT date_trunc('second', now()), es_med int, es_nutri int , CONSTRAINT pk_profesional PRIMARY KEY (dni));

create table paciente_profesional(dni_paciente int,dni_medico int,fecha_desde timestamp without time zone NOT NULL DEFAULT date_trunc('second', now()),fecha_hasta date , CONSTRAINT pk_paciente_profesional PRIMARY KEY (dni_paciente ,dni_medico, fecha_desde) , CONSTRAINT fk_paciente_paciente_profesional FOREIGN KEY(dni_paciente) REFERENCES paciente(dni), CONSTRAINT fk_profesional_paciente_profesional FOREIGN KEY(dni_medico) REFERENCES profesional(dni));

-------------  insercion 

insert into paciente(dni,nombre,apellido,domicilio,telefono,fecha_nac,peso_inicial,talla) values(22151741,'Ariel','Perez','Edison 112','4412345','1975-08-01',85,175),(25741852,'Esteban','Lozano','Soler 1741','4459855','1988-05-14',90,170);

insert into dieta (nombre,autor,fecha_alta) values('dieta mediterranea', '-', '2016-04-01'),('dieta disociada', '-', '2016-04-10');

insert into dieta_paciente (cod_dieta,dni_paciente,fecha) values(1,25741852, '2016-04-01');

insert into platos( nombre, detalle) values('ensalada de tomate','tomates, aceite sal y pimienta');

insert into paciente_control(dni_paciente,fecha,peso)values(22151741,date_trunc('second', now()), 82.7);

insert into ingredientes(nombre, uni_med) values('papa','kg'),('carne','kg'),('sal','gr'),('limon','kg'),('pimienta','gr');

insert into plato_dieta(cod_plato,cod_dieta,porcion)values(1,1,1);

insert into plato_ingrediente(cod_plato,cod_ingrediente, cantidad) values(1,3,5),(1,4,1);

insert into plato_paciente(cod_plato, dni_paciente,cantidad) values(1,25741852,1);

insert into profesional(dni,matricula,nombre,apellido,telefono,fecha_alta,es_med,es_nutri) values(22554496,'nutri-321','Diego','Rojas','280154789654',date_trunc('second', now()),0,1),(19956321,'med-1021','Raul','Morales','280154956321',date_trunc('second', now()),1,0);

insert into paciente_profesional(dni_paciente, dni_medico) values (22151741,19956321);