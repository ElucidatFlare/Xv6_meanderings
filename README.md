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
 - [ *munprotect(void addr, int len)
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