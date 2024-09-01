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
Yep... parece que todo funciona en c... tendre que repasar el lenguaje... varios de los otros .c en Kernel tambien se ven relacionados con mi proposito actual. El proximo registro vendra cuando comienze a modificar estos archivos.
