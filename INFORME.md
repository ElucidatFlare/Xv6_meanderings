# Reporte de progreso en la Tarea 0 y sus resultados.

Entrega por - A_V

## Tarea 0: Instalación y Ejecución de xv6

### Objetivo
Instalar xv6, demostrar que está funcionando correctamente en tu computadora, y compartir tu trabajo en un repositorio de GitHub.


1. Clonar el Repositorio
2. Crear una Nueva Rama
3. Instalar Dependencias
4. Compilar xv6
5. Ejecutar xv6
6. Verificar la Instalacion
7. Documentar la Instalacion
8. Captura de Pantalla
9. Commit y Push
10. Compartir en Github

### Proceso

#### Prologo

Todo comenzo con la instalacion de Ubuntu,
Ah si, el viejo ubuntu, crei que ya no tendria que toparme con instalar linux en mi maquina personal, pero aqui estamos denuevo. Han pasado años desde que cruze caminos con aquel OS, y sabia que andaria dando vueltas por las rafagas de problemas y cambios que ese pedazo de codigo de 5 condados mas alla de la llanura me traeria.

Debo decir que estaba en lo correcto. Pero ya me conozco una que otra de sus triquerias, y no era mi primer rodeo, pero mejor dejo de ruminar en las peripecias que salieron antes de partir con la tarea misma. Y voy al relato de la resolucion del trabajo asignado

#### Los primeros avances

Una ves instalado el OS, actualizado software, configurado partes del OS, y leido un poco de como funcionaba Linux. El primer intento de progresar fue ponerse a tratar de avanzar, segun lo Aparecia en la PPT1 y lo escrito en el servidor de discord.

En esta etapa mis acciones fueron:
- Abrir varias pestañas con links random de repos 
    que aparentemente Necesitaria
- Descargarlas en un zip formato zip. y organizar 
un directorio con los archivos
- Investigar mas comandos de linux y tratar de Instalar todo

Segun el PPT habria que instalar:
VSCODE, Git, qemu (riscv64-softmmu), Riscv toolchan y algo llamado XV6

 - VSCode: No hubo grandes complicaciones
 - Git: No hubo complicaciones, En instalar...

Y el resto..., 

#### La experiencia Linux tm

En estos primeros intentos, iva principalmente guiado por post sueltos, y tratando de resolverlo todo una cosa a la vez.

 - Qemu: En mi primer intento de instalarlo, Fue buscar comandos de instalacion de diversar fuentes.
    - Stackoverflow, posts y discuciones, la pagina oficial, pero todo diferia en la respuesta y/o no resultaba. Gravemente con la falta familiaridad con todos los quirks y mañas que hay en todo lo que se hace con linux.
    Aparentemente instalacion mediante APT, ya no aceptaba los comandos para instalar qemu, ya que lo separaron en paquetes mas pequeños, y solo se instalaria con el nombre de paquete preciso, para el uso preciso, para la aplicacion precisa...
    * Aplique el "qemu-systems" pero como que no importa mucho gracias a pasos futuros

 - XV6, Riscv64 y Riscv Toolchain:
    - Llamenme Capitan Ahab, ya que aqui vino mi ballena.
    - Al igual que con Qemu, la variedad de instructivos contradictorios era basta. y tomar paso a paso comandos de "configs", "install" y "Makes" fueron donde recayo la mayor de mis vueltas y pesares.
    - Pero parte de estos problemas viendo en retrospectiva, es que todo lo que tiene que ver con computadores es complicado y es parte del proceso...
#### Instalando las dependencias

   Como XV& necesitaria al Toolchain, y esa era la pieza que mas se omentaba en el servidor, decidi partir por esa.

   La mayoria de las guias siempre pedian
   - Clona un repo
   - Crear carpetas, mover directorios y todo en terminal
   - Sudo install muchas cosas
   Y los pasos de compilar...
   Como gajes del oficio de este paso, al parecer el comando curl si no se instala con el comando correcto? Tambien, como Python3 ah reemplazado a Python2, tuve que buscar como instalar un paquete externo, porque aparentemente en una de las compilaciones que intente, se rompia porque un programa al llamar a python pedia con un comando deprecado?. Eh... bueno, me marea tratar de comprenderlo, pero instalando paquetes de los que no se nada a ciegas, eventualmente ese tema se arreglo.
   - Fuera de eso, efectivamente, instalar dependencias se resumio a usar:

   `sudo apt-get install <Paquetes pedidos en documentos de instalacion>`

#### Compilar y ejecutar: (El toolchain)

   - Multiples horas de compilacion para algo que podria si o no funcionar

Hubieron multiples intentos. tengo como 5 carpetas con repos de diferentes paginas, de instrucciones que se frenaron por diversos motivos. 

#### Git:
   Abandonamos el paso anterior porque en todo esto lo eh hecho localmente, y hasta ese punto no habia visto el documento T0.md
# :upside_down_face: 
 - Se clono el repo a una carpeta local,
 - se entro a la carpeta y comenzo a trabajar con vscode
 - Configure git.config con la cuenta de github, el token personal
 - Comenze una tangente que tardo mucho para recordar como funciona git
 - cree una repo en mi cuenta personal, y la deje vacia
 - Cree una nueva branch
 - Se cambio el repositorio remoto del git en la carpeta clonada
    - desde la duente del repo original, hacia el repo personal
 - Le di triple confirmacion de hacer push no lo mandaria al repo original
 - Mande el Push al repo personal
 
 Despues de eso reviso lo pedido y me doy cuenta que no quedo como fork.
 pero quedara como "Todo" para mas adelante.

 ##### Link del repositorio

    https://github.com/ElucidatFlare/Xv6_meanderings

 #### RE: Compilar y ejecutar:

Entre varios intentos de compilar y ejecutar el toolchain con diversos grados de exito, 

Al combinar los comandos de la guia que mas me resulto, junto con leer en extremo detenimiento el instructivo en la guia del XV6 pedido en el repo de la tarea 0.

Finalmente comprendi.

Comprendi que hacia en detalle el

    ./configure --prefix=something

El:

    make something

Y que estaba haciendo mal hasta aquel momento.

En una de las multiples carpetas donde trataba de instalar el toolchain, de uno de los recursos disponibles,

Uno utilizaba un par de scripts.sh para la parte de instalar dependencias y compilar. Fue el que siguiendo la misma guia me llevo mas lejos, pero no corria con el "make qemu" de la tarea. Pero si compilaba test cases incorporados, y comandos que llamaban al programa existian.

(Como adendum, en algun punto, de experimentar y resolver todo esto, tambien estuvo el proceso de agregar definitivamente el camino de el toolchain "instalado/compilado" a las variables de entorno {$PATH}, pero no lo puedo fijar a merito de su propia seccion)

La gran diferencia, aquel compilaba a un formato diferente, algo relacionado con 32bits con mas parametros,  y otros detalles.

Con esto, comparando con las "Instrucciones" del GNU-toolchain oficial,  finalmente comprendi, que aquella no funcionaba porque estaba
 - Instalando Toolchain (LINUX), o installando Toolchain (32Bits) y otras arquitecturas...

Y otras variedades a lo largo de mis intentos anteriores.

Leyendo por enesima vez el README de xv6-riscv entre la minuscula monoespaciada fuente de las instrucciones...

Pide: "NewLib", y "Riscv64-softmmu"...

Claro, si uno va a por la toolchain y  va a instalar, uno asume que debe tomar la que dice (LINUX)...

...

Compile la repo del toolchain oficial en una carpeta de facil acceso.
Estaves solo con "Make" y no "Make linux", ni otro aparataje de una guia externa...

Agrege la carpeta al entorno.

Volvi al dir con la tarea y el repo. 

"Make Qemu"

"Compilo"

#### Verificar la Instalacion y captura de pantalla

[Imagen adjunta con la entrega]

[//]: # "Podria Usar el linking de Markdown, pero averiguar si se mantendria su funcionalidad fuera de mi visualizador y en el de quien este corrigiendo suena mucho para algo que ya ah llevado tanto tiempo lograr"

#### Documentar la instalacion

Escribo este documento...

No se, hace unos dias que complete la compilacion y ejecucion, pero la parte de github faltaba completar, y redactar este MARKDOWN tambien me tomaria mas tiempo.

Estas es la recapitulacion a posteriori despues de lograr completar todo, y ando dudando de la valides o rigurosidad de este documento ya que primero me enfoque mas en instalar las cosas, Sin tener idea porque nada funcionaba.

Supongo que redactar el viaje en paralelo hubiera entregado un documento mas ordenado y riguroso, Pero mas extenso, ya que hubiera puesto en detalle todas las tangentes que surgieron.

En fin. 
Si bien fue la Tarea 0
Y la estoy entregando atrasada
Y no me siento orgulloso de como esta redactada. 
Puedo ver atras y apreciar que tuve que meterme en lleno a como usar linux, la terminal, manejarme en la estructura de carpetas de linux, debuguear varios problemas, configurar el git y la terminal, y todo para poder finalmente correr el xv6


Bueno. Termino de redactar este documento, voy a ver que contiene T1.md

