#ifndef USUARIO_PEDIDO_HPP
#define USUARIO_PEDIDO_HPP

#include <map>
#include <set>

using namespace std;

class Pedido; // Declaración adelantada
class Usuario; // Declaración adelantada

// Representa la asociación bidireccional uno-a-varios entre Usuario y Pedido. Es una clase típica de asociación.
// Tiene dos atributos, uno por cada una de ellas:
// Un diccionario de punteros a Usuario y conjunto de punteros a Pedido. El tipo del conjunto de punteros a Pedido será Pedidos, un tipo miembro de Usuario_Pedido.
// Un diccionario de punteros a Pedido y punteros a Usuario.
class Usuario_Pedido{
public:
    // Definición del sinónimo público del conjunto de punteros a Pedido
    typedef std::set<Pedido*> Pedidos;

    // Definición del sinónimo público del diccionario de punteros a Usuario y el conjunto de punteros a Pedido
    typedef std::map<Usuario*, Pedidos> usPed;

    // Definición del sinónimo del diccionario de punteros a Pedido y punteros a Usuario.
    typedef std::map<Pedido*, Usuario*> pedUs;

    // Métodos sobrecargados asocia(), encargados de realizar las asociaciones
    void asocia(Usuario& user, Pedido& order);
    void asocia(Pedido& order, Usuario& user);

    // El método pedidos recibe un Usuario y devuelve los pedidos que haya hecho ese usuario.
    Pedidos& pedidos(Usuario& user);

    // El método cliente recibe un Pedido y devuelve la dirección de memoria del usuario que ha hecho el pedido.
    Usuario* cliente(Pedido& order);
private:
    usPed usuarioPedido; // std::map<Usuario*, Pedidos>
    pedUs pedidoUsuario; // std::map<Pedido*, Usuario*> pedUs
};

#endif //USUARIO_PEDIDO_HPP
