#include "string.h"
#include <iostream>
#include <ctime>
#include "Fecha.h"
using namespace std;

bool Fecha::bisiesto(int a){
	return ((a % 4 == 0) && (a % 400 == 0 || a % 100 != 0)); // Expresión matemática que comprueba si el año dado es bisiesto
}

bool Fecha::treintauno(int mes){
	if (mes%2==0){ // Los meses en la clase ctime se empiezan a contar, siendo los meses (en vez de los imparez como ordinariamente) pares los que tienen 31 días (Enero [0], Marzo[2], Mayo[4], etc)
		return true;
	}else{
		return false;
	}
}

Fecha::Fecha()
{
	dia = ltm->tm_mday;		   // El contenido de tm_mday (apuntado por la instancia ltm del struct tm) devuelve el día del sistema.
	mes = 1 + ltm->tm_mon;	   // El contenido de tm_mon (apuntado por la instancia ltm del struct tm) devuelve el mes del sistema.
	ano = 1900 + ltm->tm_year; // El contenido de tm_year (apuntado por la instancia ltm del struct tm) devuelve el año del sistema.
}

Fecha::Fecha(int a)
{							   // Hacer comprobaciones de datos introducidos
	mes = 1 + ltm->tm_mon;	   // El contenido de tm_mon (apuntado por la instancia ltm del struct tm) devuelve el mes del sistema.
	ano = 1900 + ltm->tm_year; // El contenido de tm_year (apuntado por la instancia ltm del struct tm) devuelve el año del sistema.
	
	if (a == 0) // Comprueba que el número pasado sea 0. En tal caso se obtiene el día del sistema.
	{
		dia = ltm->tm_mday; //Obtiene el día del sistema
	}else if(mes==1){ // Comprueba si el mes es Febrero (caso especial --> 28 o 29 días)
		if((bisiesto(ano))&&(dia>0)&&(dia<30)){ //Comprueba si el año es bisiesto y, en tal caso, si está comprendido entre los días 0 y 29.
			dia = a; // Dia pasado por parametro
		}else if((!bisiesto(ano))&&(dia>0)&&(dia<29)){ //Comprueba si el año es no bisiesto y, en tal caso, si está comprendido entre los días 0 y 28.
			dia = a; // Dia pasado por parametro
		}
	}
	else
	{
		if((treintauno(mes))&&(dia>0)&&(dia<32)){ // Comprueba que el mes pasado tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 31 (incluidos)
			dia = a; // Día pasado por parametro
		}else if((!treintauno(mes))&&(dia>0)&&(dia<31)){ // Comprueba que el mes pasado no tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 30 (incluidos)
			dia = a; // Dia pasado por parametro
		}else{
			// Lanzar excepción
		}	
	};
}

Fecha::Fecha(int a, int b)
{							   // Hacer comprobaciones de datos introducidos
	ano = 1900 + ltm->tm_year; // El contenido de tm_year (apuntado por la instancia ltm del struct tm) devuelve el año del sistema.
	
	if (b == 0) // Comprueba que el mes pasado sea 0. En tal caso a mes se le asignará el mes del sistema.
	{
		mes = ltm->tm_mon;
	}
	else
	{
		if((mes>=0)&&(mes<12)){ // Comprueba que el mes dado se encuentre entre 0 y 11 (siendo 0 Enero y 11 Diciembre)
			mes = b; // Mes pasado por parametro
		}else{
			// Lanzar excepción
		}
	};

	if (a == 0) // Comprueba que el número pasado sea 0. En tal caso se obtiene el día del sistema.
	{
		dia = ltm->tm_mday; //Obtiene el día del sistema
	}else if(mes==1){ // Comprueba si el mes es Febrero (caso especial --> 28 o 29 días)
		if((bisiesto(ano))&&(dia>0)&&(dia<30)){ //Comprueba si el año es bisiesto y, en tal caso, si está comprendido entre los días 0 y 29.
			dia = a; // Dia pasado por parametro
		}else if((!bisiesto(ano))&&(dia>0)&&(dia<29)){ //Comprueba si el año es no bisiesto y, en tal caso, si está comprendido entre los días 0 y 28.
			dia = a; // Dia pasado por parametro
		}
	}
	else
	{
		if((treintauno(mes))&&(dia>0)&&(dia<32)){ // Comprueba que el mes pasado tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 31 (incluidos)
			dia = a; // Día pasado por parametro
		}else if((!treintauno(mes))&&(dia>0)&&(dia<31)){ // Comprueba que el mes pasado no tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 30 (incluidos)
			dia = a; // Dia pasado por parametro
		}else{
			// Lanzar excepción
		}	
	};
}

Fecha::Fecha(int a, int b, int c)
{ // Hacer comprobaciones de datos introducidos
	if (c == 0) // Comprueba que el año pasado sea 0. En tal caso a año se le será asginado el año del sistema.
	{
		ano = ltm->tm_year;
	}
	else
	{
		if((ano>=AnnoMinimo)&&(ano<=AnnoMaximo)){ // Comprueba que año entre los valores máximos predefinidos (AnnoMinimo y AnnoMaximo). En tal caso ano recibirá el año del sistema.
			ano = c; // Mes pasado por parametro
		}else{
			// Lanzar excepción
		}
	};

	if (b == 0) // Comprueba que el mes pasado sea 0. En tal caso a mes se le asignará el mes del sistema.
	{
		mes = ltm->tm_mon;
	}
	else
	{
		if((mes>=0)&&(mes<12)){ // Comprueba que el mes dado se encuentre entre 0 y 11 (siendo 0 Enero y 11 Diciembre)
			mes = b; // Mes pasado por parametro
		}else{
			// Lanzar excepción
		}
	};

	if (a == 0) // Comprueba que el número pasado sea 0. En tal caso se obtiene el día del sistema.
	{
		dia = ltm->tm_mday; //Obtiene el día del sistema
	}else if(mes==1){ // Comprueba si el mes es Febrero (caso especial --> 28 o 29 días)
		if((bisiesto(ano))&&(dia>0)&&(dia<30)){ //Comprueba si el año es bisiesto y, en tal caso, si está comprendido entre los días 0 y 29.
			dia = a; // Dia pasado por parametro
		}else if((!bisiesto(ano))&&(dia>0)&&(dia<29)){ //Comprueba si el año es no bisiesto y, en tal caso, si está comprendido entre los días 0 y 28.
			dia = a; // Dia pasado por parametro
		}
	}
	else
	{
		if((treintauno(mes))&&(dia>0)&&(dia<32)){ // Comprueba que el mes pasado tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 31 (incluidos)
			dia = a; // Día pasado por parametro
		}else if((!treintauno(mes))&&(dia>0)&&(dia<31)){ // Comprueba que el mes pasado no tenga 31 días. En tal caso comprueba si el día dado se encuentra entre 1 y 30 (incluidos)
			dia = a; // Dia pasado por parametro
		}else{
			// Lanzar excepción
		}	
	};
}

Fecha::Fecha(const Fecha &f)
{				 
	dia = f.dia; // Dia contenido en la instancia f de la clase Fecha
	mes = f.mes; // Mes contenido en la instancia f de la clase Fecha
	ano = f.ano; // Año contenido en la instancia f de la clase Fecha
}

Fecha::operator const char *() const
{
	char *vectorChar = new char[50];
	tm t = {};
	t.tm_mday = dia;
	t.tm_mon = mes - 1;
	t.tm_year = ano - 1900;

	mktime(&t);
	strftime(vectorChar, 50, "%A, %d de %B de %Y", &t);

	return vectorChar;
}

void Fecha::imprimirFecha() const
{
	cout << dia << "/" << mes << "/" << ano << endl;
}
