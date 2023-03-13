#ifndef ARTICULO_HPP
#define ARTICULO_HPP

#include "fecha.hpp"
#include "cadena.hpp"

using namespace std;

// *** Clase Artículo ***
class Articulo{
    const Cadena codReferencia; // Almacena el código de referencia del artículo
    const Cadena tituloArticulo; // Almacena el título del artículo
    const Fecha fechaPublicacion; // Almacena la fecha de publicación del artículo
    double price; // Almacena el precio del Artículo
    size_t stockDisponible; // Almacena el stock disponible del artículo
public:
    Articulo(const Cadena& c, const Cadena& t, const Fecha& f, double p, size_t s=0); // Constructor general
    const Cadena& referencia() const; // Método observador del código de referencia
    const Cadena& titulo() const; // Método observador del título
    const Fecha& f_publi()  const; // Método observador de la fecha de publicación
    double precio() const; // Método observador (lectura) del precio
    double& precio(); // Método de lectura / escritura del precio (el compilador determinará si se devuelve como rvalue o lvalue)
    size_t stock() const; // Método observador (lectura) del stock
    size_t& stock(); // Método de lectura / escritura del stock (el compilador determinará si se devuelve como rvalue o lvalue)
};

ostream& operator <<(ostream& os, const Articulo& a); // Operador de inserción de flujo

#endif //ARTICULO_HPP
