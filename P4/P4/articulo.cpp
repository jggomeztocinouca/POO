#include "articulo.hpp"
#include "fecha.hpp"
#include "cadena.hpp"
#include <iomanip>
#include <cstdlib>
#include <iostream>

using namespace std;

// Constructor general con inicialización de atributos
Articulo::Articulo(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro):
setAutores(autoresParametro), codReferencia(referenciaParametro), tituloArticulo(tituloParametro), fechaPublicacion(fechaParametro), price(precioParametro){
    if(autoresParametro.empty()){
        throw Articulo::Autores_vacios();
    }
}

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

const Articulo::Autores &Articulo::autores() const {
    return setAutores;
}

// Operador de inserción de flujo
ostream& operator <<(ostream& os, const Articulo& articuloParametro){
    std::locale::global( std::locale( "" ) ); // Establece la consola con el idioma español

    os<<"["<<articuloParametro.referencia()<<"] \"";
    os<<articuloParametro.titulo()<<"\", de ";

    auto it = articuloParametro.autores().begin();
    os << (*it)->apellidos();
    while (++it != articuloParametro.autores().end()){
        os << ", " << (*it)->apellidos();
    }
    os << ". " << articuloParametro.f_publi().anno() << ". " << fixed << setprecision(2) << articuloParametro.precio() << " €\n\t";
    articuloParametro.impresion_especifica(os); // Según el tipo de Articulo se llama a una impresion.
    return os;
}


// CLASE AUTOR
// El constructor recibirá esos tres parámetros y no lanzará ninguna excepción.
Autor::Autor(const Cadena &nombreParametro, const Cadena &apellidosParametro, const Cadena &direccionParametro) noexcept: nombreAutor(nombreParametro),
                                                                                                                 apellidosAutor(apellidosParametro),
                                                                                                                 direccionAutor(direccionParametro) {}


// Habrá 3 métodos observadores llamados "nombre", "apellidos" y "direccion" que devolverán los atributos correspondientes
const Cadena &Autor::nombre() const {
    return nombreAutor;
}

const Cadena &Autor::apellidos() const {
    return apellidosAutor;
}

const Cadena &Autor::direccion() const {
    return direccionAutor;
}

// CLASE ARTICULO ALMACENABLE
// El constructor de esta clase añade un parámetro más a los de Articulo: el stock, que por defecto valdrá 0.
ArticuloAlmacenable::ArticuloAlmacenable(const Autores& autoresParametro, const Cadena &referenciaParametro, const Cadena &tituloParametro, const Fecha &fechaParametro, double precioParametro, size_t cantidad):
Articulo(autoresParametro, referenciaParametro,tituloParametro,fechaParametro,precioParametro), stockDisponible(cantidad) {}

// Método observador (lectura) del stock
size_t ArticuloAlmacenable::stock() const {
    return stockDisponible; // Devuelve, simplemente, el valor (por copia) del stock
}

// Método de lectura / escritura del stock (el compilador determinará si se devuelve como rvalue o lvalue)
size_t &ArticuloAlmacenable::stock() {
    return stockDisponible; // Devuelve una referencia al atributo donde se almacena el stock, para su lectura o escritura.
}

// CLASE LIBRO
// El constructor recibe los mismos parámetros que ArticuloAlmacenable más el número de páginas, que es el penúltimo, quedando de nuevo el stock en último lugar, para poder ponerle el valor predeterminado 0.
Libro::Libro(const Autores& autoresParametro, const Cadena &referenciaParametro, const Cadena &tituloParametro, const Fecha &fechaParametro, double precioParametro, size_t numeroPaginasParametro, size_t cantidad):
        ArticuloAlmacenable(autoresParametro, referenciaParametro,tituloParametro,fechaParametro,precioParametro,cantidad), numeroPaginas(numeroPaginasParametro){}

// El número de páginas se podrá consultar a través del observador n_pag.
size_t Libro::n_pag() const {
    return numeroPaginas;
}

// Definición del método virtual puro que se hereda de la clase abstracta Articulo
void Libro::impresion_especifica(ostream &os) const {
    os << n_pag() << " págs., " << stock() << " unidades.";
}

// CLASE CEDERRON

// El Constructor es como el de Libro pero cambiando el número de páginas por el tamaño en MB.
Cederron::Cederron(const Autores& autoresParametro, const Cadena &referenciaParametro, const Cadena &tituloParametro, const Fecha &fechaParametro, double precioParametro, size_t MBParametro, size_t cantidad):
        ArticuloAlmacenable(autoresParametro, referenciaParametro, tituloParametro,fechaParametro,precioParametro,cantidad), MB(MBParametro){}

// Observador del tamaño del Cederron
size_t Cederron::tam() const {
    return MB;
}

// Definición del método virtual puro que se hereda de la clase abstracta Articulo
void Cederron::impresion_especifica(ostream &os) const {
    os << tam() << " MB, " << stock() << " unidades.";
}

// CLASE LIBRODIGITAL

// El constructor recibe los mismos parámetros que el de Articulo más la fecha de expiración.
LibroDigital::LibroDigital(const Autores& autoresParametro, const Cadena &referenciaParametro, const Cadena &tituloParametro, const Fecha &fechaParametro, double precioParametro, const Fecha &fechaExpiracionParametro):
        Articulo(autoresParametro, referenciaParametro, tituloParametro,fechaParametro,precioParametro), fechaExpiracion(fechaExpiracionParametro){}

// Observador de la fecha de expiración
const Fecha &LibroDigital::f_expir() const {
    return fechaExpiracion;
}

// Definición del método virtual puro que se hereda de la clase abstracta Articulo
void LibroDigital::impresion_especifica(ostream &os) const {
    os << "A la venta hasta el "<< f_expir() << ".";
}