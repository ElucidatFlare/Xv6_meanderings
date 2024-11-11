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

> Preambulo

Recomiendo saltar al Log 07 donde verdadero progreso se logro para comenzar la tarea.

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

> Log 05

Jugando con un test de como funciona la memoria que venia en las presentaciones, viendo su posiciones
Text en 0x000000.
Data en 0x001000
Heap en 0x013FF0 
Stck en 0x003FBC 

Probando agregar mas variables y datos en memoria muestra cambios en como se comportan
static 1->2 | 0x001004 > 0x001000   
Heap   1->2 | 0x013FF0 > 0x013FB0
Stack  1->2 | 0x003FBC > 0x003FB8
Pareciera que todas parten de un techo, y nuevos datos guardados van retrocediendo hasta un limite.
Lo extraño es que variables para el stack y el heap parecen crecer en a misma direccion (Abajo)
Por lo que el "Maximo" del heap es 0x014000 a 0x004000 donde comienza el stack.
Pero Datos estaticos crecen en la otra direccion, partiendo de 0x001000
Como estos movimientos son en orden de magnitur de 0x1000, 4 digitos hex, son espacios de 12 bits. (Oh!)

Lo cual coincide con definido en riscv.h. PGSHIFT es 12, equicalente la longitud en bits de 0x000, y PGSize es 4096, equivalente a 0x1000. En bits 1 0000 0000 0000.

Ok... entonces, Las paginas se mueven en orden del 4to hex, 
Pg 0 Txt 
Pg 1 Data (Crece desde inicio)
Pg 2 Sin uso (?)
Pg 3 Stack (Decrece desde final)
...
PG 20 Heap (Decrece desde final) (20 = 0x14)
Aunque me queda el misterio de donde se escapan Las direcciones del Stack presentes antes de 0x3FBC
Son... FF0-FBC= 0x034... 0011 0100 ?
Si la memoria parte desde FFFF, El Heap parece guardarse 4 bits para otro propositos?
Pero con Stack... no... Se salta F,E,D,C... No estoy seguro donte se encuentre el resto de esa memoria,
Mmmh... probando como se mueve la direccion con mallocs... Creo me me estoy perdiendo fatal.

> Log 06

Intentemos de nuevo. AAAAAAAH.

Decidi probar como funciona el test de prueba de la clase 10, inverstigar con el de la 8 no me llevo a ningun lado...

Ok... oh! Correr Sbrk me mandaba a la cima de donde se allocaba Stack 0x04000. Woah. Simillar a como Malloc me manda a 0x14000.
...
Ya no se en que iva siquiera viendo como funciona la definicion de malloc y mil otras cosas.
...Ok... umalloc termina llamando a sbrk y eso... es un syscall... y vien de sysproc.c...
Vaya--- Procesos denuevo... Eso es un poco mas familiar... wait.
WAIT.
El OS trabaja todo mediante procesos, y todos los datos que eh tratado de localizar directamente en memoria segun los archivos que tienen funciones que los manejan existen referenciados en procesos. 
kstack, sz, trapframe, pagetables, Estan todos aqui.
Los procesos trabajan con memoria y la memoria contiene el trabajo de procesos y procesos acceden y escriben esta memoria y de memoria se cargan/guardan procesos y la tarea anterior se trabajaba con procesos, y me di 500 vueltas en el codigo fuente de memoria, y creo que mejor salgo a caminar.
... 
Hora de comenzar a analizar de nuevo desde un nuevo comienzo.
Oh, ya que voy a estar con trabajando procesos puedo volver a aprovechar procdump para que me imprima todos los datos relevantes en tiempo real!.
Se... creo que esto seria un mejor metodo que tratar de decifrar en reversa segun como responde llamar memoria haciendo diferentes cosas en el test.

> Log 07

Expandi la funcionalidad de procdump() en proc.c con la funcion definida abajo procdumpmore(), donde ire experimentando con obtener valores/registros que voy a trabajar.

Como pequeña tangente, explore como funciona el printf interno, queria ver si se podria formatear prints, y printear el mismo dato an decimal, hex y binario.
 "%u", "%x", "%lx" y "%llx" se veian interesantes pero resolvi a castear ints, longs, hex a pointer para poder leerlos en Hex, con la separacion de 0xFFFFF
Los uint64 se escribian bien con "%lx" en hex y "%ld" en decimal. Pero no habia forma de que pudiera formatear el tamaño del print para que tuviera un tamaño constante. No hay forma... a menos que me ponga a modificarlo... Mejor sigo con lo que tengo de momento. No mas distracciones.

Pase gran parte del dia para mejorar procdump() para Printear toda la memoria de cada proceso. Voy a ver en detalle Que registros me son de interes,

Para acceder el Memory dump, Simplemente ejecutar Ctrl+p en consola.

> Log 08

Las horas de sueño son minimas, Crei que no encontraria ni alguna idea de como avanzar en este punto. Al parecer la vuelta gigante de Logs 01-06 si me ayudaron un poco, pero tambien casi nada... ahora comprendo... 

Andaba armando un dumper de la Pagetable y PTE, pero creo que finalmente abandono esta en pos de modificar riscv de verdad.

Con gran seguridad (Espero) en riscv.h agregare a la definicion del PTE
~~~
...
#define PTE_RO (1L << 5) // READ ONLY PERMISION
...
~~~
En teoria, no debiera causar problemas, el offset de los PTE es de 10 Bits, y se ocupan solo los bits 5~0. El calculo de PA no es afectado por este y...
No.. Una VA tiene [63-39] De solo 0. (9 bits) Por cada nivel de PTE y son Niveles 2, 1, 0. y luego (12 bits) de offset.
Un PTE tiene 10 bits de offset... equivalentes al tamaño de cada nivel con 1 bit verificador..
Entonces,
VA = [0000...0000] [ PTE lvl 2 | V ] [ PTE lvl 1 | V ] [ PTE lvl 0 | V ] [ OFFSET 12 bits ]
PTE = [  Codigos de PA ] [ PTE siguiente ] [ V ]  y luego con PA...
...
No, Porque PX(,va) retorna el indice de las 512 entradas... Ya me perdi denuevo. Y ya que creia que lo tenia.
Oh no?, Ya que mas da. Me estoy dando vueltas ante el hipotetico crasheo, sin intentarlo.
Asumire que puedo tomar ese bit sin Problema. Y si se rompe todo... A todo sumare +1 a todo numero que se vea relevante. para hacerle espacio.

No Tengo idea de lo que estoy haciendo, no tengo idea donde continuar, no tengo idea como afrontar esto, estoy cansado de seguir el libro/clase/codigofuente funciones variables y que ninguno me lleve a alguna idea de como resolver esto. como voy a proteger la memoria... si nisiquiera estoy seguro que mi dump de la Pagetable esta correcto, ya que no se si son entradas en distintos niveles 2 - 1 o las finales del nivel 0. Como voy a agregar o trabajar con bits, no veo donde poer una rutina que modifique esa entrada, porque no veo una rutina que checkee su estado, fuera del momento de Allocar espacio o Liberarlo. No Leerlo o modificarlo. Tengo que meterme con los diferentes contextos? Algo de eso en la cima de Riscv.h  que se ve esoterico? Machine Status Surpervisor Status. Exceptions y Interrupt Handlers, y todo en eso de asm volatile que no entiendo. Yo.. yo... 

Ya fue, eh estado toda la semana siquiera queriendo entender el problema, pero ni si quiera puedo empezar a imaginar donde integrar codigo.

VM.c? kalloc.c? memlayout.c? riscv.h?, proc.c? exex.c? pipe.c? umalloc.c? start.c ? ando dando vueltas por todo este codigo fuente, y nada tiene sentido...

En algunos momentos tenia la idea de armar un struct con un lock y simplemente que si se llamaba tirara un error. Pero un Lock solo se me ocurre que viva como programa/proceso, y no como una marca en la paginacion y que el sistema lea esa paginacion, y que tire el error de ahi. no me sale, no se me ocurre, no se puede. nose puede. Nose asfasdfasdfsadfsdfsadfsdfsdafsdafsdafsadfsdafsdafsdafsdfsdafsadsfdafasdfsdafsdafasdsfdfsdafsdfsadfsadfasdfsadfasdfsdafsdafsadfsdafsdfasdfasdfsadfsdaafsdfsafsdafsad.
