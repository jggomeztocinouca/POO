#include "pedido.hpp"

// Los atributos estáticos se inicializarán en distintos "sitios" dependiendo del tipo de atributo que define
// Haremos una clasificación:
// static const enteros (1 bit) → bool, char, int
//	- Pueden inicializarse dentro de la propia clase
//	- Se ha de tener especial cuidado con los enteros de tipo puntero (punteros, vectores)
//static y derivados
//	- Deben inicializarse (obligatoriamente) fuera de la clase

// Como tenemos un atributo static derivado, se ha de inicializar fuera de la clase.
size_t Pedido::numeroPedidos = 0; // Se inicializa el atributo estático

// Constructor general
// El constructor realiza la compra de los artículos, para lo cual utiliza cinco parámetros:
// • La asociación entre usuarios y pedidos, representada por la clase Usuario_Pedido --> (usuarioPedido)
// • la asociación entre pedidos y artículos, representada por la clase Pedido_Articulo --> (pedidoArticulo)
// • el Usuario que realiza la compra, --> (comprador)
// • la Tarjeta con la que se realiza el pago, y --> (tarjetaUsada)
// • la Fecha del pedido, por omisión será la actual. --> (fechaCompra)

Pedido::Pedido(Usuario_Pedido &usuarioPedido, Pedido_Articulo &pedidoArticulo, Usuario &comprador, const Tarjeta &tarjetaUsada, const Fecha &fechaCompra):n_pedido(numeroPedidos+1),
                                                                                                                                                          tarjetaPago(&tarjetaUsada),
                                                                                                                                                          fechaPedido(fechaCompra),
                                                                                                                                                          importeCompra(0.) {

    // Si el carrito no tiene ningún artículo, para evitar crear un pedido vacío, el constructor terminará con la excepción Pedido::Vacio.
    if(comprador.compra().empty()){
        throw Pedido::Vacio(&comprador);
    }

    // Se comprobará también que el titular de la tarjeta es quien hace el pedido. De no ser así se lanzará la excepción Pedido::Impostor.
    if(tarjetaUsada.titular() != &comprador){
        throw Pedido::Impostor(&comprador);
    }

    // Además, el constructor lanzará la excepción Tarjeta::Caducada si la fecha de caducidad de la tarjeta de pago es anterior a la fecha del pedido,
    if (tarjetaUsada.caducidad()<fechaCompra){
        throw Tarjeta::Caducada(tarjetaUsada.caducidad());
    }

    // o Tarjeta::Desactivada si la tarjeta de pago no está activa.
    if (!tarjetaUsada.activa()){
        throw Tarjeta::Desactivada();
    }

    // En el caso de que la cantidad de alguno de los artículos del carrito supere las existencias
    // en almacén (stock), se vacía el carrito y se anula el pedido lanzando la excepción Pedido::SinStock.

    // Hacemos un bucle de rango --> for(variable acumulador : contenedor) --> recorre el contenedor entero
    // compra() devuelve el carrito (unordered_map<Articulo*, size_t>) asociado al usuario.
    for(auto it : comprador.compra()){
        if(it.first->stock()<it.second) { // Si el stock del artículo del carrito es menor que la cantidad deseada...
            // Vaciamos el carrito
            // No podemos hacer clear(), porque vaciaría el contenedor y para lanzar la excepción necesitamos acceder al artículo sin stock.
            for (auto it1:comprador.compra()){
                // El método sobrecargado compra, recibiendo un artículo y una cantidad, actualiza el carrito del usuario. Si la cantidad es 0, se elimina el artículo del carrito.
                comprador.compra(*it1.first,0); // Ponemos todas las cantidades de los artículos del carrito a 0
            }
            throw Pedido::SinStock(it.first); // Se lanza la excepción (Se ha borrado el artículo del contenedor, no de memoria)
        }
    }

    // Cuando se genera un pedido de los artículos que contiene el carrito, este quedará vacío y las existencias de los artículos vendidos actualizadas.
    // El nuevo pedido quedará asociado al usuario que lo realiza, a la tarjeta de pago y a los artículos incluidos en la compra.
    // Se llamará a los métodos apropiados de las clases de asociación para crear los enlaces
    // asocia() recibe como parámetros el pedido (*this) y el usuario que realiza la compra (usuarioPedido)
    usuarioPedido.asocia(*this, comprador); // Se introduce en la clase de asociación usuario_pedido, que representa la asociación bidireccional uno-a-varios entre Usuario y Pedido.
    for(auto it2:comprador.compra()){
        importeCompra += it2.first->precio() * it2.second; // Se acumula el total del importe (precio x cantidad)
        // pedir() es el método de asociación de la clase de asociación pedido_articulo, que Representa la asociación bidireccional varios-a-varios con atributos de enlace entre Pedido y Articulo.
        pedidoArticulo.pedir(*it2.first, *this, it2.first->precio(), it2.second); // Este método recibe un articulo ((*it).first), un pedido (*this), un precio((*it).first.precio()) y una cantidad ((*it).second)
        it2.first->stock()-=it2.second;
    }
    // Se vacía el carrito
    const_cast<Usuario::Articulos&>(comprador.compra()).clear();

    // Se aumenta el número de pedidos hechos
    numeroPedidos++;
}

// Métodos observadores

// Método observador del número de pedido
size_t Pedido::numero() const {
    return n_pedido;
}

// Método observador de la tarjeta usada para la compra
const Tarjeta *Pedido::tarjeta() const {
    return tarjetaPago;
}

// Método observador de la fecha del pedido
Fecha Pedido::fecha() const {
    return fechaPedido;
}

// Método observador del importe del pedido
double Pedido::total() const {
    return importeCompra;
}

// Método observador del número de pedidos hechos
unsigned Pedido::n_total_pedidos() {
    return numeroPedidos;
}

// Constructor de conversión de la clase de excepción Vacio
Pedido::Vacio::Vacio(Usuario *u):user(u) {}

// Método observador de la clase de excepción
const Usuario& Pedido::Vacio::usuario() const {
    return *user;
}

// Constructor de conversión de la clase de excepción Impostor
Pedido::Impostor::Impostor(Usuario *u):user(u) {}

// Método observador de la clase de excepción
const Usuario& Pedido::Impostor::usuario() const {
    return *user;
}

// Constructor de conversión de la clase de excepción SinStock
Pedido::SinStock::SinStock(Articulo *_articulo):item(_articulo) {}

// Método observador de la clase de excepción
const Articulo& Pedido::SinStock::articulo() const {
    return *item;
}

// Operador de inserción de flujo de Pedido
std::ostream& operator<<(std::ostream& os, const Pedido& order){
    os << left;
    os << "Núm. pedido:" << order.numero() << endl;
    os << setw(13) << "Fecha:" << order.fecha() << endl;
    os << setw(13) << "Pagado con:" << order.tarjeta()->tipo() << " n.o: " << order.tarjeta()->numero() << endl;
    os << setw(13) << "Importe:" << fixed << setprecision(2) << order.total() << " €" << endl;
    return os;
}