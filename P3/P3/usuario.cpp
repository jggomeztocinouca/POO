#include "usuario.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <random>
#include <iomanip>
#include <set>

using namespace std;

// ****************************** Clase Clave ******************************
// Constructor de conversión
Clave::Clave(const char *c){
    if(strlen(c)<5){ // Si la cadena de caracteres tiene longitud menor a 5, no es válida.
        throw Clave::Incorrecta(Clave::CORTA); // Lanzamos excepción
    }
    random_device r; // Objetos de la clase random_device, que nos permitirá hacer el cifrado.
    uniform_int_distribution<size_t> dist(0, 63); // Creamos un contenedor de distribución uniforme de enteros de 0 a 63
    Cadena letras{"./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"}; // Creamos una cadena con todos los caracteres que pueden cifrar nuestra contraseña
    char sal[2]{letras[dist(r)],letras[dist(r)]}; // Seleccionamos dos caracteres aleatorios de la distribución uniforme dist, haciendo uso del objeto r (de random_device) que nos devolverá aleatoriamente dos números.
    if(crypt(c, sal) == nullptr){ // crypt devolverá la contraseña cifrada haciendo uso de los dos caracteres que hemos seleccionado aleatoriamente. Si devuelve nullptr, ha habido un error de cifrado.
        throw Clave::Incorrecta(Clave::ERROR_CRYPT); //ERROR ERROR_CRYPT
    }
    password = crypt(c, sal); // Si no ha habido error de cifrado, guardamos la contraseña cifrada.
}

// Método observador que devuelve la contraseña cifrada (que es un objeto de la clase Cadena)
const Cadena &Clave::clave() const { return password; }

// Método que devuelve true o false si la contraseña es la misma que la almacenada.
bool Clave::verifica(const char *claveParametro) const {
    // Como la contraseña almacenada está cifrada, deberemos cifrar la contraseña que recibimos como parámetro
    Cadena aux{crypt(claveParametro,password.c_str())}; // En este caso ciframos la contraseña en una Cadena auxiliar con los mismos caracteres con las que se cifró la contraseña original.
    return (aux==password); // Devuelve si las dos Cadenas son iguales
}

// *** Clase de excepción Incorrecta ***
// Constructor de conversión
Clave::Incorrecta::Incorrecta(const Clave::Razon r):motivo(r) {}

// Método observador
Clave::Razon Clave::Incorrecta::razon() const { return motivo; } // Devuelve el motivo de la incorrección


// ****************************** Clase Usuario ******************************

// Inicializamos el atributo estático del contenedor de identificadores de Usuarios
std::unordered_set<Cadena> Usuario::users;

// Constructor de conversión de Usuario
Usuario::Usuario(const Cadena &identificador_, const Cadena &nombre_, const Cadena &apellidos_, const Cadena &direccion_, const Clave &password_): identifier(identificador_), name(nombre_), surname(apellidos_), address(direccion_), password(password_){
    // Si el identificador de usuario ya se encuentra en el contenedor que almacena los identificadores...
    if(!users.insert(identificador_).second) {
        throw Id_duplicado(identificador_); // Lanzamos la excepción...
    }
}

// Constructor de conversión de la clase de excepción de Usuario::Id_duplicado
Usuario::Id_duplicado::Id_duplicado(const Cadena &id):id_repetido(id) {}

// Método observador de la clase de excepción Usuario::Id_duplicado
const Cadena &Usuario::Id_duplicado::idd() const {
    return id_repetido;
}

// Métodos observadores de la clase Usuario
const Cadena &Usuario::id() const {
    return identifier;
}

const Cadena &Usuario::nombre() const {
    return name;
}

const Cadena &Usuario::apellidos() const {
    return surname;
}

const Cadena &Usuario::direccion() const {
    return address;
}

// Método de asociación de tarjetas
void Usuario::es_titular_de(const Tarjeta &card) {
    // Comprobamos si realmente la el titular de la tarjeta que queremos asociar pertenece al usuario que llama al método
    if (card.titular()==this){
        cards[card.numero()] = const_cast<Tarjeta*>(&card); // Lo insertamos en el contendor
    }
}

// Método de desasociación de tarjetas
void Usuario::no_es_titular_de(Tarjeta &card) {
    // Comprobamos si realmente la el titular de la tarjeta que queremos desasociar pertenece al usuario que llama al método
    if (card.titular()==this){
        card.anula_titular();
        cards.erase(card.numero()); // Lo eliminamos del contenedor
    }
}

// Método observador del contenedor de artículos
const std::unordered_map<Articulo *, size_t> &Usuario::compra() const {
    return items;
}

// Método observador del contenedor de tarjetas
const Usuario::Tarjetas &Usuario::tarjetas() const {
    return cards;
}

// Destructor de los objetos Usuarios
Usuario::~Usuario() {
    // Recorremos el contenedor que posee el Usuario con sus tarjetas y anulamos todas
    for(auto it = cards.begin(); it!=cards.end(); it++){
        it->second->anula_titular();
    }
    users.erase(identifier); // Eliminamos el usuario del contenedor de usuarios
}

// Método para añadir o eliminar artículos de la compra
void Usuario::compra(Articulo& a, size_t cantidad) {
    if(cantidad==0){ // Si la cantidad es 0...
        if(items.find(&a)!=items.end()){
            items.erase(const_cast<Articulo*>(&a)); // Eliminamos el artículo
        }
    }else{
        // Si no, aumentamos o añadimos el articulo al carrito
        items[const_cast<Articulo*>(&a)] = cantidad;
    }
}

// Método observador del número de artículos en el carrito
size_t Usuario::n_articulos() const {
    return items.size();
}

// Operador de inserción de flujo de usuario
ostream& operator << (ostream& os, const Usuario& user){
    setlocale(LC_ALL, "");
    os<<user.id()<<" ["<<user.password.clave()<<"] "<<user.nombre()<<" "<<user.apellidos()<<endl;
    os<<user.direccion()<<endl;
    os<<"Tarjetas:\n";
    for (auto& it : user.cards) { // Hacemos un bucle para imprimir todas las tarjetas que tiene
        os << *it.second << endl;
    }
    return os;
}

// Método de inserción de flujo del carrito de usuario
void mostrar_carro(ostream& os, const Usuario& user){
    os<<"Carrito de compra de "<<user.id()<<" [Artículos: "<<user.n_articulos()<<"]"<< endl<< " Cant.\tArtículo" <<endl;
    os <<"===========================================================" <<endl;
    for(auto i : user.compra()){
        os << " " << i.second << "\t" << "[" << i.first->referencia() << "] \"" << i.first->titulo() << "\", ";
        os << i.first->f_publi().anno() << ". " << setprecision(2) << std::fixed << i.first->precio() << " €" << endl;
    }
}

