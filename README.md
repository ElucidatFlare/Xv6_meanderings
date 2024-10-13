# Reporte de progreso en la Tarea 2 y sus resultados.


## Tarea 2: 

### Objetivo
Modificar el programador de procesos en xv6 para implementar un sistema
de prioridades.

## Parte I: Incorporar Prioridad

 - [X] Agragar un cambio de prioridad

 - [X] Inicializar prioridad en 0

## Parte II: Incorporar Boost

- [X] Agragar un cambio de prioridad

- [X] Inicializar en 1

## Consejos (Hints)

 * Revizar el codigo existente en Xv6
 * Documentar las modificaciones

## Detalles


 - [X]    Al código fuente de xv6, agregar un campo de prioridad a la estructura de proceso.
 - [X]    Inicializar la prioridad de cada proceso en 0 (menor número = mayor prioridad).
 - [X]    Agregar un campo de boost a la estructura de proceso.
 - [X]    Inicializar el boost en 1.
 - [ ]    Cada vez que un proceso ingresa, aumentar la prioridad de todos los procesos existentes que puedan ser ejecutados (no zombies).
 - [ ]    Implementar la lógica: Prioridad += Boost.
 - [ ]    Si la prioridad alcanza 9, cambiar el boost a -1.
 - [ ]    Si la prioridad llega a 0, cambiar el boost a 1.
 - [ ]    Crear programa de prueba.

## Pruebas

 * Utilice fork para crear al menos 20 procesos.
 * Cada proceso muestre un mensaje en pantalla al ejecutarse, con el formato: “Ejecutando proceso ”.
• Incluya un sleep para hacer que los procesos se detengan unos
segundos

### Consejos ( Hints )
 * Revisar el código del programador de procesos existente para entender cómo se gestionan los procesos.
 * Asegurarse de que los cambios no afecten negativamente el funcionamiento general del sistema.
 * Documentar adecuadamente las modificaciones realizadas.
## Entrega
  * Codigo modificado de xv6
  * Tests
  * Informe (README)
## Instrucciones de Entrega


### Reportaje

> Log 01
Comenze Probando donde quede en la tarea anterior, viendo como crear tests. Partiendo desde cero, arme un Hello World. Hace bastante que no programaba en C, y me dio recordo viejos tiempos olvidar invocar Stdio.h.
 Despues de corroborar que compila y corria, le di una mirada como printeaban otros ejecutables en xv6 sin la necesidad de llamar a las librerias de c. Siguiendo el rastro desde echo.c, Buscando donde se definia write(), Me lleva ala definision de sys_write() en sysfile.c. el cual me dirige a otra definicion, filewrite(f, addr, n) en file.c. Este se ve prometedor.
 Despues de comprobar que write(1. "texto", 1), solo entrega el primer caracter, y que el parametro n dirige el len del output. El print fue exitoso. Supongo que f=1 implica que el file 1 es la consola. Por curiosidad entregue un len mas extenso que el string y con el overflow me entrego "(null)0123456789ABCDEF$", curioso... Si bien el n entregado era mas extenso, se corto la lectura en ese punto. Y aparte, en el el string entregado un array "digits" presente en printf.c.
 Creo que me detendre de avanzar por esta tangente. Al menos ya se que esta ves si podre armar un test.c

 > Log 02
 Volviendo a la labor principal. Supongo que para agregar Prioridad y Boost habra que buscar el PCB de los procesos y agregar un int que sea la prioridad.
 Incorporar el campo debiera ser sencillo. Algo como:
"""
... 
    int priority;
    int boost;
...
"""
 Para inicializar la prioridad en 0, tendre que averiguar donde se inicialisan procesos, y agregar un campo con: `p->priority = 0;`
 Similar con el Boost, pero en otras funciones quizas: `p->boost = 1;`
Probablemente hallan multiples funciones que inicialisen procesos... y quizas tenga que modificar el len de algunas alocaciones de memoria... a ver que encuentro.

> Log 03
De momento, efectivamente agregue Priority y boost como planee. Esto lo implemente en proc.h
Tengo dudas de si Boost corresponda ahi. Pero esta bien de momento.
Supongo que mejor lo junto con las variables que los comentarios recomienda poseer Locks.

Siguiente esta implementar la inicialiacion...
Pasando de proc.h a proc.c, leyendo como se inicialican los procesos... dando unas cuantas vueltas entre funciones y definiciones. Al inicio procinit() se veia como la opcion mas directa, pero inicializar la proc table deja la mayoria de los proc vacios, excepto por el state y kstack.
Hay varias funciones que "Inicializan" los proc, en diferentes estados. por intuicion, Voy a pasar de hacerlo en la que los inicializa en "Unused". Pero dudo si sera mejor inicializar la prioridad en las funciones que establecen el estado "USED" como en "allocproc()" o "RUNNABLE" como en "userinit()" y "fork()"...
Elegire inicializar las variables en allocproc(), para que en caso de meter la pata, almenos cambie solo una funcion. Ademas que tanto userinit y fork llamaban a esta, por lo que la cadena de dependencias pareciera implicar como la mejor opcion.
"""
  ...
  p->priority = 0;          //★
  p->boost = 1;             //★
"""
Y como para todo constructor hay que tambien el destructor, agrege a freeproc() que ambas se vayan a 0 al liberar un proceso.
Ahora a ver si compila con estos cambios.
Exito.

> Log 04
 Parece que todavia me quedaba mas de lo que creia...
    "Cada vez que un proceso ingresa, aumentar la prioridad de todos los procesos existentes que puedan ser ejecutados (no zombies)."
Entonces... "poder ser ejecutados" seria "Run-able"?, ṕor lo que:
    - al crear un proceso, 
    - priority++ if case.
    Pero, sera solo aumentar en "Runnable"? Debiera incluir "Sleeping" y "Running?".
Al menos "Used" no creo, ya que es el paso previo a Runnable, y cuando se utiliza es crear el proceso, por lo que creo que es la mejor posicion para implementar el priority++. Pero...
Me dejara?... Al invocar el "allocproc()"  manejo 1 "lock" del proceso mismo. Si mando a aumentar la prioridad del resto de los procesos... estoy trabajando fuera de este lock... y si no me deja?...
Creo que en este punto ya se va a empezar a complicar...
De momento. supongo que podria diseñar una funcion que itere la proctable y aumente la prioridad.

