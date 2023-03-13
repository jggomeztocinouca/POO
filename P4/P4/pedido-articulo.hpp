#ifndef PEDIDO_ARTICULO_HPP
#define PEDIDO_ARTICULO_HPP

#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include "pedido.hpp"

using namespace std;

// Se utilizará para almacenar los atributos de enlace de la asociación bidireccional varios a-varios entre Pedido y Articulo.
// Por tanto, constará de dos atributos: precio de venta y cantidad vendida.
// Este precio puede ser diferente del precio actual, ya que los precios pueden cambiar a lo largo del tiempo, de ahí la necesidad de almacenarlo por separado.
class LineaPedido{
public:
    // Una LineaPedido se construye con dos parámetros que corresponden a los valores de sus atributos.
    // La cantidad, que es el segundo, tomará por omisión el valor 1.
    // No se permitirá la conversión implícita de un precio a LineaPedido.
    explicit LineaPedido(double, int = 1);

    // Tendrá dos métodos observadores llamados precio_venta y cantidad, que devolverán sus atributos.
    double precio_venta() const;
    size_t cantidad() const;
private:
    double precioVenta;
    size_t cantidadVendida;
};

// Se definirá el operador de inserción << para mostrar o imprimir un objeto LineaPedido en un flujo de salida.
std::ostream& operator<<(std::ostream& os, const LineaPedido& pedido);

class Pedido; // Declaración adelantada
class Articulo; // Declaración adelantada

// OrdenaPedidos es una clase de objetos función para ordenar los pedidos ascendentemente por número.
class OrdenaPedidos: public std::binary_function<Pedido*,Pedido*,bool>{
public:
    bool operator()(const Pedido* pedido1, const Pedido* pedido2) const;
};

// OrdenaArticulos es otra clase de objetos función para ordenar los artículos ascendentemente por referencia.
class OrdenaArticulos: public std::binary_function<Articulo*,Articulo*,bool>{
public:
    bool operator()(const Articulo* articulo1, const Articulo* articulo2) const;
};

// Representa la asociación bidireccional varios-a-varios con atributos de enlace entre Pedido y Articulo.
class Pedido_Articulo{
public:
    // El que representa la asociación desde Pedido a Articulo será del tipo
    // std::map<Pedido*, ItemsPedido, OrdenaPedidos>
    // donde ItemsPedido será un tipo público de la clase Pedido_Articulo definido como un diccionario
    // std::map<Articulo*, LineaPedido, OrdenaArticulos>.
    typedef std::map<Articulo*, LineaPedido, OrdenaArticulos> ItemsPedido;
    typedef std::map<Pedido*, ItemsPedido, OrdenaPedidos> pedArt;

    // La asociación en el sentido inverso (desde Articulo a Pedido) se representará por un
    // diccionario del tipo std::map<Articulo*, Pedidos, OrdenaArticulos>, donde
    // Pedidos será un tipo público de la clase Pedido_Articulo definido como
    // std::map<Pedido*, LineaPedido, OrdenaPedidos>.
    typedef std::map<Pedido*, LineaPedido, OrdenaPedidos> Pedidos;
    typedef std::map<Articulo*, Pedidos, OrdenaArticulos> artPed;

    // Para la creación de enlaces bidireccionales entre pedidos y artículos la clase proporcionará el método pedir,
    // que tendrá cuatro parámetros en el orden: pedido, articulo, precio, cantidad (por omisión, 1).
    // Este método se sobrecargará invirtiendo los dos primeros parámetros. No devolverá nada.
    void pedir(Pedido& ped, Articulo& art, double precio, size_t cantidad=1);
    void pedir(Articulo& art, Pedido& ped, double precio, size_t cantidad=1);

    // El método detalle devolverá la colección de artículos de un pedido (que se le pasa, por referencia) junto a su precio de venta y cantidad comprada;
    // o sea, una referencia constante a ItemsPedido.
    const ItemsPedido& detalle(Pedido& order);

    // Para obtener los enlaces en el sentido contrario se proporcionará un método ventas, que devolverá todos los pedidos de un artículo (que se le pasa, por referencia)
    // con precio de venta y cantidad; o sea, un Pedidos.
    Pedidos ventas(Articulo& a);

    // Un método llamado mostrarDetallePedidos imprimirá en el flujo de salida proporcionado el detalle de todos los pedidos realizados hasta la fecha, así como el importe de todas las ventas.
    ostream& mostrarDetallePedidos(ostream& os);

    // El método mostrarVentasArticulos visualizará en el flujo de salida proporcionado todas las ventas agrupadas por artículos.
    ostream& mostrarVentasArticulos(ostream& os);
private:
    pedArt pedidoArticulo; //(std::map<Pedido*, ItemsPedido, OrdenaPedidos>)
    artPed articuloPedido; //(std::map<Articulo*, Pedidos, OrdenaArticulos>)
};

// Se sobrecargará el operador de inserción en flujo de salida << para los tipos Pedido_Articulo::ItemsPedido y Pedido_Articulo::Pedidos.

// El primero, además de los detalles del pedido, mostrará el importe total y el número de ejemplares del pedido.
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::ItemsPedido& items);

// El segundo mostrará precio, cantidad y fecha de cada venta y el importe total de las ventas del artículo.
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& orders);

#endif //PEDIDO_ARTICULO_HPP
