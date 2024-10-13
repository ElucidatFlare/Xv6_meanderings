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
 - [X]    Cada vez que un proceso ingresa, aumentar la prioridad de todos los procesos existentes que puedan ser ejecutados (no zombies).
 - [X]    Implementar la lógica: Prioridad += Boost.
 - [X]    Si la prioridad alcanza 9, cambiar el boost a -1.
 - [X]    Si la prioridad llega a 0, cambiar el boost a 1.
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
~~~
... 
    int priority;
    int boost;
...
~~~
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
Entonces... "poder ser ejecutados" seria "Run-able"?, por lo que:
    - al crear un proceso, 
    - priority++ if case.
    Pero, sera solo aumentar en "Runnable"? Debiera incluir "Sleeping" y "Running?".
Al menos "Used" no creo, ya que es el paso previo a Runnable, y cuando se utiliza es crear el proceso, por lo que creo que es la mejor posicion para implementar el priority++. Pero...
Me dejara?... Al invocar el "allocproc()"  manejo 1 "lock" del proceso mismo. Si mando a aumentar la prioridad del resto de los procesos... estoy trabajando fuera de este lock... y si no me deja?...
Creo que en este punto ya se va a empezar a complicar...
De momento. supongo que podria diseñar una funcion que itere la proctable y aumente la prioridad.

> Log 05

Despues de distraerme un rato viendo el scheduler, y debatiendo si tendria que leer como funciona los contextos y el contextswitch. Me di cuenta que no considere que pasa en los edge cases de aumentar la prioridad. y que asumia que era un priority++. Leyendo la lista de Detalles, entonces, si agrego una funcion increment_priority() (Pensare en un mejor nombre luego) en algun punto del codigo, la logica seran los otros objetivos de la entrega.

Ok, hacer una funcion que tenga:
"""
    if ( p->priority == 9){p->boost =-1};
    if ( p->priority == 0){p->boost = 1};
    p->priority += p->boost;
"""
Suena sencillo, pero y al menos ese seria el incremetador, ahora voy a ver donde poner la funcion que itere sobre todos los procesos, y que no rompa todo con el manejo de los candados...
Curioso, la prioridad solo se modificaria al crear nuevos procesos... y no me piden que agregue esta prioridad a la toma de decisiones del scheduler. Que bueno, porque sino seria mas complejo.
...
Agregue de momento "priority_next()" en proc.c con la logica descrita en este log.
~~~
// Priority Increment
void
priority_next(struct proc *p)
{
  if ( p->priority == 9)
    p->boost =-1;
  if ( p->priority == 0)
    p->boost = 1;
  p->priority += p->boost;
}
~~~
No parece crashear. Pero claro, no estoy cambiando nada dinamicamente aun...

> Log 06 (Ignore This One)

Con la esperanza de no tener que hacer los test antes de continuar. Meror dejo un registro de los apuntes de los"Locks", porque se que si no los reviso me van a traer problemas.
Los candados estan en Spinlock .c y .h ; 
- Contienen el flag del lock, un nombre y el cpu asignado.
Entre las funciones esta el 
- inicializador. Holding para checkear. y Acquire()/Release() son similares a push_off()/pop_off(), pero los primeros requieren el *spinlock. Los primeros llaman a los segundos para abrir o cerrar los candados. pero hacen unas cuantas cosas mas, pero no creo que me sirva por ahora anotar mas al respecto.

En proc.c se usan 3 spinlocks. "pid_lock", con nombre "nextpid" solo se llama cuando se alloca la ID.
"struct proc->lock", con nombre "proc". Es el lock de cada proceso.
"wait_lock" con nombre "wait_lock". //Comentarios implican que es importante dentro de el sistema y la memoria. Pero mantendre en cuanta que es obligacion adquirirlo previamente a invocar cualquier p->lock.
(Al menos a lo que respecta con procesos Padre-Hijo?)

Wait_lock es llamado en: fork, exit y wait.
    Durante el periodo Adquirido se modifican "proc->parent", se llama reparent() y wakeup() pero parece que todo wait_lock es adquirido y liberado dentro de la misma funcion que lo llama---
    Aunque algunos proc Locks quedan adquiridos sin release (dentro). Supongo que no todas los aquire/release seran dentro de estos wait_locks entonces... Pero ahora debo segir el rastro de releases en general.
Tomando nota de esto:
allocproc(): acquires todos y libera si esta utilizado, o se cae durante su ejecucion.
    Si Unused... inicializa, si se cae, libera. y no parece liberar si retorna p.
    (returns Locked proc)
Fork invoca este *p, y lo libera justo antes de trabajar en waitlock. desíes adqioere y libera el candado. (No Locks)
userinit(): trabaja este *p como initproc... pero lo libera al final (Return no locks)
exit (): adquiere el Wait, luego adquiere un Lock, libera el wait, pero no libera el proc (return 1 Lock)
wait(): adquiere el wait. Se manda un Loop. Si es que todo esta abierto. Debiera adquirir Locks, y por condiciones liberar el lock y el wait, o solo el lock. y eventualmente el wait. No parece dejar Locks sueltos, pero se trabaria si uno de los Locks esta cerrado mientras corre (Returns no Locks)
sleep(): 1 acquire, y manda un release a un lock externo?, pero Libera el lock y adquiere el externo antes de terminar. (No locks)
wakeip(): segun el channel, itera procs, lockeando al observar y liberando al salir. (no lock out)
kill(): similar, iter, lock, check, release.
setkilled(), killed(): adquiere y libera rapidamente
yield() parace abrir i cerrar, con un sched() de por medio.
scheduler(), Tambien itera, pero abre y cierra...
No tengo idea donde termina abriendose el Lock de Exit(), me preocupa. Que pueda causarme problemas a futuro. Supongo que le preguntare al profe la proxima clase sobre esto.
...
Despues de meditar un rato. Supongo que no no sirve quedarse con miedo a romper el sistema.

> Log 07

Entonces, hay muchas funciones que iteran sobre la tabla de procesos. abriendo y cerrando candados.
Hay que modificar prioridades al crear nuevos procesos.
Si se aplica en allocproc(), tendria que ser en el punto previo a retornar el *p, porque sino estaria boosteando en casos que el proceso no se cree exitosamente. en ese punto, existe el Lock del proceso que crea el puntero. Por lo que no podria copiar la iteracion que se aplica en otras funciones... No quiero imaginar que pasa si llamo a un aquire a un lock dentro del codigo que ya esta adquirido.

Ante eso mi idea seria.

~~~
     for(p2 = proc; p2 < &proc[NPROC]; p2++) {
         if (&p2 == &p){
            continue
        }
        acquire(&p->lock);
         if (p2->state == RUNNABLE)
            increment_priority:
        release(&p->lock);
~~~
Supongo que el primer condicional saltaria el proceso actual. Evitando adquirir el proceso actual. Pero no tengo idea si funcionara.
Addendum: COnsiderando que piden "puedan ser ejecutados", Runnable es claramente si. Pero poder ser ejecutado, y estar siendo ejecutado son bastante diferentes... y estar durmiendo no es exactamente estar disponible para ser ejecutado... asumire que solamente suba la prioridad a runnable.
Addendum2; Reparent justamente tiene una iteracion a travez de toda la tabla y compara que 2 pointers apunten al mismo. Creo que me guiare por esto y corregire mi codigo hipotetico
Termine implementando la rutina en proc.c como:
~~~
void
boost_runnable(struct proc *p)
{
  struct proc *np;  //Process different of *p

  for(np = proc; np < &proc[NPROC]; np++){ //Itera sobre toda la tabla
    if(np == p){  //Se salta el candado actual
      continue;
    }
    if(np != p){
      acquire(&np->lock);
      if(p->state == RUNNABLE) {  // Por descarte es solo este caso.
        priority_next(p);
      }

      release(&np->lock);
    }
  }
}
~~~

No parece crashear al compilar. Pero no puedo verificar si este funcionando como planee.
Hora de formular los Tests.

> Log 08

Hora de la parte que menos queria armar Testing...
 - [ ] Forkear 20 procesos
 - [ ] Cada proceso tiene un print en pantalla
 - [ ] Aplicar Sleep para que hayan pausas...

Como no arme testing en la T1, supongo que voy a aprender como funcionan aca.
Todo parece trabajar en C. entonces... vere si puedo basarme en algun script de la carpeta user. o quizas el libro del ramo.
