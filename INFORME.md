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
>Addendum #3-1 ~~:~~PM
Oh... cambio de planes por informacion nueva...
5

>Log #4 ??:?? ??
Me lei hace unos dias los capitulos del libro hasta la clase 3 o 4, y todo tiene bastante mas sentido

>Log #5 ??:?? ??
El tiempo falta asi que documento ideas y notas para mis siguientes cambios.

Tomando en cuenta que la estructura de un proceso es:
~~~c
// Per-process state
struct proc {
  struct spinlock lock;

  // p->lock must be held when using these:
  enum procstate state;        // Process state
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  int xstate;                  // Exit status to be returned to parent's wait
  int pid;                     // Process ID

  // wait_lock must be held when using this:
  struct proc *parent;         // Parent process

  // these are private to the process, so p->lock need not be held.
  uint64 kstack;               // Virtual address of kernel stack
  uint64 sz;                   // Size of process memory (bytes)
  pagetable_t pagetable;       // User page table
  struct trapframe *trapframe; // data page for trampoline.S
  struct context context;      // swtch() here to run process
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};
~~~
El proceso posee el pionter al proceso padre.
como getpid() basicamente invoca a myproc(), y este invoca al proceso mismo desde la cpu.
El pseudo codigo para getppid() seria, llamar a myproc() y guardar el puntero del struct "proc" "parent"
y luego sacar el pid de ese "proc"
Asimismo, getancestor ejecutaria estos pasos, en un tipo de loop.

El codigo resultante seria
~~~c
// De momento esta es solo el primer draft sin implementar.
int64 sys_getppid(void)
{
  struct proc *father;
  father = myproc()->parent;
  return father->pid
}

// Actualmente es
int64 sys_getppid(void)
  { return myproc()->parent->pid; }
~~~

---
Asumiendo que corrio, tambien se empieza a agregar a las llamadas del sistema, y traptable. y detalles para que pueda ser llamada. Se compila y se si corre

>Log 6 ??:?? ??
Mientras iva anotando las dependencias antes de compilar, leyendo mas atentamente el struct de Proc. ese comentario de "wait_lock must be held when using this" hace creer que no va a correr y que tengo la mitad del puzzle de momento. igual, seguire agregando las referencias y definiciones a syscall, user, usys y demas de momento y segun los resultados. extendere el informe. Si mi sospecha es cierta y se me cae, supongo que tendre que armar algo similar al myproc(), porque segun lo que lei, pop_off() y push_() tienen relacion con Locks
