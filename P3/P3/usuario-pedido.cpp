#include "usuario-pedido.hpp"
#include "usuario.hpp"

// Métodos sobrecargados asocia(), encargados de realizar las asociaciones
void Usuario_Pedido::asocia(Usuario &user, Pedido &order) {
    usuarioPedido[&user].insert(&order);
    pedidoUsuario.insert(make_pair(&order,&user));
}

void Usuario_Pedido::asocia(Pedido &order, Usuario &user) {
    asocia(user,order); // Llama al asocia() definido anteriormente, con los parámetros invertidos (respecto a esta función).
}

// El método pedidos recibe un Usuario y devuelve los pedidos que haya hecho ese usuario.
Usuario_Pedido::Pedidos &Usuario_Pedido::pedidos(Usuario &user) {
    return usuarioPedido.find(&user)->second;
}

// El método cliente recibe un Pedido y devuelve la dirección de memoria del usuario que ha hecho el pedido.
Usuario *Usuario_Pedido::cliente(Pedido &order) {
    return pedidoUsuario.find(&order)->second;
}
