
#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <cstring>

using namespace std;

class Fecha
{
private:
    time_t now = time(nullptr);	   // Devuelve el número de segundos que han pasado desde el 1 de Enero de 1970 (basado en la fecha del sistema)
    tm *ltm = localtime(&now); // Convierte los segundos obtenidos en días, meses y años.
    /* -- Estructura del Struct TM --
     * struct tm {
    int tm_sec;   // seconds of minutes from 0 to 61
    int tm_min;   // minutes of hour from 0 to 59
    int tm_hour;  // hours of day from 0 to 24
    int tm_mday;  // day of month from 1 to 31
    int tm_mon;   // month of year from 0 to 11
    int tm_year;  // year since 1900
    int tm_wday;  // days since sunday
    int tm_yday;  // days since January 1st
    int tm_isdst; // hours of daylight savings time
    }*/

    int dd=0; // Variable que almacena el día
    int mm=0; // Variable que almacena el mes
    int aaaa=0; // Variable que almacena el año


    int diaMax() const; // Función que devuelve el día máximo según el mes

    int valida() const; // Comprueba que la fecha construida es válida y devuelve códigos de errores

    void comprobar(); // Comprueba el código de error y evalúa la excepción

    void corregir(); // Corrige la fecha en caso de que sea inválida.

public:
    static const int AnnoMinimo = 1902; // Año mínimo
    static const int AnnoMaximo = 2037; // Año máximo

    int dia() const; // Función observadora que devuelve el día
    int mes() const; // Función observadora que devuelve el mes
    int anno() const; // Función observadora que devuelve año

    class Invalida{ // Clase Invalida para gestionar las excepciones y códigos/mensajes de error
    private:
        const char* errorInfo; // Almacena el mensaje de error
        // const char* es un mutable puntero a un inmutable cadena.
        // No puedes cambiar el contenido de la(s) ubicación(es) a la que apunta este puntero.
        // Además, los compiladores están obligados a dar mensajes de error cuando lo intentas.
        // Por la misma razón, la conversión de const char * a char* está desaprobado.

    public:
        explicit Invalida(const char *Info); // Constructor que recibe un mensaje de error (para luego inicializar errorInfo) &&// setCX //getX
        const char* por_que() const; // Función que devolverá el mensaje de error (errorInfo)
    };

    explicit Fecha(int a=0, int b=0, int c=0) ; // Funciona como constructor por defecto (0 parámetros), constructor de conversión (1 parámetro), o constructor general (+1 parámetro)

    const char * cadena() const; // Definición del constructor de conversión --> Convertido a función explícita
    Fecha(const char* cadena); // Constructor a partir de una cadena de caracteres de bajo nivel

    // Operadores aritméticos miembros
    Fecha& operator+=(int i);   // Operador suma-igual
    Fecha& operator-=(int i);  // Operador resta-igual
    Fecha& operator++();      // Operador pre-incremento
    Fecha operator++(int);  // Operador post-incremento
    Fecha& operator--();    // Operador pre-incremento
    Fecha operator--(int);// Operador post-incremento
    
};

// Operadores aritméticos externos
Fecha operator+(const Fecha& f, int i);
Fecha operator-(const Fecha& f, int i);

// Operadores lógicos
bool operator==(const Fecha& f1, const Fecha& f2); // Operador igual que
bool operator!=(const Fecha& f1, const Fecha& f2); // Operador distinto que
bool operator> (const Fecha& f1, const Fecha& f2); // Operador mayor que
bool operator< (const Fecha& f1, const Fecha& f2); // Operador menor que
bool operator>=(const Fecha& f1, const Fecha& f2); // Operador mayor o igual que
bool operator<=(const Fecha& f1, const Fecha& f2); // Operador menor o igual que


/* Funcionamiento del constructor de conversion
Surge la necesidad de crear un constructor de conversión cuando queramos asignar, pasar u operar datos de distintos tipos
Por ejemplo
class Ejemplo {
	int x;
}

int main(){
	Ejemplo e;
	int i = e; // Error --> Distintos tipos
}

Para ello, se define en la clase a trabajar un constructor de conversión de la siguiente manera:
inline operator tipo_de_dato(){return x};
*/

// Operadores de flujo
std::ostream& operator<<(std::ostream& salida, const Fecha& f);
std::istream& operator>>(std::istream& entrada, Fecha& f);

#endif //FECHA_H
