#include <iostream>
#include "string.h"

using namespace std;

class Fecha
{
private:
	time_t now = time(0);	   // Devuelve el número de segundos que han pasado desde el 1 de Enero de 1970
	tm *ltm = localtime(&now); // TAD TM (sólo necesitamos saber cómo funciona, no su implementación)
	int dia = 0;
	int mes = 0;
	int ano = 0;
	static const int AnnoMinimo = 1902;
	static const int AnnoMaximo = 2037;

public:
	bool bisiesto(int a); // Método que comprueba si el año pasado por parámetro es bisiesto
	bool treintauno(int mes);
	Fecha();
	Fecha(int a);				// 1 parámetro dado (día), mes y año tomados del sistema
	Fecha(int a, int b);		// dos parámetros dados (día, mes), año tomado del sistema
	Fecha(int a, int b, int c); // 3 parámetros dados (día, mes, año)
	Fecha(const Fecha &f);		// Fecha construida a partir de otra fecha pasada por parámetro (constructor de copia)
	// Como argumento se pasa una referencia constante al objeto
	// Siempre se hace así para constructores con parámetros que reciben la propia clase y para trabajar con plantillas
	// Evita la copia, mejora la eficiencia de memoria y trabaja directamente sobre una zona de memoria, evitando la modificación (const)
	operator const char *() const; // Definición del constructor de conversión
	void imprimirFecha() const;
};


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
