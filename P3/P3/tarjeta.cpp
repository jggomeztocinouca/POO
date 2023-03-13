#include "tarjeta.hpp"
#include <iomanip>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <functional>

using namespace std;

bool luhn(const Cadena& numero); // Declaración adelantada de luhn (se incorpora de luhn.cpp)

// *** Clase Numero ***
// Constructor de conversión
// El constructor tendrá que quitarle los blancos y comprobar que es un número válido.
// Si no lo fuera lanzará la excepción Numero::Incorrecto.

// Validez del número de la tarjeta
// Un número de tarjeta de pago válido debe contener solo dígitos,
// y esta cadena de solo dígitos debe tener una longitud comprendida entre 13 y 19, incluidos.
// Así que primero, en el constructor de Numero, hay que quitar los espacios en blanco y ver si hay algún carácter no dígito.
// Después hay que comprobar la longitud, y por último la validez, ya que el último dígito de la tarjeta es de control.
// Para esta tarea se debe usar el algoritmo de Luhn.
//Se proporciona el fichero luhn.cpp con una implementación en C++.
// Si el número sin espacios es correcto, se vuelve a almacenar.



Numero::Numero(const Cadena &numeroParametro): numero("") { // Inicializamos la Cadena numero a una vacía
    // Creamos una cadena auxiliar, donde almacenaremos la cadena pasada por parámetro.
    // Como queremos modificar esta cadena pero no la que se nos pasa como parámetro (no sabemos si se va a volver usar en otro sitio), se crea esta cadena.
    Cadena cadenaAuxiliar(numeroParametro);

    // --- ELIMINAR ESPACIOS EN BLANCO ---

    // La función remove_if elimina todos los elementos de un contenedor (dado sus iteradores principio y fin) que cumplan la condición del predicado.
    // En realidad, pone todos los elementos a 0 y los agrupa al final del contenedor, dejando el contenedor resultante al principio.
    // remove_if devuelve un iterador a la siguiente posición del último elemento no modificado.
    // Sintaxis: remove_if(iterador_principio, iterador_principio, predicado)

    // ¿Qué es un predicado? --> Es toda aquella función que devuelve un valor booleano.
    // Tipos de predicado que podemos usar en estas situaciones:
    // 1.- Funciones booleanas
    // inline mayorque7(int x){ return x>7; }

    // 2.- Objetos función --> Sirven para generalizar funciones predicado y adaptarse en tiempo de ejecución. Tienen sobrecargado el operador () con devolución booleana.
    //class mayorque{
    //    int y;
    //public:
    //    mayorque(int y):y(x){};
    //    bool operator()(int z){ return z>y; }
    //}

    // 3.- Funciones anónimas (λ) --> Son funciones que no están declaradas pero se definen en el propio predicado
    // Los corchetes indican que es una función anónima --> [] (int x) { return x>7; }

    // Se llama a la función remove_if con los iteradores principio y fin de la cadena auxiliar creada, además del predicado mediante una función anónima que devuelve si el carácter leído es un espacio.
    auto it1 = remove_if(cadenaAuxiliar.begin(),cadenaAuxiliar.end(),[](char x){return isspace(x);});
    // Se almacena el iterador a la siguiente posición del último elemento no modificado.

    // Si el iterador devuelto por remove_if es igual al iterador de final de contenedor, significa que no se ha borrado ningun elemento.
    if (it1!=cadenaAuxiliar.end()){
        // Marcamos el final de cadena en el primer elemento borrado (apuntado por el iterador devuelto por remove_if)
        *it1 = '\0';
        // Esto nos servirá para crear una nueva cadena con los caracteres válidos resultantes, ya que el constructor de conversión de Cadena (a partir de un puntero) usa strcpy,
        // y esta función copia caracteres hasta que encuentre el carácter terminador \0.
        cadenaAuxiliar = Cadena(cadenaAuxiliar.c_str());
    }

    // --- COMPROBAR DÍGITOS Y LONGITUD ---

    // Hacemos uso del adaptador de negación unaria definido en la cabecera <functional>
    unary_negate<EsDigito> not_EsDigito((EsDigito())); // Hacemos una función negativa del objeto función (en particular de la clase de objetos función EsDigito)

    // find_if es similar a remove_if, con la diferencia de que esta función devuelve el primer elemento que cumple con la condición del predicado.
    Cadena::const_iterator it2 = find_if(cadenaAuxiliar.begin(), cadenaAuxiliar.end(), not_EsDigito);

    // Si el iterador devuelto por find_if es distinto al iterador end del contenedor, significa que ha habido una coincidencia con el predicado.
    if(it2!=cadenaAuxiliar.end()){
        throw Incorrecto(Razon::DIGITOS); // Si existe un carácter no dígito, se lanza la excepción
    }
    if (cadenaAuxiliar.length()<13||cadenaAuxiliar.length()>19){ // Si la Cadena resultante es de tamaño menor que 13 o mayor que 19...
        throw Numero::Incorrecto(Razon::LONGITUD); // Lanzamos la excepción
    }
    if(!luhn(cadenaAuxiliar)){ // Si luhn detecta que el último número de la Cadena no es válido...
        throw Numero::Incorrecto(Razon::NO_VALIDO); // Lanza una excepción
    }

    // Si pasa todas las comprobaciones, la cadena resultante es correcta y se asigna al atributo del objeto.
    numero = cadenaAuxiliar;
}

// Operador de conversión a cadena de caracteres de bajo nivel
Numero::operator const char* () const{
    return numero.c_str(); // Devuelve en forma de cadena de caracteres de bajo nivel, la Cadena que almacena el número.
}

// Operador externo de comparación (menor-que)
bool operator <(const Numero& n1, const Numero& n2){ // Recibe dos referencias constantes a objetos de tipo Numero
    return(strcmp(n1,n2)<0); // Haciendo uso del operador de conversión definido para la clase Numero y la función strcmp definida en la cabecera <cstring>
    // comparamos los dos números.
    // strcmp < 0 --> numero1 < numero2
    // strcmp == 0 --> numero1 == numero2
    // strcmp > 0 --> numero1 > numero2
}

// *** Clase excepción Incorrecto ***
// Constructor de conversión a partir de un enumerado de tipo Numero::Razon
Numero::Incorrecto::Incorrecto(const Numero::Razon r):motivo(r) {}

// Método observador del motivo
Numero::Razon Numero::Incorrecto::razon() const {
    return motivo; // Devuelve (por copia) el motivo de la incorrección.
}


// *** Clase Tarjeta ***
set<Numero> Tarjeta::num;
Tarjeta::Tarjeta(const Numero &numero, Usuario &usuario, const Fecha &fecha): numeroTarjeta(numero),
                                                                  titularTarjeta(&usuario),
                                                                  caducidadTarjeta(fecha),
                                                                  tarjetaActiva(true){ // Una Tarjeta se crea siempre activada.

    //  Si la fecha es anterior a la actual, el constructor lanzará la excepción Tarjeta::Caducada.
    if(fecha<Fecha()){ // Si la fecha es menor a la del sistema, se nos da una tarjeta caducada
        throw Tarjeta::Caducada(fecha); // Lanzamos una excepción
    }

    // No puede haber 2 tarjetas con el mismo número. En el caso de que se encuentre duplicidad de tarjetas, se lanzará la
    // excepción Tarjeta::Num_duplicado
    if(!num.insert(numero).second){ // Si resulta falso, el Numero no se ha podido introducir en el contendor porque está repetido.
        throw Num_duplicado(numero); // Lanzamos la excepción
    }

    // El constructor deberá asociar la tarjeta que se está creando con su Usuario correspondiente, llamando a Usuario::es_titular_de sobre su titular.
    usuario.es_titular_de(*this); // Asignamos al usuario titular (pasado por parámetro) la tarjeta creada (*this).
}


// Métodos observadores
// Habrá un método observador para cada atributo, que lo devuelva. Se llamarán numero, titular, caducidad y activa.
const Numero &Tarjeta::numero() const {
    return numeroTarjeta;
}

const Usuario* Tarjeta::titular() const {
    return titularTarjeta;
}

const Fecha &Tarjeta::caducidad() const {
    return caducidadTarjeta;
}

bool Tarjeta::activa() const {
    return tarjetaActiva;
}


// El método observador tipo devolverá el Tipo de la Tarjeta.
// Este está determinado por los primeros dígitos del Numero.
// Para simplificar, aunque no sea muy correcto, supondremos que el tipo es:
// AmericanExpress --> Si el Numero empieza por 34 o 37,
// JCB --> Si el Numero empieza por 3, salvo 34 o 37,
// VISA --> Si el Numero empieza por 4,
// Mastercard --> Si el Numero empieza por 5,
// Maestro --> Si el Numero empieza por 6, y
// Otro --> En cualquier otro caso.
Tarjeta::Tipo Tarjeta::tipo() const {
    // Valores ASCII de los números 0-9 --> 48-57

    // Si los dos primeros son 34 o 37
    if (numeroTarjeta[0] == '3' && (numeroTarjeta[1] == '4' || numeroTarjeta[1] == '7')){
        return Tipo::AmericanExpress; // Es una tarjeta American Express
    }else {
        // Si el primer numero es...
        switch (numeroTarjeta[0]) {
            // Si empieza por cualquier otro número
            default:
                return Tipo::Otro; // Es una tarjeta Otro
                break;

                // Si el Numero empieza por 3
            case '3':
                return Tipo::JCB; // Es una tarjeta JCB
                break;

                // Si el Numero empieza por 4
            case '4':
                return Tipo::VISA; // Es una tarjeta VISA
                break;

                // Si el Numero empieza por 5
            case '5':
                return Tipo::Mastercard; // Es una tarjeta Mastercard
                break;

                // Si el Numero empieza por 6
            case '6':
                return Tipo::Maestro; // Es una tarjeta Maestro
                break;
        }
    }
}

// Habrá también un método modificador para activar o desactivar una Tarjeta.
// Se llamará activa (siendo por tanto una sobrecarga del método observador anterior) y podrá recibir un parámetro booleano,
// cuyo valor predeterminado será true, y devolverá un booleano que será el nuevo valor después de la activación o desactivación.
bool& Tarjeta::activa(bool estado) {
    tarjetaActiva=estado; // Ponemos el estado de la tarjeta al pasado por parámetro
    return tarjetaActiva; // Devolvemos el nuevo valor
}


// Cuando un Usuario se destruya, sus tarjetas asociadas seguirán «vivas», aunque obviamente ya no pertenezcan a nadie.
// Por lo tanto, la clase Tarjeta tendrá un método modificador llamado anula_titular
// que dará al puntero que representa al titular el valor nulo y desactivará la Tarjeta poniendo a false el atributo correspondiente.
void Tarjeta::anula_titular() {
    activa(false); // Se llama a la función activa con el valor falso.
    // Como titularTarjeta es un puntero constante a un Usuario, no podemos cambiarlo simplemente.
    // Deberemos hacer una conversión (cast) a el tipo a cambiar.
    const_cast<Usuario*&>(titularTarjeta) = nullptr; // Se cambia el valor del puntero
}

// Al destruirse un objeto de tipo Tarjeta deberá desasociarse de su Usuario, llamando a
// Usuario::no_es_titular_de sobre su titular, en caso de que este no haya sido destruido
// previamente; de lo contrario, la Tarjeta habrá sido desligada de su Usuario al ser destruido este.
Tarjeta::~Tarjeta() {
    if(titularTarjeta){ // Si el titular no ha sido anulado previamente.
        const_cast<Usuario*&>(titularTarjeta)->no_es_titular_de(*this); // Desasociamos al titular de esta tarjeta
        const_cast<Usuario*&>(titularTarjeta)=nullptr; // Ponemos al atributo puntero de Tarjeta que almacena el Usuario a null.
    }
    num.erase(numero()); // Eliminamos la tarjeta eliminada del contenedor
}

// Se sobrecargará el operador de inserción en flujo (<<) para mostrar o imprimir una
//Tarjeta en un flujo de salida. El formato será:
// tipo
// número
// titular facial
// Caduca: MM/AA
// donde MM es el mes de la fecha de caducidad, expresado con dos dígitos y AA son los
// dos últimos dígitos del año; por ejemplo: 05/09 sería mayo de 2009.
// El titular facial es el nombre y apellidos del propietario de la tarjeta, en mayúsculas.
// Si quiere, por estética, puede dibujar líneas rodeando la información impresa de la
// tarjeta, simulando, aun pobremente, su aspecto. Esto es opcional.
// Para imprimir el nombre del tipo de la tarjeta (VISA, American Express. . . ), deberá
// sobrecargar también el operador de inserción para Tarjeta::Tipo.
std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& tipo){
    switch(tipo){
        case Tarjeta::VISA: os << "VISA"; break;
        case Tarjeta::Mastercard: os << "Mastercard"; break;
        case Tarjeta::Maestro: os << "Maestro"; break;
        case Tarjeta::JCB: os << "JCB"; break;
        case Tarjeta::AmericanExpress: os << "AmericanExpress"; break;
        case Tarjeta::Otro: os << "Tipo indeterminado"; break;
        default: os << "Error";
    }
    return os;
}
std::ostream& operator <<(std::ostream& salida, const Tarjeta& t)
{
    // Como no podemos hacer uso del operador de inserción de Usuario, deberemos separar la información que deseamos imprimir. En este caso nombre y apellidos del titular.
    // Nombre facial significa que deberemos imprimir el nombre del titular en mayúsculas
    Cadena nombreTitular(t.titular()->nombre()); // Creamos una Cadena con el nombre del titular
    Cadena apellidosTitular(t.titular()->apellidos()); // Creamos una Cadena con el apellido del titular
    auto it = nombreTitular.begin(); // Creamos un iterador para recorrer la Cadena...
    size_t i=0; // y un acumulador para acceder al operador índice (no podemos acceder con el iterador porque este guarda una dirección de memoria.)
    while(it!=nombreTitular.end()){ // Mientras no recorra la Cadena entera, se procede con el bucle.
        if(islower(*it)){ // islower devuelve si el carácter es minúsculas. Si es minúsculas...
            nombreTitular[i]= toupper(*it); // Rescribimos el carácter con toupper, que nos devolverá el carácter pasado a la función pero en mayúsculas.
        }
        it++; // Incrementamos el iterador
        i++; // Incrementamos el acumulador
    }
    it = apellidosTitular.begin(); // Asignamos al iterador (que al recorrer otra Cadena, es el mismo tipo y no hace falta definir otra variable)
    i=0; // Reestablecemos el acumulador a 0
    // Y repetimos el mismo proceso
    while(it!=apellidosTitular.end()){
        if(islower(*it)){
            apellidosTitular[i]= toupper(*it);
        }
        it++;
        i++;
    }
    // Una vez reajustados todos los datos al formato pedido, y los métodos necesarios para imprimirlos, introducimos en el objeto de flujo.
    salida << t.tipo() << endl << t.numero() << endl << nombreTitular << " " << apellidosTitular << endl << "Caduca: " << std::setfill ('0') << std::setw (2) << t.caducidad().mes() << "/" << std::setw (2) << (t.caducidad().anno() % 100) << std::endl;
    return salida;
}

// Constructor de conversión de la clase de excepción Tarjeta::Caducada
Tarjeta::Caducada::Caducada(const Fecha &f):caducidad(f) {}


// Método observador de la clase de excepción de Tarjeta::Caducada
const Fecha &Tarjeta::Caducada::cuando() const {
    return caducidad;
}

// Constructor de conversión de la clase de excepción Tarjeta::Num_duplicado
Tarjeta::Num_duplicado::Num_duplicado(const Numero &n): duplicado(n) {}

// Método observador de la clase de excepción de Tarjeta::Num_duplicado
const Numero &Tarjeta::Num_duplicado::que() const {
    return duplicado;
}

// Operador binario externo de comparación (menor-que) de tarjetas
bool operator < (const Tarjeta& t1, const Tarjeta& t2){
    return (t1.numero()<t2.numero());
}