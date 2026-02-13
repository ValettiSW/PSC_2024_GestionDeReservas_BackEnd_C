// Biblioteca Reserva

#ifndef RESERVA_H
#define RESERVA_H

#include <stdbool.h>

// Declaración de las estructuras de datos
typedef struct TFecha {
    int dia;
    int mes;
    int anio;
} TFecha;

typedef struct THora {
    int hora; // Formato 24 horas
    int minutos;
    int segundos;
} THora;

typedef struct TResponsable {
    char nombre[120];
    char apellido[120];
    char telefono[50];
    char institucion[120];
    int dni;
} TResponsable;

typedef struct TReserva {
    int nroReserva;
    TFecha diaReserva;
    THora horaInicio;
    THora horaFin;
    TResponsable responsable;
    char descripcion[256];
    float monto;
    bool pagadoAdeudado; // true = pagado, false = adeudado
    char observaciones[256];
} TReserva;

typedef struct TDoblete TDoblete;

typedef struct TDoblete {
    TReserva r;
    TDoblete *next;
} TDoblete;

typedef struct TLista {
    TDoblete *p;
} TLista;

// Perfiles de las acciones y funciones
void CargarArchivoEnLSE (char nombre[150], TLista *l);
void GuardarLSEEnArchivo (char nombre[150], TLista *l);
void CopiarArchivos (char origen[150], char destino[150]);
bool EsBisiesto (int a);
void Mensajes();
bool VerificarDisponibilidad (TLista l, TFecha fecha, THora inicio, THora fin);
void IngresarReserva (TLista *l);
void BuscarReserva (TLista l);
void ModificarReserva (int num, TLista l); // Modifica la reserva que tiene el número de reserva num
void OrdenarListaPorFecha (TLista *l); // Ordena la lista por fechas, mediante el método burbuja
void OrdenarListaPorNombre (TLista *l); // Ordena la lista por apellido y nombre, mediante el método burbuja
bool ValidarFecha (TFecha fe); // Devuelve verdadero si la fecha tiene un formato válido
bool ValidarHora (THora h); // Devuelve verdadero si la hora tiene un formato válido
bool EsMayorNombre (TResponsable p1, TResponsable p2); //  Devuelve verdadero si el primer nombre es mayor que el segundo (apellido y nombre)
bool EsIgualFecha (TFecha fe1, TFecha fe2); // Devuelve verdadero si ambas fechas tienen mismo año, mes y día
bool EsMayorFecha (TReserva res1, TReserva res2); // Devuelve verdadero si la primer fecha es mayor que la segunda (tiene en cuenta las horas)
bool EsMayorFechaSinHora (TFecha fe1, TFecha fe2); // Devuelve verdadero si la primer fecha es mayor que la segunda (NO tiene en cuenta las horas)
bool FechaEnRango (TFecha fe1, TFecha fe2, TFecha fe3); // Devuelve verdadero si fe1 está entre fe2 y fe3
bool EsMayorHora (THora hora1, THora hora2); // Devuelve verdadero si la primer hora es mayor que la segunda
void IntercambiarReservas (TReserva *a, TReserva *b); // Intercambia el campo info de la lista
bool EsMayuscula (char *c); // Devuelve verdadero si una cadena está en mayúsculas
void AMinuscula (char *c); // Pasa cualquier cadena a minúscula (mantiene la primera letra en mayúscula)
bool EsLetra (char c); // Devuelve verdadero si el caracter es una letra
bool EsNombreValido (char *c); // Función que verifica si lo ingresado son letras
void EliminarReserva (int num, TLista *l);
void VaciarLista (TLista *l); // Elimina el contenido de una lista
TLista CopiarLista (TLista l); // Copia el contenido de una lista
void ListarEventosEntreFechas (TFecha fe1, TFecha fe2, TLista l);
void ListarDisponibilidadDia (TFecha fe, TLista l);
void ListarIngresosEntreFechas (TFecha fe1, TFecha fe2, TLista l);

#endif