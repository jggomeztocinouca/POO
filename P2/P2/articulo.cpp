#include "articulo.hpp"
#include "fecha.hpp"
#include "cadena.hpp"
#include <iomanip>
#include <cstdlib>
#include <iostream>

using namespace std;

// Constructor general con inicialización de atributos
Articulo::Articulo(const Cadena &c, const Cadena &t, const Fecha &f, double p, size_t s): codReferencia(c), tituloArticulo(t), fechaPublicacion(f), price(p), stockDisponible(s) {}

// Método observador del código de referencia
const Cadena &Articulo::referencia() const {
    return codReferencia; // Devuelve una referencia constante al objeto Cadena que almacena el código de referencia.
}

// Método observador del título
const Cadena &Articulo::titulo() const {
    return tituloArticulo; // Devuelve una referencia constante al objeto Cadena que almacena el título.
}

// Método observador de la fecha de publicación
const Fecha &Articulo::f_publi() const {
    return fechaPublicacion; // Devuelve una referencia constante al objeto Fecha que almacena la fecha de publicación.
}

// Método observador (lectura) del precio
double Articulo::precio() const {
    return price; // Devuelve, simplemente, el valor (por copia) del precio
}

// Método de lectura / escritura del precio (el compilador determinará si se devuelve como rvalue o lvalue)
double &Articulo::precio() {
    return price; // Devuelve una referencia al atributo donde se almacena el precio, para su lectura o escritura.
}

// Método observador (lectura) del stock
size_t Articulo::stock() const {
    return stockDisponible; // Devuelve, simplemente, el valor (por copia) del stock
}

// Método de lectura / escritura del stock (el compilador determinará si se devuelve como rvalue o lvalue)
size_t &Articulo::stock() {
    return stockDisponible; // Devuelve una referencia al atributo donde se almacena el stock, para su lectura o escritura.
}
// Operador de inserción de flujo
ostream& operator <<(ostream& os, const Articulo& a){
    std::locale::global( std::locale( "" ) ); // Establece la consola con el idioma español

    // Se inserta una cadena de caracteres de bajo nivel con el siguiente formato --> [110] "Fundamentos de C++", 1998. 29,95 €
    os << "["
    << a.referencia()
    << "]\""
    << a.titulo()
    << "\", "
    << a.f_publi().anno()
    << ". "
    << fixed  // Ajusta el formato a fixed
    << setprecision(2) // Ajusta los decimales del precio a 2
    << a.precio()
    << " €.";
    return os;
}








