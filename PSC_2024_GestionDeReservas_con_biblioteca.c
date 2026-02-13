/*
Gestión de Reservas

Prácticas Socio-Comunitarias 2024

Introducción a la Computación y Programación II

Sistema operativo donde se compiló: Windows 10

Versión de gcc: gcc (MinGW.org GCC-6.3.0-1) 6.3.0

gcc (MinGW.org GCC-6.3.0-1) 6.3.0
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Fecha: 11/12/2024

*/

#include "Reserva.h"
#include <stdio.h>
#include <locale.h>

// Léxico global
TReserva res;
TFecha fecha1, fecha2;
int opcion, numres;
char sn;
TLista lista;

// Nombres archivos guardados en variables
char archivoBaseDeDatos[150] = "BaseDeReservas.dat";
char archivoTemporal[150] = "temp.dat";

// Algoritmo principal
int main () {
    setlocale(LC_ALL, "es_ES.UTF-8");  // Asegura que la localización sea compatible con UTF-8
    int c;

    // Abre el archivo BaseDeReservas.dat y lo soporta sobre una lista
    CargarArchivoEnLSE (archivoBaseDeDatos, &lista);

    // Menú de opciones
    do {
        printf ("\nSistema de Gestión de Reservas del Club Juan Bautista Alberdi");
        printf ("\n\nMenú de opciones principales: ");
        printf ("\n1- Ingresar una reserva");
        printf ("\n2- Buscar una reserva");
        printf ("\n3- Modificar una reserva");
        printf ("\n4- Eliminar una reserva");
        printf ("\n5- Listar eventos entre dos fechas");
        printf ("\n6- Listar disponibilidad del salón para un día");
        printf ("\n7- Listar ingresos entre dos fechas");
        printf ("\n8- Salir");
        printf ("\n\nIngrese la opción: ");
        while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 8) { // hasta no ingresar una opción entre 1 y 8 no sale del ciclo.
            printf("\n Entrada no válida. Por favor, ingrese una opción entre 1 y 8.\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);
        printf ("\n");
        if (opcion == 1) {
            IngresarReserva (&lista);
        }
        else if (opcion == 2) {
            BuscarReserva (lista);
        }
        else if (opcion == 3) {
            printf ("-Ingrese el número de reserva de la reserva a modificar: ");
            while (scanf("%d", &numres) != 1 || numres < 1) { // hasta no ingresar un número de reserva positivo no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese un número de reserva válido.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);
            // Modificar la reserva
            ModificarReserva (numres, lista);
        }
        else if (opcion == 4) {
            printf ("-Ingrese el número de reserva de la reserva a eliminar: ");
            while (scanf("%d", &numres) != 1 || numres < 1) { // hasta no ingresar un número de reserva positivo no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese un número de reserva válido.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);
            // Eliminar la reserva
            EliminarReserva (numres, &lista);
        }
        else if (opcion == 5) {
            // Carga de la fecha inicial
            do {
                printf ("Ingrese la fecha inicial (dd/mm/aaaa): ");
                while (scanf("%2d/%2d/%4d", &fecha1.dia, &fecha1.mes, &fecha1.anio) != 3 || !ValidarFecha(fecha1)) { // hasta no ingresar una fecha válida no sale del ciclo.
                    printf("Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.        
                }
                while ((c = getchar()) != '\n' && c != EOF);
                // Carga de la fecha final
                printf ("Ingrese la fecha final (dd/mm/aaaa): ");
                while (scanf("%2d/%2d/%4d", &fecha2.dia, &fecha2.mes, &fecha2.anio) != 3 || !ValidarFecha(fecha2)) { // hasta no ingresar una fecha válida no sale del ciclo.
                    printf("Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.        
                }
                while ((c = getchar()) != '\n' && c != EOF);
            } while (!(EsMayorFechaSinHora(fecha2, fecha1) && !(EsIgualFecha(fecha2, fecha1)))); // repetir hasta que la fecha2 sea >= que la fecha1
            ListarEventosEntreFechas (fecha1, fecha2, lista);
        }
        else if (opcion == 6) {
            // Carga de la fecha a consultar
            printf ("Ingrese la fecha a consultar (dd/mm/aaaa): ");
            while (scanf("%2d/%2d/%4d", &fecha1.dia, &fecha1.mes, &fecha1.anio) != 3 || !ValidarFecha(fecha1)) { // hasta no ingresar una fecha válida no sale del ciclo.
                printf("Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.        
            }
            while ((c = getchar()) != '\n' && c != EOF);
            ListarDisponibilidadDia (fecha1, lista);
        }
        else if (opcion == 7) {
            // Carga de la fecha inicial
            do {
                printf ("Ingrese la fecha inicial (dd/mm/aaaa): ");
                while (scanf("%2d/%2d/%4d", &fecha1.dia, &fecha1.mes, &fecha1.anio) != 3 || !ValidarFecha(fecha1)) { // hasta no ingresar una fecha válida no sale del ciclo.
                    printf("Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.        
                }
                while ((c = getchar()) != '\n' && c != EOF);
                // Carga de la fecha final
                printf ("Ingrese la fecha final (dd/mm/aaaa): ");
                while (scanf("%2d/%2d/%4d", &fecha2.dia, &fecha2.mes, &fecha2.anio) != 3 || !ValidarFecha(fecha2)) { // hasta no ingresar una fecha válida no sale del ciclo.
                    printf("Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.        
                }
                while ((c = getchar()) != '\n' && c != EOF);
            } while (!(EsMayorFechaSinHora(fecha2, fecha1) && !(EsIgualFecha(fecha2, fecha1)))); // repetir hasta que la fecha2 sea >= que la fecha1
            ListarIngresosEntreFechas (fecha1, fecha2, lista);         
        }
        else { // opcion = 8
            printf ("\nSaliendo del sistema...");
            // Abre el archivo temporal temp.dat para guardar temporalmente la lista
            GuardarLSEEnArchivo (archivoTemporal, &lista);
            // Copia del contenido del archivo temp.dat en el archivo BaseDeReserva.dat
            CopiarArchivos (archivoTemporal, archivoBaseDeDatos);
        }
    } while (opcion != 8);
    return 0;
}