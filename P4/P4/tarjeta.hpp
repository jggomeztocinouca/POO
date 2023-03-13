#ifndef TARJETA_HPP
#define TARJETA_HPP

#include "cadena.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
#include "usuario.hpp"
#include <set>

using namespace std;

class Usuario; // Declaración adelantada
class Clave; // Declaración adelantada

// *** Clase Número ***
// Esta clase representa el número troquelado en el anverso de una tarjeta de crédito.
class Numero{
public:
    // El constructor recibirá como parámetro esa Cadena con el número.
    Numero(const Cadena& numeroParametro); // Constructor de conversión a partir de una Cadena con el número de la tarjeta

    // Se definirá la enumeración Razon con los elementos LONGITUD, DIGITOS y NO_VALIDO, para representar por qué un Numero no es válido.
    enum Razon{LONGITUD,DIGITOS,NO_VALIDO}; // Enumerado con 3 valores

    // La clase Incorrecto, con un atributo de tipo Numero::Razon, el constructor que
    // recibe una Razon como parámetro, y el método observador razon que devuelve el atributo
    class Incorrecto { // Clase de excepción Numero::Incorrecto
        const Numero::Razon motivo; // Atributo de tipo Numero::Razon (Razon es un tipo de dato enumerado definido en Numero)
    public:
        Incorrecto(const Numero::Razon r); // Constructor de conversión a partir de un tipo de dato Numero::Razon
        Numero::Razon razon() const; // Método observador del motivo de la incorrección.
    };

    // Clases de objetos función
    class EsDigito: public unary_function<char,bool>
    {
    public:
        bool operator()(char c) const {return isdigit(c);}
    };

    // La clase Numero tendrá también un operador de conversión a cadena de caracteres constantes de bajo nivel.
    operator const char* () const; // Operador de conversión

private:
    // Se almacenará este dato como un atributo de tipo Cadena, ya que este «número» puede
    // tener espacios de separación al principio, al final o, más normal, en medio.
    Cadena numero; // Atributo Cadena que almacena el número de la tarjeta (con o sin espacios)
};

// *** Operadores externos de la clase Número ***
// Deberá definirse el operador «menor-que» para dos objetos de la clase.
bool operator <(const Numero& n1, const Numero& n2); // Operador de comparación (menor que) entre dos objetos de tipo Número.


// *** Clase Tarjeta ***
class Tarjeta{
public:
    // Se definirá una enumeración pública de nombre Tipo con los valores Otro, VISA, Mastercard, Maestro, JCB y AmericanExpress.
    enum Tipo{Otro, VISA, Mastercard, Maestro, JCB, AmericanExpress}; // Definición de tipo enumerado "Tipo"

    // Una tarjeta se construye únicamente a partir del Número, el Usuario y la Fecha de caducidad.
    Tarjeta(const Numero& numero, Usuario& usuario, const Fecha& fecha); // Constructor general

    // Véase el contexto en la definición del constructor
    // Esta clase de excepción tendrá un atributo que almacenará la fecha caducada, un constructor
    // que la reciba como parámetro y un método observador cuando(), que la devolverá.
    // Este tipo de excepción se lanzará cuando se cree una Tarjeta con una fecha de caducidad anterior a la de hoy.
    class Caducada{
    public:
        Caducada(const Fecha& f); // Constructor de conversión a partir de una Fecha
        const Fecha& cuando() const; // Método observador de la fecha caducada
    private:
        const Fecha caducidad; // Atributo que guarda la Fecha caducada
    };

    // Esta clase de excepción, cuyo constructor recibe el Numero en cuestión, tiene un atributo que es el Numero,
    // y un método observador que() que lo devuelve.
    class Num_duplicado{
    public:
        Num_duplicado(const Numero& n); // Constructor de conversión a partir de un Numero
        const Numero& que() const; // Método observador del Numero duplicado
    private:
        const Numero duplicado; // Atributo que guarda el Numero duplicado
    };

    // Dos tarjetas no se pueden copiar, ni al crearse ni por asignación. Esto sería ilegal, no puede haber dos tarjetas iguales.
    Tarjeta(const Tarjeta& t)=delete; // Se especifica que no se cree el constructor de copia
    Tarjeta(const Tarjeta&& t)=delete; // Se especifica que no se cree el constructor de copia con movimiento
    Tarjeta& operator=(const Tarjeta& t)= delete;
    Tarjeta& operator=(const Tarjeta&& t)= delete;

    // Habrá un método observador para cada atributo, que lo devuelva. Se llamarán numero, titular, caducidad y activa.
    const Numero& numero() const;
    const Usuario* titular() const;
    const Fecha& caducidad() const;
    bool activa() const;

    // El método observador tipo devolverá el Tipo de la Tarjeta.
    Tipo tipo() const;

    // Habrá también un método modificador para activar o desactivar una Tarjeta.
    // Se llamará activa (siendo por tanto una sobrecarga del método observador anterior)
    // y podrá recibir un parámetro booleano, cuyo valor predeterminado será true, y devolverá un booleano
    //que será el nuevo valor después de la activación o desactivación.
    bool& activa(bool);

    // Se definirá también una clase de excepción Tarjeta::Desactivada, vacía, para uso posterior en otros casos de uso.
    class Desactivada{};


    // Cuando un Usuario se destruya, sus tarjetas asociadas seguirán «vivas», aunque obviamente ya no pertenezcan a nadie.
    // Por lo tanto, la clase Tarjeta tendrá un método modificador llamado anula_titular que
    // dará al puntero que representa al titular el valor nulo y desactivará la Tarjeta poniendo a false el atributo correspondiente.
    // El destructor de la clase Usuario llamará a este método para cada una de sus tarjetas (se da a entender que tenemos que definir
    // nuestro propio destructor para la clase Usuario)
    void anula_titular();

    // Al destruirse un objeto de tipo Tarjeta deberá desasociarse de su Usuario, llamando a
    // Usuario::no_es_titular_de sobre su titular, en caso de que este no haya sido destruido
    // previamente; de lo contrario, la Tarjeta habrá sido desligada de su Usuario al ser destruido este.
    ~Tarjeta();

private:
    // Sus atributos son:
    // • un Numero constante, que es el número de la tarjeta tal como viene troquelado;
    // • un puntero a Usuario constante, que es su titular;
    // • una Fecha constante, que es la de caducidad,
    // • y un booleano, que representa si la Tarjeta está activa o no.
    const Numero numeroTarjeta;
    Usuario* const titularTarjeta; // Puntero constante a un usuario
    // const Usuario* titularTarjeta; // Puntero a usuario constante
    const Fecha caducidadTarjeta;
    bool tarjetaActiva;

    // Comprobación de tarjeta duplicada
    // Para evitar la duplicidad de tarjetas, guardaremos los Numero en un conjunto ordenado (set) estático privado.
    // Como un conjunto no admite elementos repetidos, al insertar en él un Numero que ya esté,
    // el método set::insert, que devuelve un par (clase pair) formado por un iterador apuntando al sitio de la inserción
    // y un booleano, devolverá el par (set::end(), false), así que se puede comprobar por
    // ejemplo el segundo (second) valor del par: si da falso es que el Numero ya existía en el
    // conjunto y por tanto está duplicado.
    static std::set<Numero> num;

};

// *** Operadores externos de la clase Tarjeta ***
// Se sobrecargará el operador de inserción en flujo (<<) para mostrar o imprimir una Tarjeta en un flujo de salida.
ostream& operator <<(ostream& salida, const Tarjeta& t);
ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& tipo);

// Dos Tarjeta podrán ordenarse por sus números. Para ello tendrá que definir el operador menor-que de dos tarjetas
bool operator < (const Tarjeta& t1, const Tarjeta& t2);

#endif //TARJETA_HPP
