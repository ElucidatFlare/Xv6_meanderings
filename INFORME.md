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

> Log 02

Aparentemente hay 2 tipos de Inodos. En fs.h "dinode" Es el nodo guardado en disco, mientras que en file.h el struct de "inode" se define segun como se guarda en memoria.
Segun veo, "inode" contiene todos los valores de "dinode" mas unos campos agregados.
Por un momento, como dice agregar bits para los permisos crei que caeria en el foso de manipular bits que cai en la tarea anterior, pero viendo que las instrucciones piden campom creo que mi primer cambio sera agregar una variable tanto en "dinode" y "inode" llamado "perms".

Me gustaba el balance que tenian de ocupar exactamente 4 bytes el struct, ojala que aumentar el tamaño agregando la variable no desestabilize algo en otra parte del sistema.

~~~
struc dinode { // Mismo cambio en "struct inode {"
   ...
   uint perms;
   ...
}
~~~
...

Wait. En File.h el struct de file tiene los campos "char readable, char writable"
...
Pero la tarea pide los permisos en el inode, no en el file...
Creo que continuare, pero tomare en cuenta esto en lo que sigue.

> Log 03

Como siguiente paso seria integrar la inicializacion del campo.
Busque entre file.c y fs.c por alguna funcion que creara inodos.

"iinit" parecia ser un poco previo a lo que buscaba, creando una tabla de inodos y sus candados

"ialloc" se ve mas pertinente, inicializando "dinodes" y finalizando con "iget" el cual carga en memoria el "inode" a partir de el "dinode"
Por lo que le agregue el init del campo a 3 como se pide.
~~~
ialloc(uint dev, short type){
   ...
      dip->type = type;
      dip->perm = 3;   // ★★★ Init perms
   ...
}
~~~

A primera vista pase si alguna funcion podria necesitar tambien agregar algo respecto a esto. Al parecer no hay un "ifree", donde los valores sean marcados como 0.
Lo mas similar a esto era "iput", por lo que tambien le integre el cambio de los permisos a 3. Porque pensando a futuro me pregunto que pasaria si se aplica put a un archivo con perm 5 y luego al allocarlo denuevo no me deja. No lo estoy pensando muy a fondo, pero por si acaso.

Tambien, vi pertinente agregar un pequeño cambio en "iupdate"
Ya que parece modificar todo el "dinode" segun lo leido en el "inode". Y los campos que posee se ven relevantes.
~~~
iupdate(struct inode *ip){
   ...
      dip->perm = ip->perm;       // ★★★ Update perms
   ...
}
~~~
Me parece que esta funcion podria ser clave despues cuando me ponga a integrar chmod. quizas.
Pero ya vere.

Por ultimo "stati" parece llamar varios valores de los inodos, pero no se en que se utilize, y mejor me concentro en el desarrollo actual. No se que hara el struct stat de "stat-h" aun.

> Log 04

I was checking the Read Write implementations, but The compiling crashed. Testing by removing parts, the culprits seems to ve the permission added in "dinode", porque en mkfs se hace un test sobre el tamaño en disco...
Creo que mi temor de romper el balance de 4 bytes en el tamaño del struct si era justificado
Actualmente sin permiso su sizeof() es 64, y con tenia 68. El punto de fallo es un modulo con "BSIZE" y eso es 1024

Me hubiera encantado que esto fuera bonito y facil, PERO NOOOOOO, bueno, en ves de complicarme la vida y ver como optimizar bits de espacio, simplemente duplique el tamaño de los dinodes con una variable uint64 pad[7], que esta ahi solamente para que el tamaño permita compilar.
Y como no quiero mas problemas asi con inode tambien, ahora este tambien tiene el padding.

Solucionado.

[Rant Redacted]

> Log 05

Despues de un sufrir un indeterminado periodo de tiempo, resumi y decidi reenfocar por donde continuar... Mejor dejo tranquilo lo que no se ve roto y muevo a otra parte.
Buscando soluciones, parece que entre el camino de continuar modificando fs.c no me llevara muy lejos, asi que vere si file.c tiene mejores resultados.
O mejor reviso como agregar chmod?...
Sem aunque me falle el resto, debieran ser puntos que valen la pena

> Log 06

Nah, no creo que lo logre.

Si me disculpan, me ire a otro lado a recuperar mi psiquis, y mañana continuare con otras entregas... Esto no acaba. 