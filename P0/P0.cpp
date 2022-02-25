#include <iostream>
#include "Fecha.cpp"

using namespace std;

int main(){
    int a = 30;
    int a1 = 29;
    int a2 = 28;
    int a3 = 27;
    int b = 1;
    int c = 2002;
    Fecha f1;
    f1.imprimirFecha();
    Fecha f2(a); // Prueba de constructor con 1 parámetro (día)
    // f2 = a1; Restringir | Preguntar
    // f2(a2); Comprobado que está restringido 
    // f2(Fecha(a3)); Hacer posible esta declaración | Preguntar
    f2.imprimirFecha();
    Fecha f3(a,b); // Prueba de constructor con 2 parámetros (día y mes)
    f3.imprimirFecha();
    Fecha f4(a,b,c); // Prueba de constructor con 3 parámetros (día, mes y año)
    f4.imprimirFecha();
    Fecha f5(f3); // Prueba de constructor a partir de otro objeto (pasado por parámetro)
    f5.imprimirFecha();
    Fecha f6 = f1; // Prueba de constructor a partir de otro objeto (igualación)
    f6.imprimirFecha();
    return 0;
}