#include "pedido-articulo.hpp"

// Constructor general de LineaPedido
LineaPedido::LineaPedido(double precio, int cantidad):precioVenta(precio), cantidadVendida(cantidad) {}

// Método observador del precio de la venta
double LineaPedido::precio_venta() const {
    return precioVenta;
}

// Método observador de la cantidad vendida
size_t LineaPedido::cantidad() const {
    return cantidadVendida;
}

// Operador de inserción de flujo de LineaPedido
std::ostream& operator<<(std::ostream& os, const LineaPedido& pedido){
    // Se imprimirá el precio de venta con dos decimales, seguido de un espacio, el símbolo de moneda C, un tabulador y la cantidad.
    os<<fixed<<setprecision(2)<<pedido.precio_venta()<<" €\t"<<pedido.cantidad();
    return os;
}

// OrdenaPedidos es una clase de objetos función para ordenar los pedidos ascendentemente por número.
bool OrdenaPedidos::operator()(const Pedido *pedido1, const Pedido *pedido2) const {
    return pedido1->numero()<pedido2->numero();
}

// OrdenaArticulos es otra clase de objetos función para ordenar los artículos ascendentemente por referencia.
bool OrdenaArticulos::operator()(const Articulo* articulo1, const Articulo* articulo2) const {
    return articulo1->referencia() < articulo2->referencia();
}

// Para la creación de enlaces bidireccionales entre pedidos y artículos la clase proporcionará el método pedir,
// que tendrá cuatro parámetros en el orden: pedido, articulo, precio, cantidad (por omisión, 1).
// Este método se sobrecargará invirtiendo los dos primeros parámetros. No devolverá nada.
void Pedido_Articulo::pedir(Pedido &order, Articulo &item, double precio, size_t cantidad) {
    pedidoArticulo[&order].insert(make_pair(&item,LineaPedido(precio,cantidad)));
    articuloPedido[&item].insert(make_pair(&order,LineaPedido(precio,cantidad)));
}

// Método pedir() con parámetros invertidos
void Pedido_Articulo::pedir(Articulo &art, Pedido &ped, double precio, size_t cantidad) {
    pedir(ped,art,precio,cantidad);
}

// El método detalle devolverá la colección de artículos de un pedido (que se le pasa, por referencia) junto a su precio de venta y cantidad comprada;
// o sea, una referencia constante a ItemsPedido.
const Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& order){
    return pedidoArticulo.find(&order)->second;
}

// Para obtener los enlaces en el sentido contrario se proporcionará un método ventas, que devolverá todos los pedidos de un artículo (que se le pasa, por referencia)
// con precio de venta y cantidad; o sea, un Pedidos.
Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& a){
    if(articuloPedido.find(&a)!=articuloPedido.end()){
        return articuloPedido.find(&a)->second;
    }else{
        Pedido_Articulo::Pedidos vacio;
        return vacio;
    }
}

// Un método llamado mostrarDetallePedidos imprimirá en el flujo de salida proporcionado el detalle de todos los pedidos realizados hasta la fecha, así como el importe de todas las ventas.
ostream &Pedido_Articulo::mostrarDetallePedidos(ostream &os) {
    double precio = 0.0;
    for(auto& iter : pedidoArticulo){
        os << "Pedido núm. " << iter.first->numero();
        os << "\tCliente: " << iter.first->tarjeta()->titular()->nombre() << " \n";
        os << "Fecha: " << iter.first->fecha() << iter.second;
        precio += iter.first->total();
    }
    os << "TOTAL VENTAS: " << fixed<< setprecision(2) << precio << " €" << endl;
    return os;
}

// El método mostrarVentasArticulos visualizará en el flujo de salida proporcionado todas las ventas agrupadas por artículos.
ostream &Pedido_Articulo::mostrarVentasArticulos(ostream &os) {
    for(auto& iter: articuloPedido){
        os << "Ventas de " << "[" << iter.first->referencia() << "]";
        os << "\"" << iter.first->titulo() << "\" \n" << iter.second << endl;
    }
    return os;
}

// Métodos de inserción de flujo
// El primero, además de los detalles del pedido, mostrará el importe total y el número de ejemplares del pedido.
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::ItemsPedido& items){
    double precio = 0;
    Pedido_Articulo::ItemsPedido::const_iterator i;
    os << endl << "====================================================\n" << endl;
    os << " PVP \t Cant. \t Articulo \n";
    os << "====================================================\n" << endl;
    for(i = items.begin(); i != items.end(); ++i){
        os << " " << i->second.precio_venta() << "€\t";
        os << i->second.cantidad() << "\t";
        os << "[" << i->first->referencia() << "] ";
        os << "\"" << i->first->titulo() << "\" " << endl;
        precio = precio + i->second.precio_venta() * i->second.cantidad();
    }
    os << "===================================================\n" << endl;
    os << fixed;
    os << setprecision(2) << precio << " €" << endl;
    return os;
}

// El segundo mostrará precio, cantidad y fecha de cada venta y el importe total de las ventas del artículo.
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& orders){
    double precio = 0;
    unsigned total = 0;
    Pedido_Articulo::Pedidos::const_iterator i;
    os << "====================================================\n" << endl;
    os << " PVP \t Cant. \t Fecha venta \n";
    os << "====================================================\n" << endl;
    for(auto iter : orders){
        os << " " << iter.second.precio_venta() << "€\t";
        os << iter.second.cantidad() << "\t";
        os << iter.first->fecha() << endl;
        precio += (iter.second.precio_venta() * iter.second.cantidad());
        total += iter.second.cantidad();
    }
    os << "====================================================\n" << endl;
    os << fixed;
    os << setprecision(2) << precio << " €\t" << total <<endl ;
    return os;
}


