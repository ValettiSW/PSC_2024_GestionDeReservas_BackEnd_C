// Biblioteca Reserva

#include "Reserva.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Implementación
// Carga el contenido de un archivo en una Lista Simpelemente Encadenada
void CargarArchivoEnLSE (char nombre[150], TLista *l) {
    FILE *h;
    TLista aux;
    TLista nuevo;

    // Abre el archivo "nombre" y lo soporta sobre una lista simplemente encadenada
    h = fopen (nombre, "r");
    if (h != NULL) { // Apertura correcta del archivo
        
        l->p = NULL;
        
        nuevo.p = (TDoblete *) malloc (sizeof(TDoblete)); // Almaceno el primer dato en este nodo de forma temporal
        nuevo.p->next = NULL;

        //Lee el primer registro si el archivo no está vacío
        if (fread (&(nuevo.p->r), sizeof(nuevo.p->r), 1, h) == 1) {
            l->p = nuevo.p; // Cabeza de lista
            aux.p = nuevo.p; // Auxiliar para recorrer la lista

            // Lee los siguientes registros
            nuevo.p = (TDoblete *) malloc (sizeof(TDoblete)); // Creo el doblete para almacenar el siguiente dato del archivo
            nuevo.p->next = NULL;
            while (fread (&(nuevo.p->r), sizeof(nuevo.p->r), 1, h) == 1) {
                aux.p->next = nuevo.p;
                aux.p = nuevo.p;
                nuevo.p = (TDoblete *) malloc (sizeof(TDoblete)); // Creo el doblete para almacenar el siguiente dato del archivo
                nuevo.p->next = NULL;             
            }
            free (nuevo.p); // Al finalizar la lectura del archivo, permanece el doblete nuevo.p sin información asignada, por lo que lo libero
        }
        else { // Archivo vacío
            free (nuevo.p); // Como el archivo está vacío, puedo liberar el espacio de memoria reservado a nuevo.p
        }
    fclose (h);
    }
    return;
}

// Copia el contenido del archivo origen en el archivo destino
void CopiarArchivos (char origen[150], char destino[150]) {
    FILE *f;
    FILE *g;
    TReserva r;

    f = fopen (destino, "w");
    g = fopen (origen, "r");
    if (g != NULL) {
        while (fread (&r, sizeof(r), 1, g) == 1) {
            fwrite (&r, sizeof(r), 1, f);
        }
    }
    fclose (f);
    fclose (g);
    return;
}

// Guarda el contenido de una Lista simplemente encadenada (LSE) en un archivo binario
void GuardarLSEEnArchivo (char nombre[150], TLista *l) {
    FILE *h;
    TLista aux;
    TLista borrar;

    // Abre el archivo temporal nombre para guardar temporalmente la lista
    h = fopen (nombre, "w");
    aux.p = l->p;
    while (aux.p != NULL) {
        fwrite (&(aux.p->r), sizeof(aux.p->r), 1, h);
        borrar.p = aux.p; // el doblete apuntado por aux.p es asignado a borrar.p para poder eliminarlo
        aux.p = aux.p->next;
        free (borrar.p); // se libera de la memoria el espacio ocupado por el doblete.
    }
    fclose (h);
    return;
}

// Devuelve 1 si el año es bisiesto, o 0 si no lo es
bool EsBisiesto (int a) {
    return (((a % 4 == 0) && (a % 100 != 0)) || (a % 400 == 0));
}

// Muestra un mensaje a la hora de cargar la reserva
void Mensajes(){
    printf("\n");
    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*           Cargar reserva             *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");
    printf("\n");

    printf("\n");
    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*   Sugerencia: el formato es de 24 hs   *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");
    printf("\n");
}

// Indica si la hora de la reserva está disponible
bool VerificarDisponibilidad (TLista l, TFecha fecha, THora inicio, THora fin) {
    TLista aux;

    aux.p = l.p;

    while (aux.p != NULL) {
        if (EsIgualFecha (aux.p->r.diaReserva, fecha)) {
            if (!(EsMayorHora (aux.p->r.horaInicio, fin) || EsMayorHora (inicio, aux.p->r.horaFin))) {
                printf ("Ya existe una reserva para este horario.\n");
                return false;
            }
        }
        aux.p = aux.p->next;
    }
    return true;
}

// Ingresa una o más reservas en el sistema
void IngresarReserva (TLista *l) {
    TReserva res;
    TFecha fechaActual;
    TDoblete *aux;
    TDoblete *nuevo;
    TDoblete *ult;
    bool seguir;
    int nRes, c;
    char sn;

    // Pedir la fecha actual
    printf ("\n- Ingrese la fecha actual (dd/mm/aaaa): ");
    while (scanf("%2d/%2d/%4d", &fechaActual.dia, &fechaActual.mes, &fechaActual.anio) != 3 || !ValidarFecha(fechaActual)) { // hasta no ingresar una fecha válida no sale del ciclo.
        printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
        while (getchar() != '\n'); // limpia el búfer de entrada.
    }
    while ((c = getchar()) != '\n' && c != EOF);
    // Cargar los datos de la reserva
    do {
        printf ("\n- Ingrese los datos de la reserva:");
        int c;

        //Validar fecha
        do {
            printf ("\n- Ingrese la fecha de la reserva (dd/mm/aaaa): ");
            while (scanf("%2d/%2d/%4d", &res.diaReserva.dia, &res.diaReserva.mes, &res.diaReserva.anio) != 3 || !ValidarFecha(res.diaReserva)) { // hasta no ingresar una fecha válida no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.        
            }
            while ((c = getchar()) != '\n' && c != EOF);          
        } while (!EsMayorFechaSinHora (res.diaReserva, fechaActual));

        // Validar hora de inicio y hora de fin con disponibilidad
        seguir = true;
        do {
            printf ("- Ingrese la hora de inicio del evento (hh:mm) ");
            while (scanf("%2d:%2d", &res.horaInicio.hora, &res.horaInicio.minutos) != 2 || !ValidarHora(res.horaInicio)) { // hasta no ingresar una hora válida no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf ("- Ingrese la hora de finalización del evento (hh:mm): ");
            while (scanf("%2d:%2d", &res.horaFin.hora, &res.horaFin.minutos) != 2 || !ValidarHora(res.horaFin)) { // hasta no ingresar una hora válida no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);

            // Verificar disponibilidad
            if (!VerificarDisponibilidad (*l, res.diaReserva, res.horaInicio, res.horaFin)) {
                printf("\nEl horario ya está ocupado. Por favor, intente con otro horario.\n");             
            }
            else {
                seguir = false; // Salir del bucle si todo es válido 
            }            
        } while (seguir);

        // Cargar responsable
        printf ("- Ingrese el nombre del responsable (sin acentos): ");
        if (fgets (res.responsable.nombre, sizeof(res.responsable.nombre), stdin) != NULL) {
            // Elimina el salto de línea si es necesario
            size_t len = strlen (res.responsable.nombre);
            if (len > 0 && res.responsable.nombre[len-1] == '\n') {
                res.responsable.nombre[len-1] = '\0';
            }
        }
        printf ("- Ingrese el apellido del responsable (sin acentos): ");
        if (fgets (res.responsable.apellido, sizeof(res.responsable.apellido), stdin) != NULL) {
            // Elimina el salto de línea si es necesario
            size_t len = strlen (res.responsable.apellido);
            if (len > 0 && res.responsable.apellido[len-1] == '\n') {
                res.responsable.apellido[len-1] = '\0';
            }
        }
        printf ("- Ingrese el teléfono del responsable: ");
        scanf ("%s", res.responsable.telefono);
        while ((c = getchar()) != '\n' && c != EOF);
        printf ("- Ingrese el DNI del responsable: ");
        while (scanf("%d", &res.responsable.dni) != 1 || res.responsable.dni < 1) { // hasta no ingresar un número entero positivo no sale del ciclo.
            printf("\n Entrada no válida. Por favor, ingrese un DNI correcto.\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);
        printf ("- Si representa a una institución, ingrese el nombre (sin acentos) (o presione Enter para omitir): ");
        if (fgets (res.responsable.institucion, sizeof(res.responsable.institucion), stdin) != NULL) {
            // Elimina el salto de línea si es necesario
            size_t len = strlen (res.responsable.institucion);
            if (len > 0 && res.responsable.institucion[len-1] == '\n') {
                res.responsable.institucion[len-1] = '\0';
            }
        }

        // Cargar descripción
        printf ("- Ingrese una descripción para la reserva (sin acentos): ");
        if (fgets (res.descripcion, sizeof(res.descripcion), stdin) != NULL) {
            // Elimina el salto de línea si es necesario
            size_t len = strlen (res.descripcion);
            if (len > 0 && res.descripcion[len-1] == '\n') {
                res.descripcion[len-1] = '\0';
            }
        }

        // Cargar monto
        printf ("- Ingrese el monto de la reserva: ");
        while (scanf("%f", &res.monto) != 1 || res.monto < 0) { // hasta no ingresar un monto mayor o igual a cero no sale del ciclo.
            printf("\n Entrada no válida. Por favor, ingrese un monto correcto.\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);

        // Cargar estado de pago
        printf ("- ¿La reserva está pagada? (s/n): ");
        while (scanf(" %c", &sn) != 1 || (sn != 's' && sn != 'S' && sn != 'n' && sn != 'N')) { // hasta no ingresar s/S o n/N no sale del ciclo.
            printf("\n Entrada no válida. Por favor, ingrese (s/S) o (n/N).\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);
        res.pagadoAdeudado = (sn == 's' || sn == 'S');

        // Cargar observaciones
        printf ("- Ingrese observaciones adicionales (o presione Enter para omitir): ");
        if (fgets (res.observaciones, sizeof(res.observaciones), stdin) != NULL) {
            // Elimina el salto de línea si es necesario
            size_t len = strlen (res.observaciones);
            if (len > 0 && res.observaciones[len-1] == '\n') {
                res.observaciones[len-1] = '\0';
            }
        }

        // Guardar en la lista
        nuevo = (TDoblete *) malloc (sizeof(TDoblete));
        nuevo->next = NULL;
        nuevo->r.diaReserva.dia = res.diaReserva.dia;
        nuevo->r.diaReserva.mes = res.diaReserva.mes;
        nuevo->r.diaReserva.anio = res.diaReserva.anio;
        nuevo->r.horaInicio.hora = res.horaInicio.hora;
        nuevo->r.horaInicio.minutos = res.horaInicio.minutos;
        nuevo->r.horaInicio.segundos = res.horaInicio.segundos;
        nuevo->r.horaFin.hora = res.horaFin.hora;
        nuevo->r.horaFin.minutos = res.horaFin.minutos;
        nuevo->r.horaFin.segundos = res.horaFin.segundos;
        strcpy (nuevo->r.responsable.nombre, res.responsable.nombre);
        strcpy (nuevo->r.responsable.apellido, res.responsable.apellido);
        strcpy (nuevo->r.responsable.telefono, res.responsable.telefono);
        nuevo->r.responsable.dni = res.responsable.dni;
        strcpy (nuevo->r.responsable.institucion, res.responsable.institucion);
        strcpy (nuevo->r.descripcion, res.descripcion);
        nuevo->r.monto = res.monto;
        nuevo->r.pagadoAdeudado = res.pagadoAdeudado;
        strcpy (nuevo->r.observaciones, res.observaciones);

        aux = l->p;
        ult = NULL;

        nuevo->r.nroReserva = 1; // Inicializo el contador de nroReserva en 1
        nRes = 0; // Inicializo nRes en 0

        while (aux != NULL) {
            nRes = aux->r.nroReserva;
            ult = aux;
            aux = aux->next;
        }
        if (nuevo->r.nroReserva <= nRes) {
            nuevo->r.nroReserva = nRes + 1;
        }

        if (l->p == NULL) { // La lista está vacía, es una inserción a la cabeza
            l->p = nuevo;
        }
        else { // La lista tiene al menos un elemento. No necesito hacer un nuevo recorrido porque ya lo 
            ult->next = nuevo;
        }

        printf ("- ¿Desea cargar otra reserva? (s/n): ");
        while (scanf(" %c", &sn) != 1 || (sn != 's' && sn != 'S' && sn != 'n' && sn != 'N')) { // hasta no ingresar s/S o n/N no sale del ciclo.
            printf("\n Entrada no válida. Por favor, ingrese (s/S) o (n/N).\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);    
    } while (sn == 's' || sn == 'S');

    return;
}

// Busca una reserva por número de reserva, DNI del responsable o nombre y apellido del responsable
void BuscarReserva (TLista l) {
    int opcion, numReserva, doc, c;
    char nombre[120];
    char apellido[120];
    TReserva elem;
    bool encontrado = false;
    TLista aux;

    if (l.p == NULL) {
        printf ("\nNo hay reservas\n");
        return;
    }
    else {
        aux.p = l.p;
        printf ("\nSeleccione el criterio de búsqueda:");
        printf ("\n1- Número de reserva");
        printf ("\n2- DNI");
        printf ("\n3- Nombre y apellido");
        printf ("\n- Ingrese su opción: ");
        while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 3) { // hasta no ingresar 1, 2 o 3 no sale del ciclo.
            printf("\n Entrada no válida. Por favor, elija una opción entre 1 y 3.\n ");
            while (getchar() != '\n'); // limpia el búfer de entrada.
        }
        while ((c = getchar()) != '\n' && c != EOF);

        if (opcion == 1) {
            printf ("- Ingrese el número de reserva: ");
            while (scanf("%d", &numReserva) != 1 || numReserva < 1) { // hasta no ingresar un número positivo no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese un número de reserva positivo.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);

            // Búsqueda de numReserva en la lista
            while (aux.p != NULL && !encontrado) {
                if (aux.p->r.nroReserva == numReserva) {
                    encontrado = true;
                    printf ("\nReserva Nro: %d", aux.p->r.nroReserva);
                    printf ("\nFecha: %02d/%02d/%04d", aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio);
                    printf ("\nHora inicio: %02d:%02d", aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos);
                    printf ("\nHora fin: %02d:%02d", aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                    printf ("\nResponsable: %.15s %.15s", aux.p->r.responsable.nombre, aux.p->r.responsable.apellido);
                    printf (", Tel: %s, DNI: %d", aux.p->r.responsable.telefono, aux.p->r.responsable.dni);
                    printf ("\nInstitución: %s", aux.p->r.responsable.institucion);
                    printf ("\nDescripción: %s", aux.p->r.descripcion);
                    if (aux.p->r.pagadoAdeudado) {
                        printf ("\nEstado: Pagado");
                    }
                    else {
                        printf ("\nEstado: No Pagado");
                    }
                    printf ("\nMonto: %.2f", aux.p->r.monto);
                    printf ("\nObservaciones: %s\n", aux.p->r.observaciones);
                }
                else {
                    aux.p = aux.p->next;
                }
            }
        }
        else if (opcion == 2) {
            printf ("- Ingrese el número de DNI: ");
            while (scanf("%d", &doc) != 1 || doc < 1) { // hasta no ingresar un número de DNI positivo no sale del ciclo.
                printf("\n Entrada no válida. Por favor, ingrese un número de DNI positivo.\n ");
                while (getchar() != '\n'); // limpia el búfer de entrada.
            }
            while ((c = getchar()) != '\n' && c != EOF);

            // Búsqueda del DNI en la lista
            while (aux.p != NULL) {
                if (aux.p->r.responsable.dni == doc) {
                    encontrado = true;
                    printf ("\nReserva Nro: %d", aux.p->r.nroReserva);
                    printf ("\nFecha: %02d/%02d/%04d", aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio);
                    printf ("\nHora inicio: %02d:%02d", aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos);
                    printf ("\nHora fin: %02d:%02d", aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                    printf ("\nResponsable: %.15s %.15s", aux.p->r.responsable.nombre, aux.p->r.responsable.apellido);
                    printf (", Tel: %s, DNI: %d", aux.p->r.responsable.telefono, aux.p->r.responsable.dni);
                    printf ("\nInstitución: %s", aux.p->r.responsable.institucion);
                    printf ("\nDescripción: %s", aux.p->r.descripcion);
                    if (aux.p->r.pagadoAdeudado) {
                        printf ("\nEstado: Pagado");
                    }
                    else {
                        printf ("\nEstado: No Pagado");
                    }
                    printf ("\nMonto: %.2f", aux.p->r.monto);
                    printf ("\nObservaciones: %s\n", aux.p->r.observaciones);
                }
                aux.p = aux.p->next;
            }
        }
        else { // opcion == 3
            int c;
            printf ("- Ingrese el nombre (sin acentos): ");
            if (fgets (nombre, sizeof(nombre), stdin) != NULL) {
                // Elimina el salto de línea si es necesario
                size_t len = strlen (nombre);
                if (len > 0 && nombre[len-1] == '\n') {
                    nombre[len-1] = '\0';
                }
            }
            printf ("- Ingrese el apellido (sin acentos): ");
            if (fgets (apellido, sizeof(apellido), stdin) != NULL) {
                // Elimina el salto de línea si es necesario
                size_t len = strlen (apellido);
                if (len > 0 && apellido[len-1] == '\n') {
                    apellido[len-1] = '\0';
                }
            }

            // Búsqueda en la lista por nombre y apellido
            while (aux.p != NULL) {
                if (strcmp (aux.p->r.responsable.nombre, nombre) == 0 && strcmp (aux.p->r.responsable.apellido, apellido) == 0) {
                    encontrado = true;
                    printf ("\nReserva Nro: %d", aux.p->r.nroReserva);
                    printf ("\nFecha: %02d/%02d/%04d", aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio);
                    printf ("\nHora inicio: %02d:%02d", aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos);
                    printf ("\nHora fin: %02d:%02d", aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                    printf ("\nResponsable: %.15s %.15s", aux.p->r.responsable.nombre, aux.p->r.responsable.apellido);
                    printf (", Tel: %s, DNI: %d", aux.p->r.responsable.telefono, aux.p->r.responsable.dni);
                    printf ("\nInstitución: %s", aux.p->r.responsable.institucion);
                    printf ("\nDescripción: %s", aux.p->r.descripcion);
                    if (aux.p->r.pagadoAdeudado) {
                        printf ("\nEstado: Pagado");
                    }
                    else {
                        printf ("\nEstado: No Pagado");
                    }
                    printf ("\nMonto: %.2f", aux.p->r.monto);
                    printf ("\nObservaciones: %s\n", aux.p->r.observaciones);
                }
                aux.p = aux.p->next;
            }
        }

        // Reserva no encontrada
        if (!encontrado) {
            printf ("\nReserva no encontrada.\n");
        }
    }
    return;
}

// Indica si la primera hora es mayor que la segunda (formato 24 horas (hh:mm))
bool EsMayorHora (THora hora1, THora hora2) {
    if (hora1.hora > hora2.hora) {
        return true;
    }
    else if (hora1.hora == hora2.hora) {
        if (hora1.minutos > hora2.minutos) {
            return true;
        }
        else { // hora1.minutos <= hora2.minutos
            return false;
        }
    }
    else { // hora1.hora < hora2.hora
        return false;
    }
}

// Indica si ambas fechas son iguales (mismos año, mes y día)
bool EsIgualFecha (TFecha fe1, TFecha fe2) {
    return (fe1.anio == fe2.anio && fe1.mes == fe2.mes && fe1.dia == fe2.dia);
}

// Indica si la primera fecha es mayor que la segunda (formato dd/mm/aaaa)
bool EsMayorFecha (TReserva res1, TReserva res2) {
    // Compara año
    if (res1.diaReserva.anio > res2.diaReserva.anio) {
        return true;
    }
    else if (res1.diaReserva.anio < res2.diaReserva.anio) {
        return false;
    }

    // Compara mes (si los años son iguales)
    if (res1.diaReserva.mes > res2.diaReserva.mes) {
        return true;
    }
    else if (res1.diaReserva.mes < res2.diaReserva.mes) {
        return false;
    }

    // Compara días (si los años y meses son iguales)
    if (res1.diaReserva.dia > res2.diaReserva.dia) {
        return true;
    }
    else if (res1.diaReserva.dia < res2.diaReserva.dia) {
        return false;
    }

    // Si año, mes y día son iguales, compara las horas
    return EsMayorHora (res1.horaInicio, res2.horaInicio);
}

// Indica si el primer nombre es mayor que el segundo (apellido y nombre)
bool EsMayorNombre (TResponsable p1, TResponsable p2) {
    int cmpApellido = strcmp (p1.apellido, p2.apellido);

    if (cmpApellido != 0) {
        return cmpApellido > 0 ; // retorne verdadero si el apellido p1 es mayor que el p2, o falso si es menor
    }
    // Si los apellidos son iguales, compara los nombres
    return strcmp (p1.nombre, p2.nombre) > 0;
}

// Acción que intercambia el contenido de dos nodos
void IntercambiarReservas (TReserva *a, TReserva *b) {
    TReserva temp;

    temp = *a;
    *a = *b;
    *b = temp;
    return;
}

bool EsMayuscula (char *c) { // Devuelve verdadero si todos los caracteres son mayúsculas
    int i;
    bool temp;

    i = 0;
    temp = true;
    while (c[i] != '\0' && temp) {
        if ((c[i] < 'A' || c[i] > 'Z') && c[i] != 32) {
            temp = false; // si hay un carácter que no está en mayúsculas, temp pasa a falso y sale del ciclo
        }
        else {
            i++;
        }
    }
    return temp; // Devuelve verdadero si llega al final de la palabra y todos los caracteres son mayúsculas (y/o espacios)
}

// Pasa una cadena en mayúsculas a minúsculas, manteniendo la mayúscula capital o tras un espacio
void AMinuscula (char *c) {
    int i;

    i = 1; // comenzamos en la segunda posición porque la priemra letra debe ser mayúscula
    while (c[i] != '\0') {
        if ((c[i] >= 'A' && c[i] <= 'Z') && c[i-1] != 32) {
            c[i] += ('a' - 'A'); // Convertir a minúscula
        }
        i++;
    }
}

// Devuelve verdadero si el caracter es una letra
bool EsLetra (char c) {
    if ((c > 64 && c < 91) || (c > 96 && c < 123)) {
        return true;
    }
    else {
        return false;
    }
}

// Devuelve verdadero si los caracteres de la cadena son letras o espacios
bool EsNombreValido (char *c) {
    bool temp;
    int i;

    i = 0;
    temp = true;
    while (c[i] != '\0' && temp) {
        if (c[i] != 32 && !EsLetra(c[i])) { // se asigna falso si c[i] no es una letra o un espacio
            temp = false;
        }
        i++;
    }
    return temp;
}

void OrdenarListaPorFecha (TLista *l) {
    // Léxico local
    TDoblete *actual;
    TDoblete *sig;
    TDoblete *ult;

    if (l->p == NULL || l->p->next == NULL) { // Lista vacía o con un solo elemento
        return;
    }
    else { // Lista con más de un elemento. Ordenamiento de burbuja
        ult = NULL;
        while (ult != l->p) {
            actual = l->p; // Reinicia al inicio de la lista
            while (actual->next != ult) {
                sig = actual->next;
                if (EsMayorFecha (actual->r, sig->r)) {
                    IntercambiarReservas (&actual->r, &sig->r);
                }
                actual = actual->next;
            }
            ult = actual; // Se reduce el rango del bucle en cada iteración
        }
    }
}

void OrdenarListaPorNombre (TLista *l) {
    // Léxico local
    TDoblete *actual;
    TDoblete *sig;
    TDoblete *ult;

    if (l->p == NULL) { // Lista vacía
        return;
    }
    else { // Lista con datos
        actual = l->p;
        ult = NULL;
        while (ult != l->p) {
            while (actual->next != ult) {
                sig = actual->next;
                if (EsMayorNombre (actual->r.responsable, sig->r.responsable)) {
                    IntercambiarReservas (&actual->r, &sig->r);
                }
                actual = actual->next;
            }
            ult = actual; // Se reduce el rango del bucle en cada iteración
        }
    }    
}

// Elimina una reserva a partir del número de reserva
void EliminarReserva (int num, TLista *l) {
    // Léxico local
    TDoblete *aux;
    TDoblete *ant;
    TDoblete *sig;
    bool continuar;

    if (l->p == NULL) { // Lista vacía
        printf ("Lista vacía\n");
    }
    else { // Se asume que la lista está ordenada por número de reserva
        aux = l->p;
        sig = aux->next;

        // Lista con un solo elemento
        if (sig == NULL) {
            // Eliminar el único elemento de la lista
            if (num == aux->r.nroReserva) {
                l->p = NULL;
                free (aux);
                printf ("Reserva eliminada\n");
            }
            else { // El elemento no está en la lista
                printf ("La reserva no se encuentra en la lista\n");
            }
        }
        // Lista con más de un elemento
        else {
            continuar = true;
            while (sig != NULL && continuar) {
                // Avanzo hasta que num sea 
                if (num < sig->r.nroReserva) {
                    continuar = false;
                }
                else {
                    sig = sig->next;
                    ant = aux;
                    aux = aux->next;
                }
            }
            // Llegó al fin de la lista
            if (continuar) {
                // La reserva está en la lista
                if (num == aux->r.nroReserva) {
                    ant->next = sig;
                    free (aux);
                    printf ("Reserva eliminada\n");
                }
                // La reserva no está en la lista
                else {
                    printf ("La reserva no se encuentra en la lista\n");
                }
            }
            else { // continuar = false, o sea alcancé sig->r.nroReserva mayor que el buscado
                // La reserva está en la lista
                if (num == aux->r.nroReserva) {
                    ant->next = sig;
                    free (aux);
                    printf ("Reserva eliminada\n");
                }
                // La reserva no está e nla lista
                else {
                    printf ("La reserva no está en la lista\n");
                }
            }
        }
    }
}

// Modifica la reserva que tiene el nroReserva = num
void ModificarReserva (int num, TLista l) {
    TReserva res;
    TFecha fechaActual;
    TDoblete *aux;
    TDoblete *ant;
    TDoblete *sig;
    bool continuar;
    int c;
    int nreserva;
    char sn;

    // Búsqueda del número de reserva en la lista
    if (l.p == NULL) { // Lista vacía
        printf ("Lista vacía\n");
    }
    else { // Se asume que la lista está ordenada por número de reserva
        aux = l.p;
        sig = aux->next;

        // Lista con un solo elemento
        if (sig == NULL) {
            // Moficicar el único elemento de la lista
            if (num == aux->r.nroReserva) {
                nreserva = num;
                // Pedir la fecha actual
                printf ("\n- Ingrese la fecha actual (dd/mm/aaaa): ");
                while (scanf("%2d/%2d/%4d", &fechaActual.dia, &fechaActual.mes, &fechaActual.anio) != 3 || !ValidarFecha(fechaActual)) { // hasta no ingresar una fecha válida no sale del ciclo.
                    printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.        
                }
                while ((c = getchar()) != '\n' && c != EOF);

                printf ("- Ingrese los nuevos datos de la reserva:\n");

                //Validar fecha
                do {
                    printf ("- Ingrese la nueva fecha de la reserva (dd/mm/aaaa): ");
                    while (scanf("%2d/%2d/%4d", &res.diaReserva.dia, &res.diaReserva.mes, &res.diaReserva.anio) != 3 || !ValidarFecha(res.diaReserva)) { // hasta no ingresar una fecha válida no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                            while (getchar() != '\n'); // limpia el búfer de entrada.        
                    }
                    while ((c = getchar()) != '\n' && c != EOF);     
                } while (!EsMayorFechaSinHora (res.diaReserva, fechaActual));

                // Validar hora de inicio y hora de fin con disponibilidad
                do {
                    printf ("- Ingrese la nueva hora de inicio del evento (hh:mm) ");
                    while (scanf("%2d:%2d", &res.horaInicio.hora, &res.horaInicio.minutos) != 2 || !ValidarHora(res.horaInicio)) { // hasta no ingresar una hora válida no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    printf ("- Ingrese la nueva hora de finalización del evento (hh:mm): ");
                    while (scanf("%2d:%2d", &res.horaFin.hora, &res.horaFin.minutos) != 2 || !ValidarHora(res.horaFin)) { // hasta no ingresar una hora válida no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    // Verificar disponibilidad
                    if (!VerificarDisponibilidad (l, res.diaReserva, res.horaInicio, res.horaFin)) {
                        printf("\nEl horario ya está ocupado. Por favor, intente con otro horario.\n");             
                    }
                    else {
                        break; // Salir del bucle si todo es válido 
                    }            
                } while (true);

                // Cargar responsable
                printf ("- Ingrese el nuevo nombre del responsable (sin acentos): ");
                if (fgets (res.responsable.nombre, sizeof(res.responsable.nombre), stdin) != NULL) {
                    // Elimina el salto de línea si es necesario
                    size_t len = strlen (res.responsable.nombre);
                    if (len > 0 && res.responsable.nombre[len-1] == '\n') {
                        res.responsable.nombre[len-1] = '\0';
                    }
                }
                printf ("- Ingrese el nuevo apellido del responsable (sin acentos): ");
                if (fgets (res.responsable.apellido, sizeof(res.responsable.apellido), stdin) != NULL) {
                    // Elimina el salto de línea si es necesario
                    size_t len = strlen (res.responsable.apellido);
                    if (len > 0 && res.responsable.apellido[len-1] == '\n') {
                        res.responsable.apellido[len-1] = '\0';
                    }
                }
                printf ("- Ingrese el nuevo teléfono del responsable: ");
                scanf ("%s", res.responsable.telefono);
                while ((c = getchar()) != '\n' && c != EOF);
                printf ("- Ingrese el nuevo DNI del responsable: ");
                while (scanf("%d", &res.responsable.dni) != 1 || res.responsable.dni < 1) { // hasta no ingresar un número entero positivo no sale del ciclo.
                    printf("\n Entrada no válida. Por favor, ingrese un DNI correcto.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.
                }
                while ((c = getchar()) != '\n' && c != EOF);
                printf ("- Si representa a una institución, ingrese el nombre (sin acentos) (o presione Enter para omitir): ");
                if (fgets (res.responsable.institucion, sizeof(res.responsable.institucion), stdin) != NULL) {
                    // Elimina el salto de línea si es necesario
                    size_t len = strlen (res.responsable.institucion);
                    if (len > 0 && res.responsable.institucion[len-1] == '\n') {
                        res.responsable.institucion[len-1] = '\0';
                    }
                }

                // Cargar descripción
                printf ("- Ingrese una descripción para la reserva (sin acentos): ");
                if (fgets (res.descripcion, sizeof(res.descripcion), stdin) != NULL) {
                    // Elimina el salto de línea si es necesario
                    size_t len = strlen (res.descripcion);
                    if (len > 0 && res.descripcion[len-1] == '\n') {
                        res.descripcion[len-1] = '\0';
                    }
                }

                // Cargar monto
                printf ("- Ingrese el nuevo monto de la reserva: ");
                while (scanf("%f", &res.monto) != 1 || res.monto < 0) { // hasta no ingresar un monto mayor o igual a cero no sale del ciclo.
                    printf("\n Entrada no válida. Por favor, ingrese un monto correcto.\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.
                }
                while ((c = getchar()) != '\n' && c != EOF);

                // Cargar estado de pago
                printf ("- ¿La reserva está pagada? (s/n): ");
                while (scanf(" %c", &sn) != 1 || (sn != 's' && sn != 'S' && sn != 'n' && sn != 'N')) { // hasta no ingresar s/S o n/N no sale del ciclo.
                    printf("\n Entrada no válida. Por favor, ingrese (s/S) o (n/N).\n ");
                    while (getchar() != '\n'); // limpia el búfer de entrada.
                }
                while ((c = getchar()) != '\n' && c != EOF);
                res.pagadoAdeudado = (sn == 's' || sn == 'S');

                // Cargar observaciones
                printf ("- Ingrese observaciones adicionales (sin acentos) (o presione Enter para omitir): ");
                if (fgets (res.observaciones, sizeof(res.observaciones), stdin) != NULL) {
                    // Elimina el salto de línea si es necesario
                    size_t len = strlen (res.observaciones);
                    if (len > 0 && res.observaciones[len-1] == '\n') {
                        res.observaciones[len-1] = '\0';
                    }
                }
                IntercambiarReservas (&aux->r, &res);
                aux->r.nroReserva = nreserva;
                printf ("Reserva modificada con éxito\n");
            }
            else { // El elemento no está en la lista
                printf ("La reserva no se encuentra en la lista\n");
            }
        }
        // Lista con más de un elemento
        else {
            continuar = true;
            while (sig != NULL && continuar) {
                // Avanzo hasta que num sea 
                if (num < sig->r.nroReserva) {
                    continuar = false;
                }
                else {
                    sig = sig->next;
                    ant = aux;
                    aux = aux->next;
                }
            }
            // Llegó al fin de la lista
            if (continuar) {
                // La reserva está en la lista
                if (num == aux->r.nroReserva) {
                    // Pedir la fecha actual
                    nreserva = num;
                    printf ("\n- Ingrese la fecha actual (dd/mm/aaaa): ");
                    while (scanf("%2d/%2d/%4d", &fechaActual.dia, &fechaActual.mes, &fechaActual.anio) != 3 || !ValidarFecha(fechaActual)) { // hasta no ingresar una fecha válida no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.        
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    printf ("- Ingrese los nuevos datos de la reserva:\n");

                    //Validar fecha
                    do {
                        printf ("- Ingrese la nueva fecha de la reserva (dd/mm/aaaa): ");
                        while (scanf("%2d/%2d/%4d", &res.diaReserva.dia, &res.diaReserva.mes, &res.diaReserva.anio) != 3 || !ValidarFecha(res.diaReserva)) { // hasta no ingresar una fecha válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                                while (getchar() != '\n'); // limpia el búfer de entrada.        
                        }
                        while ((c = getchar()) != '\n' && c != EOF);     
                    } while (!EsMayorFechaSinHora (res.diaReserva, fechaActual));

                    // Validar hora de inicio y hora de fin con disponibilidad
                    do {
                        printf ("- Ingrese la nueva hora de inicio del evento (hh:mm) ");
                        while (scanf("%2d:%2d", &res.horaInicio.hora, &res.horaInicio.minutos) != 2 || !ValidarHora(res.horaInicio)) { // hasta no ingresar una hora válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                            while (getchar() != '\n'); // limpia el búfer de entrada.
                        }
                        while ((c = getchar()) != '\n' && c != EOF);

                        printf ("- Ingrese la nueva hora de finalización del evento (hh:mm): ");
                        while (scanf("%2d:%2d", &res.horaFin.hora, &res.horaFin.minutos) != 2 || !ValidarHora(res.horaFin)) { // hasta no ingresar una hora válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                            while (getchar() != '\n'); // limpia el búfer de entrada.
                        }
                        while ((c = getchar()) != '\n' && c != EOF);

                        // Verificar disponibilidad
                        if (!VerificarDisponibilidad (l, res.diaReserva, res.horaInicio, res.horaFin)) {
                            printf("\nEl horario ya está ocupado. Por favor, intente con otro horario.\n");             
                        }
                        else {
                            break; // Salir del bucle si todo es válido 
                        }            
                    } while (true);

                    // Cargar responsable
                    printf ("- Ingrese el nuevo nombre del responsable (sin acentos): ");
                    if (fgets (res.responsable.nombre, sizeof(res.responsable.nombre), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.nombre);
                        if (len > 0 && res.responsable.nombre[len-1] == '\n') {
                            res.responsable.nombre[len-1] = '\0';
                        }
                    }
                    printf ("- Ingrese el nuevo apellido del responsable (sin acentos): ");
                    if (fgets (res.responsable.apellido, sizeof(res.responsable.apellido), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.apellido);
                        if (len > 0 && res.responsable.apellido[len-1] == '\n') {
                            res.responsable.apellido[len-1] = '\0';
                        }
                    }
                    printf ("- Ingrese el nuevo teléfono del responsable: ");
                    scanf ("%s", res.responsable.telefono);
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf ("- Ingrese el nuevo DNI del responsable: ");
                    while (scanf("%d", &res.responsable.dni) != 1 || res.responsable.dni < 1) { // hasta no ingresar un número entero positivo no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese un DNI correcto.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf ("- Si representa a una institución, ingrese el nombre (sin acentos) (o presione Enter para omitir): ");
                    if (fgets (res.responsable.institucion, sizeof(res.responsable.institucion), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.institucion);
                        if (len > 0 && res.responsable.institucion[len-1] == '\n') {
                            res.responsable.institucion[len-1] = '\0';
                        }
                    }

                    // Cargar descripción
                    printf ("- Ingrese una descripción para la reserva (sin acentos): ");
                    if (fgets (res.descripcion, sizeof(res.descripcion), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.descripcion);
                        if (len > 0 && res.descripcion[len-1] == '\n') {
                            res.descripcion[len-1] = '\0';
                        }
                    }

                    // Cargar monto
                    printf ("- Ingrese el nuevo monto de la reserva: ");
                    while (scanf("%f", &res.monto) != 1 || res.monto < 0) { // hasta no ingresar un monto mayor o igual a cero no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese un monto correcto.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    // Cargar estado de pago
                    printf ("- ¿La reserva está pagada? (s/n): ");
                    while (scanf(" %c", &sn) != 1 || (sn != 's' && sn != 'S' && sn != 'n' && sn != 'N')) { // hasta no ingresar s/S o n/N no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese (s/S) o (n/N).\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);
                    res.pagadoAdeudado = (sn == 's' || sn == 'S');

                    // Cargar observaciones
                    printf ("- Ingrese observaciones adicionales (sin acentos) (o presione Enter para omitir): ");
                    if (fgets (res.observaciones, sizeof(res.observaciones), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.observaciones);
                        if (len > 0 && res.observaciones[len-1] == '\n') {
                            res.observaciones[len-1] = '\0';
                        }
                    }
                    IntercambiarReservas (&aux->r, &res);
                    aux->r.nroReserva = nreserva;
                    printf ("Reserva modificada con éxito\n");
                }
                // La reserva no está en la lista
                else {
                    printf ("La reserva no se encuentra en la lista\n");
                }
            }
            else { // continuar = false, o sea alcancé sig->r.nroReserva mayor que el buscado
                // La reserva está en la lista
                if (num == aux->r.nroReserva) {
                    // Pedir la fecha actual
                    nreserva = num;
                    printf ("\n- Ingrese la fecha actual (dd/mm/aaaa): ");
                    while (scanf("%2d/%2d/%4d", &fechaActual.dia, &fechaActual.mes, &fechaActual.anio) != 3 || !ValidarFecha(fechaActual)) { // hasta no ingresar una fecha válida no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.        
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    printf ("- Ingrese los nuevos datos de la reserva:\n");

                    //Validar fecha
                    do {
                        printf ("- Ingrese la nueva fecha de la reserva (dd/mm/aaaa): ");
                        while (scanf("%2d/%2d/%4d", &res.diaReserva.dia, &res.diaReserva.mes, &res.diaReserva.anio) != 3 || !ValidarFecha(res.diaReserva)) { // hasta no ingresar una fecha válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una fecha correcta.\n ");
                                while (getchar() != '\n'); // limpia el búfer de entrada.        
                        }
                        while ((c = getchar()) != '\n' && c != EOF);     
                    } while (!EsMayorFechaSinHora (res.diaReserva, fechaActual));

                    // Validar hora de inicio y hora de fin con disponibilidad
                    do {
                        printf ("- Ingrese la nueva hora de inicio del evento (hh:mm) ");
                        while (scanf("%2d:%2d", &res.horaInicio.hora, &res.horaInicio.minutos) != 2 || !ValidarHora(res.horaInicio)) { // hasta no ingresar una hora válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                            while (getchar() != '\n'); // limpia el búfer de entrada.
                        }
                        while ((c = getchar()) != '\n' && c != EOF);

                        printf ("- Ingrese la nueva hora de finalización del evento (hh:mm): ");
                        while (scanf("%2d:%2d", &res.horaFin.hora, &res.horaFin.minutos) != 2 || !ValidarHora(res.horaFin)) { // hasta no ingresar una hora válida no sale del ciclo.
                            printf("\n Entrada no válida. Por favor, ingrese una hora correcta.\n ");
                            while (getchar() != '\n'); // limpia el búfer de entrada.
                        }
                        while ((c = getchar()) != '\n' && c != EOF);

                        // Verificar disponibilidad
                        if (!VerificarDisponibilidad (l, res.diaReserva, res.horaInicio, res.horaFin)) {
                            printf("\nEl horario ya está ocupado. Por favor, intente con otro horario.\n");             
                        }
                        else {
                            break; // Salir del bucle si todo es válido 
                        }            
                    } while (true);

                    // Cargar responsable
                    printf ("- Ingrese el nuevo nombre del responsable (sin acentos): ");
                    if (fgets (res.responsable.nombre, sizeof(res.responsable.nombre), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.nombre);
                        if (len > 0 && res.responsable.nombre[len-1] == '\n') {
                            res.responsable.nombre[len-1] = '\0';
                        }
                    }
                    printf ("- Ingrese el nuevo apellido del responsable (sin acentos): ");
                    if (fgets (res.responsable.apellido, sizeof(res.responsable.apellido), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.apellido);
                        if (len > 0 && res.responsable.apellido[len-1] == '\n') {
                            res.responsable.apellido[len-1] = '\0';
                        }
                    }
                    printf ("- Ingrese el nuevo teléfono del responsable: ");
                    scanf ("%s", res.responsable.telefono);
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf ("- Ingrese el nuevo DNI del responsable: ");
                    while (scanf("%d", &res.responsable.dni) != 1 || res.responsable.dni < 1) { // hasta no ingresar un número entero positivo no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese un DNI correcto.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);
                    printf ("- Si representa a una institución, ingrese el nombre (sin acentos) (o presione Enter para omitir): ");
                    if (fgets (res.responsable.institucion, sizeof(res.responsable.institucion), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.responsable.institucion);
                        if (len > 0 && res.responsable.institucion[len-1] == '\n') {
                            res.responsable.institucion[len-1] = '\0';
                        }
                    }

                    // Cargar descripción
                    printf ("- Ingrese una descripción para la reserva (sin acentos): ");
                    if (fgets (res.descripcion, sizeof(res.descripcion), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.descripcion);
                        if (len > 0 && res.descripcion[len-1] == '\n') {
                            res.descripcion[len-1] = '\0';
                        }
                    }

                    // Cargar monto
                    printf ("- Ingrese el nuevo monto de la reserva: ");
                    while (scanf("%f", &res.monto) != 1 || res.monto < 0) { // hasta no ingresar un monto mayor o igual a cero no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese un monto correcto.\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);

                    // Cargar estado de pago
                    printf ("- ¿La reserva está pagada? (s/n): ");
                    while (scanf(" %c", &sn) != 1 || (sn != 's' && sn != 'S' && sn != 'n' && sn != 'N')) { // hasta no ingresar s/S o n/N no sale del ciclo.
                        printf("\n Entrada no válida. Por favor, ingrese (s/S) o (n/N).\n ");
                        while (getchar() != '\n'); // limpia el búfer de entrada.
                    }
                    while ((c = getchar()) != '\n' && c != EOF);
                    res.pagadoAdeudado = (sn == 's' || sn == 'S');

                    // Cargar observaciones
                    printf ("- Ingrese observaciones adicionales (sin acentos) (o presione Enter para omitir): ");
                    if (fgets (res.observaciones, sizeof(res.observaciones), stdin) != NULL) {
                        // Elimina el salto de línea si es necesario
                        size_t len = strlen (res.observaciones);
                        if (len > 0 && res.observaciones[len-1] == '\n') {
                            res.observaciones[len-1] = '\0';
                        }
                    }
                    IntercambiarReservas (&aux->r, &res);
                    aux->r.nroReserva = nreserva;
                    printf ("Reserva modificada con éxito \n");
                }
                // La reserva no está e nla lista
                else {
                    printf ("La reserva no está en la lista\n");
                }
            }
        }
    }
}

// Devuelve verdadero si fe1 está entre fe2 y fe3
bool FechaEnRango (TFecha fe1, TFecha fe2, TFecha fe3) { // requisito: fe2 <= fe3
    if (fe1.anio < fe2.anio || fe1.anio > fe3.anio) { // El año está fuera de rango
        return false;
    }
    if (fe1.anio == fe2.anio && (fe1.mes < fe2.mes || (fe1.mes == fe2.mes && fe1.dia < fe2.dia))) { // Año igual al de inicio, pero el mes o el día están fuera de rango
        return false;
    }
    if (fe1.anio == fe3.anio && (fe1.mes > fe3.mes || (fe1.mes == fe3.mes && fe1.dia > fe3.dia))) { // Año igual al de fin, pero el mes o el día están fuera de rango
        return false;
    }    
    return true; // Fecha dentro del rango
}

// Devuelve verdadero si fe1 es mayor que fe2 (NO tiene en cuenta las horas)
bool EsMayorFechaSinHora (TFecha fe1, TFecha fe2) {
    if (fe1.anio != fe2.anio) { // si los años son diferentes
        return fe1.anio > fe2.anio;
    }
    if (fe1.mes != fe2.mes) { // si los meses son diferentes
        return fe1.mes > fe2.mes;
    }
    return fe1.dia > fe2.dia;
}

// Elimina el contenido de una lista
void VaciarLista (TLista *l) {
    TDoblete *temp;

    if (l->p == NULL) { // Lista vacía
        //printf ("\nError. Lista vacía");
    }
    else {
        while (l->p != NULL) { // Lista con datos
            temp = l->p; // Copio el nodo a eliminar en temp
            l->p = l->p->next; // Avanzo al siguiente nodo
            free (temp); // Libero el espacio asignado al nodo de memoria que quité de la lista
        }
        l->p = NULL;
        //printf ("\nLista vaciada con éxito");
    }
}

// Copia el contenido de una lista
TLista CopiarLista (TLista l) {
    TLista nuevaLista;
    TDoblete *actual;
    TDoblete *nuevo;
    TDoblete *ultimo;

    nuevaLista.p = NULL;
    ultimo = NULL;
    actual = l.p;

    while (actual != NULL) {
        // Crea un nuevo nodo
        nuevo = (TDoblete *) malloc (sizeof(TDoblete));
        if (nuevo == NULL) {
            printf ("Error: No se pudo asignar memoria para la copia de la lista.\n");
            exit (EXIT_FAILURE);
        }

        // Copia el contenido del nodo
        strcpy (nuevo->r.descripcion, actual->r.descripcion);
        nuevo->r.diaReserva.anio = actual->r.diaReserva.anio;
        nuevo->r.diaReserva.dia = actual->r.diaReserva.dia;
        nuevo->r.diaReserva.mes = actual->r.diaReserva.mes;
        nuevo->r.horaFin.hora = actual->r.horaFin.hora;
        nuevo->r.horaFin.minutos = actual->r.horaFin.minutos;
        nuevo->r.horaFin.segundos = actual->r.horaFin.segundos;
        nuevo->r.horaInicio.hora = actual->r.horaInicio.hora;
        nuevo->r.horaInicio.minutos = actual->r.horaInicio.minutos;
        nuevo->r.horaInicio.segundos = actual->r.horaInicio.segundos;
        nuevo->r.monto = actual->r.monto;
        nuevo->r.nroReserva = actual->r.nroReserva;
        strcpy (nuevo->r.observaciones, actual->r.observaciones);
        nuevo->r.pagadoAdeudado = actual->r.pagadoAdeudado;
        strcpy (nuevo->r.responsable.apellido, actual->r.responsable.apellido);
        nuevo->r.responsable.dni = actual->r.responsable.dni;
        strcpy (nuevo->r.responsable.institucion, actual->r.responsable.institucion);
        strcpy (nuevo->r.responsable.nombre, actual->r.responsable.nombre);
        strcpy (nuevo->r.responsable.telefono, actual->r.responsable.telefono);
        nuevo->next = NULL;

        if (nuevaLista.p == NULL) {
            nuevaLista.p = nuevo;  // Primer nodo de la nueva lista
        }
        else {
            ultimo->next = nuevo;  // Conecta el nodo al final de la nueva lista
        }

        ultimo = nuevo;  // Actualiza el último nodo
        actual = actual->next;  // Avanza al siguiente nodo en la lista original
    }

    return nuevaLista;
}

// Muestra todos los eventos entres dos fechas determinadas
void ListarEventosEntreFechas (TFecha fe1, TFecha fe2, TLista l) {
    TLista aux;
    TLista prim;
    bool continuar;
    bool hayReservas;

    aux = CopiarLista (l);
    prim = aux;
    hayReservas = false;

    if (aux.p == NULL) {     // Lista vacía
        printf ("\n Lista vacía");
        printf ("\nNo hay reservas para el período %02d/%02d/%04d - %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
    }
    else {      // Lista con registros
        // Ordena la lista por fechas
        OrdenarListaPorFecha (&aux);

        // Formato cabecera
        printf("\n\n\n");
        printf("======================================================================================================\n");
        printf("                         Listado de Reservas entre %02d/%02d/%04d y %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
        printf("======================================================================================================\n");
        printf("| N Reserva |   Fecha    | Hora Inicio |   Hora Fin  |    Apellido    |    Nombre     |   Teléfono   |\n");
        printf("|-----------|------------|-------------|-------------|----------------|---------------|--------------|\n");
        
        // Busco en la lista ordenada
        // Valor lógico para detener el recorrido al llegar a una fecha mayor
        continuar = true;

        while (aux.p != NULL && continuar) {
            if (FechaEnRango (aux.p->r.diaReserva, fe1, fe2)) { // La fecha de reserva está en el rango
                hayReservas = true;
                printf("| %-10d| %02d/%02d/%04d |    %02d:%02d    |    %02d:%02d    |", aux.p->r.nroReserva, aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio, aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos, aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                printf("%-16s|%-15s", aux.p->r.responsable.apellido, aux.p->r.responsable.nombre);
                printf ("| %-13s|\n", aux.p->r.responsable.telefono);
            }
            else if (EsMayorFechaSinHora (aux.p->r.diaReserva, fe2)) {
                continuar = false;
            }
            aux.p = aux.p->next;
        }

        if (!hayReservas) {
            printf ("\nNo hay reservas para el período %02d/%02d/%04d - %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
        }
    }
    VaciarLista (&prim);
    return;
}

// Muestra todos los eventos e ingresos entre dos fechas determinadas
void ListarIngresosEntreFechas (TFecha fe1, TFecha fe2, TLista l) {
    TLista aux;
    TLista prim;
    float pagado;
    float adeudado;
    float total;
    bool continuar;
    bool hayReservas;

    aux = CopiarLista (l);
    prim = aux;
    hayReservas = false;

    if (aux.p == NULL) {     // Lista vacía
        printf ("\n Lista vacía");
        printf ("\nNo hay reservas para el período %02d/%02d/%04d - %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
    }
    else {    // Lista con registros
        // Ordena la lista por fechas
        OrdenarListaPorFecha (&aux);

        pagado = 0;
        adeudado = 0;
        total = 0;

        // Formato cabecera
        printf("\n\n\n");
        printf("======================================================================================================\n");
        printf("                         Listado de Reservas entre %02d/%02d/%04d y %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
        printf("======================================================================================================\n");
        printf("| N Reserva |   Fecha    | Hora Inicio |   Hora Fin  |    Apellido    |    Nombre     |   Teléfono   |\n");
        printf("|-----------|------------|-------------|-------------|----------------|---------------|--------------|\n");
        
        // Busco en la lista ordenada
        // Valor lógico para detener el recorrido al llegar a una fecha mayor
        continuar = true;


        while (aux.p != NULL && continuar) {
            if (FechaEnRango (aux.p->r.diaReserva, fe1, fe2)) { // La fecha de reserva está en el rango
                hayReservas = true;
                printf("| %-10d| %02d/%02d/%04d |    %02d:%02d    |    %02d:%02d    |", aux.p->r.nroReserva, aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio, aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos, aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                printf("%-16s|%-15s", aux.p->r.responsable.apellido, aux.p->r.responsable.nombre);
                printf ("| %-13s|\n", aux.p->r.responsable.telefono);
                total += aux.p->r.monto;
                if (aux.p->r.pagadoAdeudado) { // Si el valor fue pagado se suma a lo que fue cobrado
                    pagado += aux.p->r.monto;
                }
                else { // Si el valor es adeudado se suma a lo que resta por cobrarse
                    adeudado += aux.p->r.monto;
                }
            }
            else if (EsMayorFechaSinHora (aux.p->r.diaReserva, fe2)) {
                continuar = false;
            }
            aux.p = aux.p->next;
        }

        if (!hayReservas) {
            printf ("\nNo hay reservas para período %02d/%02d/%04d - %02d/%02d/%04d\n", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
        }
        else {
            printf ("\nPeríodo %02d/%02d/%04d - %02d/%02d/%04d", fe1.dia, fe1.mes, fe1.anio, fe2.dia, fe2.mes, fe2.anio);
            printf ("\n Total de ingresos: $ %.2f", total);
            printf ("\n Pagados: $ %.2f", pagado);
            printf ("\n Adeudados: $ %.2f\n", adeudado);
        }
    }
    VaciarLista (&prim);
    return;
}

// Muestra cuales son los horarios disponibles para una fecha en particular
void ListarDisponibilidadDia (TFecha fe, TLista l) {
    TLista aux;
    TLista prim;
    bool continuar;
    bool hayReservas;

    aux = CopiarLista (l);
    prim = aux;
    hayReservas = false;

    if (aux.p == NULL) {     // Lista vacía
        printf ("\n Lista vacía");
        printf ("\nNo hay reservas para la fecha %02d/%02d/%04d\n", fe.dia, fe.mes, fe.anio);
    }
    else {      // Lista con registros
        // Ordena la lista por fechas
        OrdenarListaPorFecha (&aux);

        // Formato cabecera
        printf("\n\n\n");
        printf("======================================================================================================\n");
        printf("                         Listado de Reservas el día %02d/%02d/%04d\n", fe.dia, fe.mes, fe.anio);
        printf("======================================================================================================\n");
        printf("| N Reserva |   Fecha    | Hora Inicio |   Hora Fin  |    Apellido    |    Nombre     |   Teléfono   |\n");
        printf("|-----------|------------|-------------|-------------|----------------|---------------|--------------|\n");

        // Busco en la lista ordenada
        // Valor lógico para detener el recorrido al llegar a una fecha mayor
        continuar = true;

        while (aux.p != NULL && continuar) {
            if (EsIgualFecha (aux.p->r.diaReserva, fe)) {
                hayReservas = true;
                printf("| %-10d| %02d/%02d/%04d |    %02d:%02d    |    %02d:%02d    |", aux.p->r.nroReserva, aux.p->r.diaReserva.dia, aux.p->r.diaReserva.mes, aux.p->r.diaReserva.anio, aux.p->r.horaInicio.hora, aux.p->r.horaInicio.minutos, aux.p->r.horaFin.hora, aux.p->r.horaFin.minutos);
                printf("%-16s|%-15s", aux.p->r.responsable.apellido, aux.p->r.responsable.nombre);
                printf ("| %-13s|\n", aux.p->r.responsable.telefono);
            }
            else if (EsMayorFechaSinHora (aux.p->r.diaReserva, fe)) {
                continuar = false;
            }
            aux.p = aux.p->next;
        }
        
        if (!hayReservas) {
            printf ("\nNo hay reservas para la fecha %02d/%02d/%04d\n", fe.dia, fe.mes, fe.anio);
        }
    }
    VaciarLista (&prim);
    return;
}

// Devuelve verdadero si la fecha tiene un formato válido
bool ValidarFecha (TFecha fe) {
    if (fe.anio < 1) {
        return false;
    }
    if (fe.mes < 1 || fe.mes > 12) {
        return false;
    }
    if (fe.dia < 1 || fe.dia > 31) {
        return false;
    }
    if ((fe.mes == 4 || fe.mes == 6 || fe.mes == 9 || fe.mes == 11) && fe.dia == 31) {
        return false;
    }
    if (fe.mes == 2 && fe.dia > 29) {
        return false;
    }
    if (fe.mes == 2 && fe.dia == 29 && !EsBisiesto(fe.anio)) {
        return false;
    }
    return true;
}

// Devuelve verdadero si la hora tiene un formato válido
bool ValidarHora (THora h) {
    if (h.hora < 0 || h.hora > 23) {
        return false;
    }
    if (h.minutos < 0 || h.minutos > 59) {
        return false;
    }
    return true;
}