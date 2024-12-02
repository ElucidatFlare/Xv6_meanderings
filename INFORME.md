# Reporte de progreso en la Tarea 5
Entrega por - A_V

## Tarea 5:
La tarea consiste en implementar un sistema simple de comunicación entre procesos (IPC) en xv6, utilizando una cola de mensajes dentro del kernel. A continuación, se desglosan los pasos en secciones, asignándoles puntaje basado en su complejidad y relevancia para el objetivo general.

### Objetivo

1.⁠ ⁠(0.5 puntos) Crear una estructura de datos para los mensajes. Puede ser algo simple, como un array de char de tamaño fijo, o una estructura más compleja que incluya metadatos como el tamaño del mensaje o un identificador del remitente:

    typedef struct message {
        int sender_pid;
        char content[128];
    } message;
2.⁠ ⁠(0.5 puntos) Implementar la cola de mensajes como una estructura global en el kernel

3.⁠ ⁠(0.5 puntos) Implementar la llamada al sistema sys_send

3.⁠ ⁠(1.5 puntos) Implementar la llamada al sistema sys_receive

Usar las funciones sleep y wakeup ya disponibles en xv6.
Despertar al proceso bloqueado cuando se agregue un nuevo mensaje a la cola.
3.⁠ ⁠(1 punto) Usar un mecanismo de sincronización como un spinlock para evitar que múltiples procesos accedan a la cola simultáneamente.

3.⁠ ⁠(1.5 puntos) Crear un programa que haga un fork para generar dos procesos:
Proceso escritor: Llame repetidamente a sys_send para enviar mensajes.
Proceso lector: Llame a sys_receive para recibir y mostrar mensajes.


Validar el comportamiento:

Asegurar que los mensajes enviados sean correctamente recibidos en orden.
Comprobar que el lector se bloquee si no hay mensajes y que se despierte cuando lleguen nuevos.

### Desarrollo

Ya no puedo mas... :c

Veamos, que se puede hacer 

Colas de mensaje, entonces defino en alguna parte el struct, lo hago global para que todos los procesos tengan acceso, armo sys_send y sys_receave...

Tambien agregarlos como llamadas de sistema... ok, a buscar en las clases sobre esto...

Agregue SYS_send/receive a las llamadas del systema por ahora.
A la rapida, modificando Syscall.h .c user.h usys.pl. y sysproc.c

Similar como en la tarea 1... Lo unico que supe hacer...
Me eche la tarea 3 y 4, y ahora hago esta a corto de tiempo... quizas no meresco pasar el ramo, quisar debiera recursarlo... pero no puedo rendirme aqui ni ahora... ando con el tiempo justo para el calendario del proximo año... y todavia queda el examen... ok... animo... ...


Como no quiero pensar en posibles caidas y errores de meter manos en donde no tuve exito en el pasado. Me centrare en agregar todo a proc.c y .h


~~Al fondo de proc.h defino la estructura~~
Por seguridad, defino los mensajes como se pide en Types.h
~~~c
typedef struct message {
    int sender_pid;
    char content[128];
} message;
~~~

Ok, Item 1: completado,
Items 3 & 4: En progreso;

Ahora... colas, primero que entra, primero en salir, y que mantenga los mensajes..

Sys_send/receive

Tomando ideas de la estructuras presentes en umalloc.c armamos una struct de queues, y agregamos la cabeza del queue de forma global, todo esto lo agregare en proc.c

~~~c
struct msgQueue {
  struct msgQueue *next;
  message *msg;  
};

typedef struct msgQueue MSG;

static MSG base;
static MSG *latest;
~~~
El struct msgQueue Contiene el pointer al mensaje y un pointer al siguente mensaje en la lista,

"base" se guarda en memoria global, y es donde se tiene guardada el inicio de la Queue
"latest" es un puntero al ultimo elemento de la queue, me base en lo que habia en umalloc.c y si bien se que sus intricancias de porque está o no está pueden ser diferentes.~~(Quisas para Pilas/Colas Bidirecionales?)~~ Lo mantendre por la prisa.
Addendum: "Latest" es el puntero al final de la cola, donde se guardan los nuevos Elementos en la cola.
---
Tratando de compilar, rectifique las llamadas de sistema de send/receive en syscall.c
Para que en el momento creen un uint64, que tiene mismo tamaño de un pointer, lea el argumento de la llamada y luego castee la funciones send() receive() que estoy elaborando en proc.c
Dentro de estas, casteo el pointer al tipo de dato que elaboro la funcion para que compile el codigo.
~~~c
uint64
sys_send(void)
{
  uint64 msg;
  argaddr(0, &msg);
  return send((message *)msg);
}

uint64
sys_receive(void)
{
  uint64 buffer;
  argaddr(0, &buffer);
  return receive( (char *)buffer);
}
~~~
Pudo haber mejores formas, pero ando con prisa.
---
Ahora, Andaba definiendo en proc.c send() y receive(), agregue su nombramiento en defs.h
Y de momento son:

~~~c
int send(message *msg){ //El plan es que ambas retornen 1 en exito, y 0 en error
  MSG *sender;          
  sender = &baseMsg; //Hubiera preferido dejar todo vacio con un return 1, pero
  sender->msg = msg; //El compilador me obliga a usar en algun punto baseMsg de momento.
  return 1;
}

int receive(char *buffer){
//  MSG *receiver;
//  receiver = &baseMsg;
  buffer = baseMsg.msg->content;
  return 1;
}
~~~
Addendum 2: agregue el sufijo "Msg" a "base" y "latest"  con miedo a que haya una sobreescritura en la memoria global con umalloc.c
Sin embargo, este es tan solo la funcion "Vacia", solo con lo nesesario para que me compile xv6 sin ponerme warning/error.

El plan ahora es:
Como en sys_send() requiere el message. Para ser enviado a la cola
Y       sys_receive() el buffer, pero todas las definiciones de syscalls que veo son definidas en void con extraccion en funciones tipo arg_type, es que las llamadas del sistema llamen a los metodos definidos fuera de esta.

Entonces. Send y receive son mi prioridad ahora.


Si "send" un Mensaje, entonces. el plan es
* Adquirir candado de la cola
* Allocar un MSG para la cola?. (Debiera usar malloc en el MSG? si es que lo guardare en Static?)
   * Revisar la mejor forma de inicializar MSG, pero usarlo finalmente.
* Anotar el Mensage en el MSG;
* Anotar el MSG en "Latest". Asegurarse de que si la cola esta vacia, "Latest" y "Base" sean iguales.
* Retornar 1 en exito.
* Abrir el lock de la cola

Y Con "Receive" entonces:
* Lockear
* Buscar el MSG de la "Base"
* Guardar en el Buffer el mensaje
* Mover la "Base" al siguiente MSG* del struct. (Practicamente borrando la antigua base (Debiera abrir una variable temporal y hacerle free quizas?. A ver de que forma compila y que aplico en send primero mejor...))
* Un Condicional de que Pasa si la Base y Latest son iguales, o sea que no hay mas mensajes o queda 1 Mensaje
   * Marcar como vacia la cola?...
* Un Condicional de que pasa si no hay Mensajes.

...

Pero de momento mi implementacion no tiene forma de comprobar si es que hay 1 o 0 Mensajes...
Ah!
Debiera crear una rutina, que al Crear "MSG"s, *next sea 0. Y que cuando el "Base" Este "Vacia", *"Mesage" tambien apunta a 0.
Asi, en el caso que Base == Latest, Y quede 1 Mensaje, se identifica si Message != 0. Y se Manda eso.
Y si no hay mensajes, identifica que MSG es 0.
...
Solo faltaria un detalle que no eh redactado, pero eh tenido en mente.
Una funcion extra que Cree el objeto de Message. Algo como "envelope()"- 
Esperen un momento... Lo de Syscalls, son las funciones que podra usar el usuario.
Los Sys_func() se convienten en func() en user.h, y son la interfaz del usuario a las llamadas,
Pero la tarea en la presentacion requiere sys_send(message * msg)...
Pero por otro lado ya defini sys_send(void) con el fin de mantener la harmonia de las otras definiciones de SYS_calls()...
Supongo... mejor empiezo a tomar desiciones.
---
Comparando el funcionamiento de otras llamadas de sistemas.
Send() desde el lado de usuario solamente recibira un string.
Y dentro de el syscall se creara el Mensage con el ID, del remitente y el string.
Y esto se pasara al Send() de proc.c

Voy a crear envelope() y modificare send del lado del usuario.
Primero, en user.h la funcion disponible al usuario es.
~~~c
int send(const char*);
int receive(const char*);
~~~
----
En el proceso de Definir envelope, me tope con afrontar el tema de como guardaria el mensaje.
Iva a usar malloc() Pero espues de considerar todo las vueltas de antiguas tareas, acudi a comprobar como otras partes de xv6 allocan memoria.
Sin embargo, El malloc disponible al usuario, es el que viene de umalloc.c,  y fuera de este, no existen malloc()s en uso a nivel kernel, por lo que podria causarme problemas, me doy cuenta.
Si voy a allocar memoria.. y ante las 5 horas restantes, y si va a ser siguiendo las restricciones dentro de xv6
Voy a simplemente Modificar El sistema MSG queue que tenia en mente.

Porque mi fuente era la Queue de umalloc. Y Esta misma trabaja con la memoria de Sistema operativo, y rompe nada, pero es el unico punto de uso de memory alocation, no quiero poner un punto de quiebre con mi codigo metiendo mano en el dominio de ese sistema.

Nuevo plan, siguiendo otros "Allocs" del sistema, Creare un Array estatico.
message es char[128] y un int id... eso es 128*1 byte + 4 bytes = 132 Bytes,
y si es global y tiene que estar en Memoria kernel, debo conservar cuanta memoria pueda, creo, no es infinita...
Con 7 Mensajes estaticos ya casi lleno 1 KB:  (132*7)/1024 = 0.90234375
Por lo que-
---
[ Perdia tiempo escribiendo cada vuelta que doy ]
Actualmente, tuve que botar gran parte de la implementacion actual,
Debido a que mi idea era con una cola dinamica, pero al ver los requerimientos del manejo de memoria, cambiar todo a arrays estaticos, me causo reestructurar casi todo.

Al momento de redactar, logro inicializar la cola, con valores "0" en contenido, y pid -1
Con un correo extra que hace de placeholder de correos a enviar, fuera de la cola misma..

Reportare nuevos avanzes luego.

Ok...

Existe el struct de Mensajes.
Existen send y receive en sistema,
dentro de proc.c estan
send(), receive(buffer), envelope(msg) y queueInit()
Envia  la carta- Recibe la carta- Envuelve la carta en el sobre - Y el que inicializa la estructura de cola y mensajes.
Tambien dentro de las funciones, hay candados, para evitar problemas.

De momento- Tire el inicializador en procinit(), para que corriera con el boot del sistema.

El sistema Compila. Y es hora de ver si puedo crear un programa test, para probar las llamadas del sistema-
Pero lo importante es que esto es Entregable de momento. 

ToT

----

El test de prueba se llamara "try"
Por la facilidad de escribirlo con rapidez y poder compilar y retestear con alta velocidad.
Para correr escribir "tri" en la consola una vez compilado xv6

----

Entonces..., Hay que crear un programa que haga fork, que hallan 2 procesos y con sleep awake esperar a que se pasen mensajes. Interesante.

Logre correr Hello world.
Ando probano las funciones.
Puedo crear un buffer, puedo enviar mensajes
Pero de momento no puedo recibir el mensaje, y no se porque. D':

Creo que es porque el buffer esta malloqueado en el programa y el buffer enviado usa esa direccion virtual dentro del kernel.
Ni idea como podria mandar memoria de ese tamaño entre kernel y user... Y queda 1 hora...

Si voy a tener que meter manos a ultima hora con memoria.
Este es mi envio semidefinitivo, previo a romper todo cambiando como funciona receive.
