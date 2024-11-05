# Tarea 3: Protección de Memoria en xv6

## Objetivo

Modificar xv6 para implementar un sistema de proteccion de memoria que permita marcar regiones de memoria con solo lectura

## Modificaciones Requeridas

 1. Incorporar Funciones de Proteccion
    - Implementar las funciones mprotect y munprotect
 2. Modificacion de la Tabla de Paginas
    - Actualiar la tabla de Paginas para incluir un bit que indique si una pagina es de solo lectura

 - [ ] *mprotect(void addr, int len)
    - Permite MArcar una region de memoria comenzando en addr con longintud len, como solo lectura
    - Calcular cuantas paginas estan involucradar y moificar el bit W de cada PTE para deshabilitar la escritura.
 - [ ] *munprotect(void addr, int len)
    - Permite revertir la proteccion de solo lectura de una region, volviendola de nuevo lectura/escritura
    - Similar a mprotect, per restaura el permiso de escritura para las paginas afectadas

## Manejo de errores
 - Las funciones deben manejar errores
    - Direcciones invalidas para addr o len
    - Direccones que no pertenecen al proceso
    - Otros errores deducibles

## Entrega
    - Codigo fuente + Tests
    - Informe README.md : Funcionamiento y logica de la proteccion de memoria, explicando las modificaciones realizadas.
    - Dificultades Encontradas y soluciones implementadas.

# Informe de avanze
---

> Log 01

Investigacion del codigo fuente y clases pasadas. Hay optimismo de conseguir el 7 en esta entrega. Memlayout se ve como un buen punto de partida.

Upd8: Wow... Cuantas capas de abstraccion hay que manejar. Pero creo que entiendo algo.

> Log 02

Despues de reevaluar como comenzar, busque info de como se manejan la memoria y las paginas-
Mediante defs.h, encuentro que pagetables se usan en proc.c y vm.c
De estos, 
El primero en las funciones `proc_mapstacks, proc_pagetable, proc_free pagetable`
Mientras que el segundo esta Todas tienen relacion a el uso de estos.
Con la distincion de Memoria `K`ernel y de `U`suario, y `V`irtual con `P`hisica

Si quiero agregar ese bit de protecion en memoria, este se ve como un buen lugar para empezar-

Catalogando las llamadas dentro de las definiciones, mejor anoto la secuencia de como se llaman entre si? Trato de seguirlo con papel y ala vista y me estoy perdiendo
~~~
#   Funcion  | Llama A | Funciones
01  kvmmake()  -> kalloc(), memset(), kvmmap()*6, proc_mapstacks
02  kvminit()  -> kvmmake() 01
03  kvminithart-> sfence_vma, w_satp, sfence_vma   // No se ve muy relevante
04  walk       -> PX, PTE_V, PTE2PA, kalloc, memset, PA2PTE (???) // Llama a funciones en riscv.h Suena importante
05  walkaddr   -> ...Similar a walk. Con un VirtualA Walk retorna PTEntry Address, walkadd es el PhisicalA
06  kvmmap()   -> mappages() 07
07  mappages() -> walk() y varias de las funciones internas... empiezo a ver un patron
08  uvmunmap() -> Similar, con un kfree() condicional
09  uvmcreate()-> kalloc, memset
10  uvmfisrt() -> kallocm, memset, mappages, memmove
11  uvmalloc() -> Similar, con uvmdealloc() 12 en ocaciones
12  uvmdealloc -> PGROUNDUP() // Tambien es de Riscv.h
13  freewalk() -> risch.v and kfree()...
14  uvmfree    -> uvmunmap, freewalk
...   Creo que no gano mucho en este punto de redactar.
~~~
Comenze a graficar esto cuando leida Kvmmap() viendo que se conectava con kvmake y kvinit. Quizas me pase.
Segun entiendo...
Las funciones KV y UV, terminan requiriendo memset y mappages()
mappages()  requiere Walk()
Y al final hay funciones para Copiar strings desde y hacia Kernel Memory y User Memory.

Dentro se usan funciones de Riscv.h , kalloc.c (Kalloc, kfree) y string.c (memset)
Segun parece Riscv contiene definidas varias constantes y metodos que se utilizan aca. Siendo el manejo de Vm un proceso mas elevado en abstraccion que en Riscv.h, O eso asumo de momento.

... 
Ademas... creo que el mayor misterio asta ahora es que es uart y hart... Lo veo mencionado en detalles, pero no me suena para nada.. Quizas lo investigue despues.

> Log 03

Riscv.h Is big... 
- It seems Hart is a CPU core...
- It works with more low level instruccions, there are asm's assembly I suppose
- Things about Supervisors and the Machine...
- Y al final, Las definiciones que vi en vm.c
Son Constantes y funciones con Valores que requieren operaciones binarias como BitShifting para redactarse
...
Las entradas de Pagetables son uint64. y la Tabla de paginas es un array de uint64...
Segun veo, VA, PA, y SZ son uint64, con sz como size de la memoria o de algo mas... Me pierdo tratando de ver la logica en su uso.

Las entradas en la tabla tienen reservadas 5 bits en las definiciones PTE_#
V - Validar
R - Read
W - Write
X - Excecute
U - User Accesible 
Tomando los Primeros?Ultimos Bits del uint. De momento no vere la Indianidad y movimiento de los shifteos en profundidad.

Y "V" Es solo asignado cuando la Entrada es creada en mappages()...

Wait... La tarea me pide agregar un bit para Read Only... Pero ya existe un Bit para Read.
Solo que Puede Pasar que R y W esten activos simultaneamente... Sera mi version de RISCV la correcta?...
Anyways... Supongo que podria ver como agregar un 6 Bit como flag... Pero tendria que ver que esto no se cargue otros procesos en la Tabla. 
...
Creo que tanta informacion me esta saturando...
Nota para revisar despues: 
- Evaluar si en verdad necesito un flag extra, o trabajar segun el caso W=0, R=1. (Los Flags pueden cambiar en runtime?)
- Revisar las definiciones en memlayout.h, ya que en kvmake se llaman varios datos de ahi, y... me perdi mucho mas.

