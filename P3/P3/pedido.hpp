#ifndef PEDIDO_HPP
#define PEDIDO_HPP

#include "usuario.hpp"
#include "tarjeta.hpp"
#include "fecha.hpp"
#include "articulo.hpp"
#include "pedido-articulo.hpp"
#include "usuario-pedido.hpp"
#include<iomanip>
#include<iostream>

class Pedido_Articulo;
class Tarjeta;
class Usuario_Pedido;

class Pedido{
public:
    // Constructor general
    Pedido(Usuario_Pedido& usuarioPedido, Pedido_Articulo& pedidoArticulo, Usuario& comprador, const Tarjeta& tarjetaUsada, const Fecha& fechaCompra = Fecha());

    // Métodos observadores
    size_t numero() const; // Devuelve el número de pedido
    const Tarjeta* tarjeta() const; // Devuelve la tarjeta usada para el pedido
    Fecha fecha() const; // Devuelve la fecha en la que se realizó el pedido
    double total() const; // Devuelve el importe del pedido
    static unsigned n_total_pedidos(); // Devuelve cuántos pedidos en total se han hecho (útil para asignar el número de pedido)

    // Clases de excepción
    // Clase de excepción Vacío, lanzada cuando el carrito no tiene ningún artículo, con la intención de evitar crear un pedido vacío.
    class Vacio{
        Usuario* user;
    public:
        Vacio(Usuario* u);
        const Usuario& usuario() const;
    };

    // Clase de excepción Impostor, lanzada cuando el usuario pasado en el constructor no coincide con el titular de la tarjeta usada para el pedido.
    class Impostor{
        Usuario* user;
    public:
        Impostor(Usuario* u);
        const Usuario& usuario() const;
    };

    // Clase de excepción SinStock, lanzada cuando la cantidad de alguno de los artículos del carrito supere las existencias disponibles.
    class SinStock{
        Articulo *item;
    public:
        SinStock(Articulo* _articulo);
        const Articulo& articulo() const;
    };
private:
    size_t n_pedido; // Número de pedido del pedido
    const Tarjeta* const tarjetaPago; // Tarjeta usada para el pedido
    Fecha fechaPedido;  // Fecha en la que se realizó un pedido
    double importeCompra; // Importe total de la compra.
    static size_t numeroPedidos; // Atributo estático que guarda el número total de pedidos que se han hecho.
};

// Operador de inserción de flujo para pedidos.
std::ostream& operator<<(std::ostream& os, const Pedido& order);

#endif //PEDIDO_HPP
