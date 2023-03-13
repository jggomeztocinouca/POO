#ifndef ARTICULO_HPP
#define ARTICULO_HPP

#include <set>
#include "fecha.hpp"
#include "cadena.hpp"

using namespace std;

// Deberá crear la clase Autor para guardar los datos de los autores (nombre, apellidos y dirección).

// Esta clase se ha omitido en el diagrama de la figura 1, ya que el estudiante deberá decidir cómo se relaciona esta con las otras clases.
// Nótese que lo único que nos interesa es saber cuáles son los autores de un artículo dado. --> Un artículo no puede existir sin un producto (artículo)
// Todo artículo tendrá al menos uno; de no ser así, se lanzará la excepción Articulo::Autores_vacios. --> Se debe crear una clase de excepción Autores_vacios en la clase Articulo
// Dicha información (los autores) será proporcionada cuando se cree el artículo, sea del tipo que sea, y no se podrá cambiar posteriormente.

class Autor{
    Cadena nombreAutor;
    Cadena apellidosAutor;
    Cadena direccionAutor;
public:
    // El constructor recibirá esos tres parámetros y no lanzará ninguna excepción.
    Autor(const Cadena& nombreParametro, const Cadena& apellidosParametro, const Cadena& direccionParametro) noexcept;

    // Habrá 3 métodos observadores llamados nombre, apellidos y direccion que devolverán los atributos correspondientes
    const Cadena& nombre() const;
    const Cadena& apellidos() const;
    const Cadena& direccion() const;
};

// *** Clase Artículo ***
// _MODIFICACIÓN DE LA P4_
// Defina esta clase como abstracta, de forma que no se puedan crear objetos de ella --> Para que una clase sea abstracta, deberá tener definido un método virtual puro.
// ¿Qué es un método virtual puro? --> Es una función que no tiene implementación en la propia clase, sino que espera dicha implementación definida en las respectivas clases derivadas.
// ¿Cómo definir un método virtual puro? --> Introducir el identificador "virtual" al principio de la DECLARACIÓN de la función, e indicar "=0" al final de esta.

class Articulo{
public:
    // DECLARACIÓN DEL MÉTODO VIRTUAL PURO --> Al tener este método, la clase será declarada como abstracta y no se podrá crear objetos de ella.
    /* Identificador virtual --> */ virtual void impresion_especifica(std::ostream&) const = 0; /* <-- = 0 */

    // Asimismo, definirá públicamente en Articulo un sinónimo Autores con el tipo necesario para representar la relación.
    typedef std::set<Autor*> Autores;
    virtual ~Articulo(){} // Destructor --> El destructor de la clase raíz tiene que ser siempre virtual?

private:
    Autores setAutores;
    const Cadena codReferencia; // Almacena el código de referencia del artículo
    const Cadena tituloArticulo; // Almacena el título del artículo
    const Fecha fechaPublicacion; // Almacena la fecha de publicación del artículo
    double price; // Almacena el precio del Artículo
    // size_t stockDisponible; SE TRASPASA A LA CLASE DE ARTÍCULOS ALMACENABLES, QUE SÍ PUEDEN TENER STOCK (SUPONEMOS QUE LOS ARTÍCULOS DIGITALES TIENEN UNA CANTIDAD ILIMITADA)
    // OJO: ESTO AFECTA AL CONSTRUCTOR DE PEDIDO
public:
    // Al haberse declarado Articulo como clase abstracta, el constructor ahora no crea un objeto como tal, sino que inicializa los atributos de la clase abstracta en cuestión.
    Articulo(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro); // Constructor general
    const Cadena& referencia() const; // Método observador del código de referencia
    const Cadena& titulo() const; // Método observador del título
    const Fecha& f_publi()  const; // Método observador de la fecha de publicación
    double precio() const; // Método observador (lectura) del precio
    double& precio(); // Método de lectura / escritura del precio (el compilador determinará si se devuelve como rvalue o lvalue)
    //size_t stock() const; // SE TRASPASA A LA CLASE DE ARTÍCULOS ALMACENABLES
    //size_t& stock(); // SE TRASPASA A LA CLASE DE ARTÍCULOS ALMACENABLES
    const Autores& autores() const;

    class Autores_vacios{}; // Clase de excepción vacía (No se especifica nada)

    //friend ostream& operator <<(ostream& os, const Articulo& articuloParametro); // Operador de inserción de flujo de Articulo
    // Lo declaramos como friend para que tenga acceso a la dirección de memoria del set de autores
};

ostream& operator <<(ostream& os, const Articulo& articuloParametro); // Operador de inserción de flujo de Articulo

// ¿Cómo construir objetos derivados?
// Un objeto derivado, a parte de instanciar sus propios atributos, tiene que inicializar sus propios atributos.
// Para ello, la clase derivada deberá recibir todos los parámetros necesarios para las clases superiores
// y tenemos que seguir un orden específico de inicialización.
// Primero se han de inicializar los atributos de las clases raíces de dicha clase derivada.
// Como todas las clases derivadas tienen que hacer este procedimiento, la última clase derivada en la jerarquía
// podrá llamar al constructor de la clase raíz anterior. Ejemplo:

// CLASE ARTICULOALMACENABLE
// Esta clase será también abstracta, de forma que tampoco se podrán crear objetos de ella.
class ArticuloAlmacenable: public Articulo{
    size_t stockDisponible; // Almacena el stock disponible del artículo
public:
    // El constructor de esta clase añade un parámetro más a los de Articulo: el stock, que por defecto valdrá 0.
    ArticuloAlmacenable(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro, size_t cantidad=0);

    // Las existencias se podrán consultar y modificar a través de los métodos sobrecargados stock, que pasan de la clase Articulo a esta.
    size_t stock() const; // Método observador (lectura) del stock
    size_t& stock(); // Método de lectura / escritura del stock (el compilador determinará si se devuelve como rvalue o lvalue)
};

// CLASE LIBRO
// Esta es una clase almacenable, o sea, que posee un stock, y además hay que guardar el número de páginas
class Libro: public ArticuloAlmacenable{
    const size_t numeroPaginas; // El número de no se podrá cambiar tras crear el objeto.
public:
    // El constructor recibe los mismos parámetros que ArticuloAlmacenable más el número de páginas, que es el penúltimo, quedando de nuevo el stock en último lugar, para poder ponerle el valor predeterminado 0.
    Libro(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro, size_t numeroPaginasParametro, size_t cantidad=0);

    // El número de páginas se podrá consultar a través del observador n_pag.
    size_t n_pag() const;

    // Definición del método virtual puro que se hereda de la clase abstracta Articulo
    void impresion_especifica(std::ostream& os) const;
};

// CLASE CEDERRON
// Esta clase también es almacenable (tiene stock), y además hay que guardar el tamaño en MB (observador tam).
// Clase derivada de ArticuloAlmacenable, que engloba los objetos Articulo que se pueden almacenar y son de tipo Cederron.
class Cederron: public ArticuloAlmacenable{
    const size_t MB; // Guarda el tamaño del Cederron. No se puede cambiar una vez se ha creado el objeto.
public:
    // El Constructor es como el de Libro pero cambiando el número de páginas por el tamaño en MB.
    Cederron(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro, size_t MBParametro, size_t cantidad=0);

    // Observador del tamaño
    size_t tam() const;

    // Definición del método virtual puro que se hereda de la clase abstracta Articulo
    void impresion_especifica(std::ostream& os) const;
};

// CLASE LIBRO DIGITAL
// Clase derivada de Articulo, que engloba los objetos Articulo digitales
class LibroDigital: public Articulo{
    const Fecha fechaExpiracion; // Fecha de expiración en la cual los ebooks dejan de venderse.
public:
    // El constructor recibe los mismos parámetros que el de Articulo más la fecha de expiración.
    LibroDigital(const Autores& autoresParametro, const Cadena& referenciaParametro, const Cadena& tituloParametro, const Fecha& fechaParametro, double precioParametro, const Fecha& fechaExpiracionParametro);

    // Observador de la fecha de expiración
    const Fecha& f_expir() const;

    // Definición del método virtual puro que se hereda de la clase abstracta Articulo
    void impresion_especifica(std::ostream& os) const;
};

#endif //ARTICULO_HPP
