#ifndef CADENA_H
#define CADENA_H

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <string>
#include <iterator>

using namespace std;

class Cadena{
private:
    char* s_{}; // Puntero de tipo char que almacenará el array dinámico de caracteres
    size_t tam_ = 0; //size_t is commonly used for array indexing and loop counting.
    // Programs that use other types, such as unsigned int, for array indexing may fail on,
    // e.g. 64-bit systems when the index exceeds UINT_MAX or if it relies on 32-bit modular arithmetic.
public:
    // -- Constructores --
    explicit Cadena(size_t i=0, char c = ' '); // Constructor con tamaño y carácter dado. Ej: 4,x --> s_ = "xxxx"
    Cadena(const Cadena& cad); // Constructor de copia de cadenas
    /*explicit*/ Cadena(const char* cad); // Constructor a partir de una cadena de caracteres de bajo nivel (const char* cad)
    Cadena(Cadena &&cad) noexcept; // Constructor de movimiento
    ~Cadena(); // Destructor
    const char* c_str() const;

    // -- Operadores de asignación --
    Cadena& operator = (const Cadena& cad); // Reemplaza la cadena actual con otra cadena pasada por parámetro
    Cadena& operator = (Cadena &&cad) noexcept; // Operador de asignación - movimiento
    Cadena& operator = (const char* c); // Reemplaza la cadena actual con una cadena de caracteres de bajo nivel
    Cadena& operator +=(const Cadena& cad); // Concatena a la cadena actual otra cadena pasada por parámetro

    // -- Operadores de índice --
    const char& operator [](size_t i) const; // Sobrecarga del operador índice para acceder (a un miembro privado s_[i]) desde la interfaz pública al elemento deseado (devuelve el valor por referencia).
    char& operator [](size_t i); // Sobrecarga del operador índice habilitado para lectura y escritura (no es const). Si se llama a este operador desde un objeto const, accedería a la función constante. De lo contrario se accedería a esta.
    char& at(size_t i) const; // Ídem del operador anterior, pero comprueba si el índice indicado está dentro del rango de la cadena. Si lo excediera lanzaría una excepción.
    char& at(size_t i); // Sobrecarga en versión constante. Si se llama a este operador desde un objeto const, accedería a la función constante. De lo contrario se accedería a esta.

    // -- Funciones auxiliares --
    Cadena substr(size_t desplazamiento, size_t tamano) const; // Crea una cadena SUBSTRaida de otra a partir de un desplazamiento y un tamaño dado.

    // -- Funciones observadoras
    size_t length() const {return tam_;}; // Devuelve el tamaño del array dinámico

    // Tipos de iterators
    // ¿Qué son los iterators?
    // Los iterators son definidos en los contenedores de la librería estándar y nos permiten recorrerlos de forma homogénea (siempre igual).
    //
    typedef char* iterator; // Definimos el tipo iterator de tipo puntero carácter (ya que trabajamos con cadena de caracteres)
    typedef const char* const_iterator; // Definimos el tipo iterator constante de tipo puntero carácter (ya que trabajamos con cadena de caracteres)
    typedef std::reverse_iterator<iterator> reverse_iterator; // Definimos el tipo iterator reverso de tipo puntero carácter (ya que trabajamos con cadena de caracteres)
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; // Definimos el tipo iterator reverso constante de tipo puntero carácter (ya que trabajamos con cadena de caracteres)

    // Funciones de iterators
    // Formas de recorrer un contenedor
    // De principio a fin --> [begin(), end())
    // end() no es incluido dentro del rango, ya que esta función está pensada para devolver una posición posterior al contenedor.
    // Definido este comportamiento, el recorrido dentro del contenedor se realizará mientras que nuestro iterator sea distinto a end().
    // De fin a principio --> [rend(), rbegin())
    // rbegin() no es incluido dentro del rango, ya que esta función está pensada para devolver una posición anterior al contenedor.
    // Definido este comportamiento, el recorrido dentro del contenedor se realizará mientras que nuestro iterator sea distinto a rbegin().
    iterator begin() {return s_;} // Devuelve un puntero al primer elemento de nuestro contenedor (cadena de caracteres)
    const_iterator begin() const {return const_iterator(s_);} // Devuelve un puntero al primer elemento de nuestro contenedor (cadena de caracteres), pero no permite la modificación del valor de este (const).
    const_iterator cbegin() const {return const_iterator(s_);} // Devuelve un puntero al primer elemento de nuestro contenedor, pero no permite la modificación del valor de este (const).
    iterator end() {return s_ + tam_;} // Devuelve un puntero al carácter terminador \0.
    const_iterator end() const {return const_iterator(s_ + tam_);} // Devuelve un puntero al carácter terminador \0, pero no permite la modificación del valor de este (const).
    const_iterator cend() const {return const_iterator(s_ + tam_);} // Devuelve un puntero al carácter terminador \0, pero no permite la modificación del valor de este (const).
    reverse_iterator rbegin() {return reverse_iterator(end());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al carácter terminador \0.
    const_reverse_iterator rbegin() const {return const_reverse_iterator(end());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al carácter terminador \0, pero no permite la modificación del valor de este (const).
    const_reverse_iterator crbegin() const {return const_reverse_iterator(end());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al carácter terminador \0, pero no permite la modificación del valor de este (const).
    reverse_iterator rend() {return reverse_iterator(begin());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al primer elemento del contendor.
    const_reverse_iterator rend() const {return const_reverse_iterator(begin());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al primer elemento del contendor, pero no permite la modificación del valor de este (const).
    const_reverse_iterator crend() const {return const_reverse_iterator(begin());} // Iterator para recorrer inversamente un contenedor. Devuelve un puntero al primer elemento del contendor, pero no permite la modificación del valor de este (const).


};

// -- Operadores aritméticos --
Cadena operator + (const Cadena& cad1, const Cadena& cad2); // Concatena dos cadenas en una nueva creada (que será devuelta por referencia)

// -- Operadores lógicos --
bool operator==(const Cadena& c1, const Cadena& c2); // Compara si dos cadenas son iguales
bool operator!=(const Cadena& c1, const Cadena& c2); // Comprueba que dos cadenas no son iguales
bool operator >(const Cadena& c1, const Cadena& c2); // Comprueba que la cadena 1 es mayor (alfabéticamente) que la cadena 2
bool operator <(const Cadena& c1, const Cadena& c2); // Comprueba que la cadena 1 es menor (alfabéticamente) que la cadena 2 (o que la cadena 2 es mayor que la cadena 1)
bool operator>=(const Cadena& c1, const Cadena& c2); // Comprueba que la cadena 1 es mayor o igual (alfabéticamente) que la cadena 2
bool operator<=(const Cadena& c1, const Cadena& c2); // Comprueba que la cadena 1 es menor o igual que la cadena 2 (o que la cadena 2 es mayor o igual que la cadena 1)

// Operadores de flujo
std::ostream& operator<<(std::ostream& os, const Cadena& cad); // Operador de inserción (de flujo)
std::istream& operator>>(std::istream& is, Cadena& cad); // Operador de extracción (de flujo)

// Para P2 y ss.
// Especialización de la plantilla hash<T>para definir la 9
// función hash a utilizar con contenedores desordenados de
// Cadena, unordered_[set|map|multiset|multimap].
namespace std { // Estaremos dentro del espacio de nombres std
template <> // Es una especialización de una plantilla para Cadena
struct hash<Cadena> { // Es una clase con solo un operador publico
size_t operator() (const Cadena& cad) const // El operador función
{
hash<string> hs; // Creamos un objeto hash de string
const char* p = cad.c_str(); // Obtenemos la cadena de la Cadena
string s(p); // Creamos un string desde una cadena
size_t res = hs(s); // El hash del string. Como hs.operator()(s);
return res; // Devolvemos el hash del string
}
};
}

#endif //CADENA_H

