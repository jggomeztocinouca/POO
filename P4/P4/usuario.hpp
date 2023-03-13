#ifndef USUARIO_H
#define USUARIO_H

#include "cadena.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>

class Tarjeta; // Declaración adelantada
class Numero; // Declaración adelantada
class Articulo; // Declaración adelantada

using namespace std;

// ****************************** CLase Clave ******************************
class Clave{
public:
    Clave(const char* c); // Constructor de conversión a partir de una cadena de caracteres de bajo nivel

    enum Razon{CORTA, ERROR_CRYPT}; // Enumeración con constantes CORTA(1) y ERROR_CRYPT(2)

    const Cadena& clave() const; // Método observador que devuelve la clave cifrada

    bool verifica(const char* claveParametro) const; // Método que verifica si la contraseña introducida por parámetro corresponde con la almacenada

    class Incorrecta{ // Clase de excepción de Clave que almacenará el error asociado
        const Clave::Razon motivo; // Atributo que almacena el motivo de incorrección
    public:
        Incorrecta(Clave::Razon); // Constructor que recibe un enumerado de tipo Clave::Razon
        Clave::Razon razon() const; // Método observador que devuelve el motivo de la incorrección
    };

private:
    Cadena password; // Cadena que almacena la clave cifrada
};

// ****************************** CLase Usuario ******************************
class Usuario{
public:
    Usuario(const Cadena& identificador_, const Cadena& nombre_, const Cadena& apellidos_, const Cadena& direccion_, const Clave& password_); // Constructor general

    Usuario(const Usuario& user)=delete; // Eliminamos el constructor de copia, ya que no queremos tener usuarios duplicados

    Usuario(Usuario&& user)=delete; // Eliminamos el constructor de copia con movimiento, ya que no queremos tener usuarios duplicados.

    Usuario& operator=(const Usuario& u) = delete; // Eliminamos el operador de asignación-copia

    Usuario& operator=(const Usuario&& u) = delete; // Eliminamos el operador de asignación-copia con movimiento

    typedef std::map<Numero, Tarjeta*> Tarjetas; // Definición del sinónimo público del tipo

    typedef std::unordered_map<Articulo*, size_t> Articulos; // Definición del sinónimo público del tipo

    const Cadena& id() const; // Método observador del identificador del usuario

    const Cadena& nombre() const; // Método observador del nombre del usuario

    const Cadena& apellidos() const; // Método observador de los apellidos del usuario

    const Cadena& direccion() const; // Método observador de la dirección del usuario

    const Tarjetas& tarjetas() const; // Método observador del contenedor de tarjetas

    const Articulos& compra() const; // Método observador del contenedor de artículos

    class Id_duplicado{ // Clase de excepción Id_duplicado
        const Cadena& id_repetido; // Identificador duplicado
    public:
        Id_duplicado(const Cadena& id); // Constructor de conversión
        const Cadena& idd() const; // Método observador del ID duplicado
    };


    void es_titular_de(const Tarjeta& card); // Método de asociación de tarjetas

    void no_es_titular_de(Tarjeta& card); // Método de deasociación de tarjetas

    ~Usuario(); // Destructor de usuarios

    void compra(Articulo& a, size_t cantidad=1); // Método de asociación de articulos

    size_t n_articulos() const; // Método observador de número de artículos

    friend ostream& operator << (ostream& os, const Usuario& user); // Operador de inserción de flujo de usuario
    // Se declara friend porque debemos acceder al método observador del atributo de tipo Clave, que es privado respecto al contexto.

private:
    const Cadena identifier; // Identificador del usuario
    const Cadena name; // Nombre del usuario
    const Cadena surname; // Apellidos del usuario
    const Cadena address; // Dirección del usuario
    Clave password; // Contraseña del usuario
    Articulos items; // En realidad Artículos es una definición de tipo que viene a ser std::unordered_map<Articulo*, size_t>
    Tarjetas cards; // En realidad Tarjetas es una definición de tipo que viene a ser std::map<Numero, Tarjeta*>
    static std::unordered_set<Cadena> users;
};

void mostrar_carro(ostream& os, const Usuario& user); // Método de inserción de flujo de artículos

#endif //USUARIO_H


