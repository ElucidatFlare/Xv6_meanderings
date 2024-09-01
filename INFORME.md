# Reporte de progreso en la Tarea 0 y sus resultados.


## Tarea 1: 

### Objetivo
Implementar y modificar llamadas al sistema en xv6, un sistema operativo educativo.
## Parte I: Implementación Básica (Nota máxima: 6)
### Nueva llamada al sistema: `int getppid(void)`

## Parte II: Implementación Avanzada (Para optar a nota 7)
### Nueva llamada al sistema: `int getancestor(int)`
## Consejos (Hints)
## Pruebas
## Entrega
## Instrucciones de Entrega


### Reportaje



>Log #1 02:00 PM
Todo comenzo con leer el documento T1 y entender que habria que hacer.
Todo se ve en interactuar con procesos del OS.
2 Niveles de entrega para optar a diferente nota
Segun entiendo sera crear nuevos comandos para el OS virtual.

Tasks:
 - [ ] Investigar como implementar nuevos comandos
 - [ ] Crear la llamada para getppid()
 - [ ] Evaluar si vale la pena ir por el 7
   - [ ] Implementar getancestor()
 - [ ] Crear "yosoytupadre.c"
   - [ ] agregarlo a "user"
   - [ ] Integrarlo al Makefile
 - [x] Crear Rama de tarea 1
 - [ ] Finalizar informe y entregar
 Cierro este registro

>Log #2 02:30 PM
Analizando la estructura:
El makefile trabaja con las 3 carpetas presentes en e repo.
Mkfs puedo intuir por su contenido que significa makefilesystem. seguramente trabaja con el malloc de el os creado. mejor no tocarlo de momento.
Entre el Kernel y Users, User contiene .c_s y la mayoria son los comandos/ejecutables? que aparecen al usar ls dentro de XV6
Supongo que Makefile los compila al ejecutarse...
En Kernel hayvarios .c_s tambien, pero por sus nombres, parecen mas como configuraciones de la instalacion del OS creado...
No me gustaria meter mano ahi, pero hay uno que se llama syscall.c, y tengo que implementar llamadas al sistema. Registro cerrado

>Addendum #2-1 02:45 PM
Yep... parece que todo funciona en c... tendre que repasar el lenguaje... varios de los otros .c en Kernel tambien se ven relacionados con mi proposito actual. El proximo registro vendra cuando comienze a modificar estos archivos. Cambio y fuera

>Log #3 04:00 PM
Ok, Hay que trabajar en C. No puedo incluir a las Librerias estandar, todo .c del repo se referencia entre si.
El requerimiento `int getppid(void)` Busca el PID del proceso padre. Lei mal y no el Proceso mismo. Ok, esta ves si ando viendo los hints. Existen varias referencias a `getpid()` en el repo... getpid, y sys_getpid, son dos resultados de una busqueda mediante el editor.

Se nombra la funcion en user.h, sin embargo no existe user.c, usys.pl pareciera truncar el nombre de SYS_getpid(), pero es solo una conjetura.... SYS_getid es mencionado en syscall .c y .h, y sysproc.c...
OH!, ya, el que tiene SYS_* no es una funcion, es un alias?struct?variable? En fin, es una categorizacion de un #define, enlaza un numero a la llamada del sistema... y sys_* es una la funcion real, Y esta se define en sysproc.c

~~~c
uint64
sys_getpid(void)
{
  return myproc()->pid;
}
~~~
Esta retorna un uint64 sacado de el contenido, de un puntero, que viene de myproc()...

myproc no esta definida en sysproc.c, pero de las buscando entre los archivos que le nombran, y limitando a los archivos incluidos en la cabeza de sysproc. me indica que proc.c es donde se define la funcion.
~~~c
// Return the current struct proc *, or zero if none.
struct proc*
myproc(void)
{
  push_off();
  struct cpu *c = mycpu();
  struct proc *p = c->proc;
  pop_off();
  return p;
}
~~~
Este retorna pointers de un struc proc o 0.
Parece que hay operaciones de listas por el Push y Pop. y naveba otros structs...
Supongo que este es un buen punto para partir con la implementacion.

De momento, voy a modificar usertests, creando un nuevo test y ver si lo corre dentro de XV6. Se ve perfecto ya que la descripcion dice que correra todos los tests internos, solo debo incluir la nueva funcion a la rutina. Luego crear la funcion get_ppid() dentro.


