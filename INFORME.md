# Tarea 4: Protección de Memoria en xv6

## Objetivo

Implementar un sistema de permisos básicos en xv6 (risc) que permita modificar archivos para tener acceso de solo lectura o lectura/escritura, y agregar un nuevo permiso especial para hacerlo inmutable.

## Modificaciones Requeridas

 1. Modificar estructura Inodes
    - campo de bits de permisos para definisr si el archivo es solo lectura o escritura. (int) "0": none; "1":Read; "2":Write; "3":Both
 2. Modificar Operaciones open/read/write
    - Modificar funciones para que tomen en cuenta los permisos
 3. Crear llamada del sistema chmod(archivo:char*, modo:int)
    - Change-Modification de los permisos. dado un int.
 4. Pruebas;
    - Crear archivo con permisos O_CREATE | O_RDWR
    - Cambiar Permisos a solo LEctura chmod(filename,1)
    - Prueba de escritura con solo lectura O_RWONLY
    - Cambio de Permisos de vuelta a Lectura/Escritura chmod(filename,3)
    - Escritura final
PARTE 2 !?
 1. Agregar Permiso especial. Immutable (REAN_ONLY) y no cambiable
 2. Pruebas
    - Cambio de Permisos a inmutable
    - Prueba e Escritura a Solo Lectura
    - Cmbio e vuelta a Lectura/Escritura

## Entrega
    - Codigo fuente + Tests
    - Informe README.md : Funcionamiento y logica de la proteccion de memoria, explicando las modificaciones realizadas.
    - Dificultades Encontradas y soluciones implementadas.

# Informe de avanze

> Log 01

Hora de ponerlo todo en la linea para completar esto. El reloj esta avanzando, y nada lo puede parar.

A ver... Inodes, Recuerdo haber leido referencias a estos durante el estudio del codigo fuente en tareas anteriores. Revisando defs.h, Sus funciones estan en fs.c, filesystem... tiene sentido. Buscando la palabra, Parece que otros documento tambien trabajan con la estructura, file .c .h se ven relevantes. y sysfile.c tambien. A leer se ah dicho.