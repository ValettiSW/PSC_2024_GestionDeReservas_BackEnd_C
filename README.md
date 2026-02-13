Sistema de gestión de reservas desarrollado en C de forma grupal como proyecto sociocomunitario.
Permite almacenar, cargar y administrar reservas utilizando estructuras dinámicas y persistencia en archivos binarios.

El sistema fue orientado a apoyar a un club en el seguimiento y control de las reservas solicitadas para el salón, simulando la lógica de un backend administrativo.

--- Arquitectura del Proyecto ---

PSC_2024_GestionDeReservas_con_biblioteca.c --> Lógica principal y menú interactivo.
Reserva.c --> Implementación de funciones de gestión y manipulación de reservas.
Reserva.h --> Declaraciones de estructuras, prototipos y definiciones compartidas.

--- Objetivos del Proyecto ---

Aplicar estructuras dinámicas en un caso real.
Implementar almacenamiento persistente de datos.
Desarrollar un sistema modular y escalable en C.
Simular el comportamiento de un backend administrativo.

--- Compilación ---

gcc PSC_2024_GestionDeReservas_con_biblioteca.c Reserva.c -o reservas
./reservas
