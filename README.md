# Reporte de progreso en la Tarea 2 y sus resultados.


## Tarea 2: 

### Objetivo
Modificar el programador de procesos en xv6 para implementar un sistema
de prioridades.

## Parte I: Incorporar Prioridad

### Agragar un cambio de prioridad

### Inicializar prioridad en 0

## Parte II: Incorporar Boost

### Agragar un cambio de prioridad

### Inicializar en 1

## Consejos (Hints)

 * Revizar el codigo existente en Xv6
 * Documentar las modificaciones

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