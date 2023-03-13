#include "cadena.hpp"
#include <iostream>
#include <cstring>


using namespace std;

Cadena::Cadena(size_t i, char c):s_(new char[i+1]), tam_(i){ // La cadena dinámica dispone de un hueco más para almacenar el carácter de fin de cadena '\0'
    if(s_!=nullptr){ // Comprobamos si el S.O ha podido asignar una zona de memoria a la cadena dinámica s_
        while((i--)>0){ // i decrece en cada comprobación (incluida la primera)
            s_[i]=c; // Se rellena la cadena con el carácter dado
        }
        s_[length()]='\0'; // Carácter de fin de cadena
    }
}

Cadena::Cadena(const Cadena &cad): s_(new char [cad.length()+1]), tam_(cad.length()) { // La cadena dinámica dispone de un hueco más para almacenar el carácter de fin de cadena '\0'
    if(s_!=nullptr){ // Comprobamos si el S.O ha podido asignar una zona de memoria a la cadena dinámica s_
        strcpy(s_,cad.s_); // Función de <cstring> que copia la cadena de caracteres cad.s_ en s_
    }
}

Cadena::Cadena(const char *cad) : s_(new char [strlen(cad)+1]), tam_(strlen(cad)) { // La cadena dinámica dispone de un hueco más para almacenar el carácter de fin de cadena '\0'
    if(s_!=nullptr){ // Comprobamos si el S.O ha podido asignar una zona de memoria a la cadena dinámica s_
        strcpy(s_,cad); // Función de <cstring> que copia la cadena de caracteres cad.s_ en s_
        s_[length()]='\0'; // Carácter de fin de cadena
    }
}

// El operador de movimiento tiene como objetivo evitar la copia innecesaria de grandes cantidades de información.
// Esto se consigue moviendo los punteros en vez de mover la información contenida en los mismos.
Cadena::Cadena(Cadena &&cad) noexcept: s_(cad.s_), tam_(cad.length()) {
    // Inicializamos tam_ al tamaño de la cadena pasada.
    // s_ al ser una variable de tipo puntero (que apunta al primer carácter del vector) podemos asignarle una cadena de caracteres (mediante el new que le asignará una zona de memoria donde se encuentra el nuevo vector)
    // o un puntero a otra cadena de caracteres ya existente.
    // Inicializamos s_ con la dirección de memoria del vector de caracteres de la cadena pasada.
    cad.tam_=0; // La cadena pasada queda vacía (movimiento)
    cad.s_=nullptr; // La cadena pasada queda vacía (movimiento)
}

// Destructor
Cadena::~Cadena() { // Se llama cada vez que un objeto sale del ámbito local o el programa finaliza
    tam_=0;
    delete [] s_; // Libera la memoria dinámica asignada a la cadena
}


//Operadores de índice
const char& Cadena::operator[](size_t i) const {
    return s_[i]; // Devuelve el elemento i de la cadena de caracteres (al ser una función de Cadena puede acceder a los atributos privados)
}

char& Cadena::operator[](size_t i) {
    return s_[i]; // Devuelve el elemento i de la cadena de caracteres (al ser una función de Cadena puede acceder a los atributos privados)
}

char& Cadena::at(size_t i) const {
    if((i>=length())||(i<0)){ // Igual que el operador [] pero comprueba que el índice dado se encuentra dentro del rango de la cadena de caracteres.
        throw out_of_range ("Indice fuera de rango de la Cadena"); // Si no se encuentra el índice en el rango lanza una excepción
    }else{
        return s_[i]; // Si está dentro del rango, devuelve el elemento i de la cadena de caracteres.
    }
}

char& Cadena::at(size_t i){
    if(i>=length()){ // Igual que el operador [] pero comprueba que el índice dado se encuentra dentro del rango de la cadena de caracteres.
        throw out_of_range ("Indice fuera de rango de la Cadena"); // Si no se encuentra el índice en el rango lanza una excepción
    }else{
        return s_[i]; // Si está dentro del rango, devuelve el elemento i de la cadena de caracteres.
    }
}

// Función substr
Cadena Cadena::substr(size_t desplazamiento, size_t tamano) const{
    if((desplazamiento+tamano>length())||(desplazamiento>length())||(tamano>length())){ // La suma del tamaño y el desplazamiento es el índice máximo al que se pretende acceder
        throw out_of_range("Indice fuera de rango de la Cadena"); // Si la suma excediese el número máximo de índice, lanzaría una excepción.
    }else{
        char* aux = new char [tamano+1]; // Creamos una cadena de caracteres del tamaño deseado (+1 para almacenar el carácter de fin de cadena)
        if (aux!= nullptr){
            aux[tamano]='\0'; // Carácter de fin de cadena
            while((tamano--)>0){ // Mientras que el tamaño sea mayor que 0, se copiarán los datos
                aux[tamano]=s_[desplazamiento+tamano]; // Copia los datos en aux[i] a partir de s_ y un desplazamiento y un tamaño dados a copiar
            }
            Cadena nueva(aux); // Creamos un objeto de Cadena con otra cadena dada, en este caso la que hemos creado para la ocasión.
            delete [] aux;
            return nueva; // Devolvemos la nueva cadena creada
        }
    }
    return nullptr; // Realmente no es necesario, pero así el compilador no se queja de haber llegado al final de una función sin devolver algo :)
}

// Función de conversión explícita
const char * Cadena::c_str() const { // Ni la referencia ni el valor del puntero podrá ser modificado
    return s_; // Devuelve un puntero a la cadena de caracteres de bajo nivel
}

// Operadores de asignación y concatenación
Cadena &Cadena::operator= (const Cadena &cad) { // El operador = sobreescribe el contenido de la cadena (en este caso con el de otra cadena)
    if(cad!=*this){
        tam_=cad.length(); // Igualamos el tamaño al de la cadena pasada
        delete[] s_; // Eliminamos la cadena de caracteres dinámica
        s_=new char[length()+1]; // Creamos una nueva cadena de caracteres dinámica con el nuevo tamaño (+1 para guardar el carácter de fin de cadena)
        strcpy(s_,cad.s_); // Función de <cstring> que copia la cadena de caracteres cad.s_ en s_
    }
    return *this; // Devuelve el objeto con el que se llamó a la función
}

Cadena &Cadena::operator= (const char *c) {
    tam_=strlen(c); // Función de <cstring> que determina el tamaño de la cadena de caracteres pasada por parámetro. Se cambia el valor de tamaño
    delete [] s_; // Eliminamos la cadena de caracteres dinámica
    s_= new char [length()+1]; // Creamos una nueva cadena de caracteres dinámica con el nuevo tamaño (+1 para guardar el carácter de fin de cadena)
    strcpy(s_,c); // Función de <cstring> que copia la cadena de caracteres 'c' en s_
    return *this; // Devuelve el objeto con el que se llamó a la función
}

Cadena &Cadena::operator=(Cadena &&cad) noexcept {
    if(cad!=*this){
        tam_=cad.length(); // Cambiamos el tamaño al tamaño de la cadena pasada.
        delete [] s_; // Liberamos la memoria del vector al que hacía referencia s_
        // Esto en el constructor no era necesario porque s_ no tenía asignada ningún vector.
        s_=cad.s_; // Se le asigna la dirección de memoria donde se encuentra el vector de caracteres de la cadena pasada.
        cad.tam_=0; // La cadena pasada queda vacía (movimiento)
        cad.s_= nullptr; // La cadena pasada queda vacía (movimiento)
    }
    return *this;
}

Cadena &Cadena::operator+=(const Cadena &cad) { // Concatena a la Cadena que llama a la función una cadena pasada por parámetro
    char* aux = new char [length() + cad.length() +1]; // Se crea una cadena de caracteres (de bajo nivel) de tamaño igual a la suma de los tamaños de ambas cadenas (+1 para el carácter de fin de cadena)
    size_t i=length(); // Variable para recorrer el bucle

    if(aux!= nullptr){
        while((i--)>0){ // Copia el contenido de la cadena de caracteres de la Cadena que llama a la función a la cadena de caracteres auxiliar (que almacenará la nueva cadena concatenada)
            aux[i]=s_[i]; // Mientras i (que se decrementa en cada comprobación, incluyendo la primera) sea mayor que 0, se copiará carácter a carácter de una cadena a otra.
        }
        i=length()+cad.length(); // Volvemos a asignarle a la variable de bucle el tamaño de la cadena (que llama a la función) + el tamaño de la cadena a concatenar

        while((i--)>length()){ // Ídem al bucle anterior, pero ahora parará cuando sea igual a la longitud de la primera cadena (tiene un desplazamiento dado [length()])
            aux[i]=cad.s_[i-length()]; // Mientras i (que se decrementa en cada comprobación, incluyendo la primera) sea mayor que 0, se copiará carácter a carácter de una cadena a otra.
        }
        delete [] s_; // Eliminamos la cadena de caracteres dinámica
        s_ = new char [length() + cad.length() + 1];
        strcpy(s_,aux);
        // ¿Por qué no hemos llamado antes a strcpy para copiar los elementos de ambas cadenas?
        // strcpy es una función que sobreescribe el contenido del destino, por lo que si hubiésemos llamado a strcpy cuando hemos copiado de ambas cadenas,
        // hubiésemos perdido la información de la primera cadena.
        // Aquí podemos llamar a "strcpy" porque copiamos la cadena ya concatenada en la propia del objeto de la clase Cadena.
        delete [] aux; // Ya no nos es necesaria mantener esa cadena de caracteres (la hemos almacenado en s_)
        s_[length()+ cad.length()]='\0'; // Carácter de fin de cadena
        tam_+=cad.length(); // Al tamaño inicial le sumamos el tamaño de la cadena pasada por parámetro
    }
    return *this; // Devolvemos el objeto que llamó a la función
}

// Operador asignación concatenación externo

Cadena operator+(const Cadena &cad1, const Cadena &cad2) {
    Cadena aux(cad1); //Creamos una cadena donde almacenar la concatenación
    aux += cad2; // Hacemos uso del operador previamente declarado
    return aux; // Devolvemos la cadena creada
}

// Podemos hacer uso de las funciones de string, pero el comportamiento definido debajo es el mismo.
// Operadores lógicos

bool operator==(const Cadena& cad1, const Cadena& cad2){
    if(cad1.length()!=cad2.length()){ // Si son de distinta longitud...
        return false; // ... devolvemos falso
    }else{ // Debemos comprobar carácter a carácter si son iguales
        size_t i=cad1.length(); // Inicializamos la variable para recorrer el bucle y las cadenas
        while((i--)>0){
            if(cad1.at(i)!=cad2.at(i)){ // Compara el valor ASCII
                return false; // En el momento que algún carácter no coincida, devolvemos falso
            }
        }
        return true; // Aquí sólo llegará si todas las comprobaciones que se han hecho anteriormente resultan verdaderas, si no se habrá devuelto falso.
    }
}

bool operator!=(const Cadena& cad1, const Cadena& cad2){
    return !(cad1==cad2); // Una vez implementado ==, podemos devolver el valor negado de comparar si son iguales.
}

bool operator >(const Cadena& cad1, const Cadena& cad2){
    if(cad1.length()>cad2.length()){
        return true;
    }else if(cad1.length()<cad2.length()){
        return false;
    }else{
        size_t i = 0, j = 0; // Creamos e inicializamos las variables de bucle
        while ((i<cad1.length())&&(j<cad2.length())){ // Mientras que las dos variables no excedan el tamaño de las cadenas, seguiremos comparando
            if (cad1.at(i)>cad2.at(j)){ // Comparamos el valor ASCII (un valor ASCII mayor que otro significará que dicha palabra está después de la otra alfabéticamente)
                return true; // En el momento que una letra sea mayor que otra (en valor ASCII) devolveremos verdadero.
            }
            i++;
            j++;
        }
        return false; // Aquí sólo llegará si todas las comprobaciones que se han hecho anteriormente en el bucle resultan falsas, si no se habrá devuelto verdadero.
    }
}

bool operator <(const Cadena& cad1, const Cadena& cad2){
    return (cad2>cad1); // Comprobar que cad1 es menor que cad2, es lo mismo que comprobar que cad2 es mayor que cad1, así que podemos hacer uso del operador anterior cambiando el orden de los parámetros.
}

bool operator>=(const Cadena& cad1, const Cadena& cad2){
    if (cad1==cad2){ // Comprobamos si son iguales
        return true;
    }else if (cad1>cad2){ // Comprobamos si cad1 es mayor que cad2
        return true;
    }else{
        return false; // Si no cumplen ninguna de las dos condiciones anteriores, se devuelve falso.
    }
}

bool operator<=(const Cadena& cad1, const Cadena& cad2){
     return (cad2>=cad1); // Comprobar que cad1 es menor o igual que cad2, es lo mismo que comprobar que cad2 es mayor o igual que cad1, así que podemos hacer uso del operador anterior cambiando el orden de los parámetros.
}

// Operadores de flujo
std::ostream& operator<<(std::ostream& salida, const Cadena& cad){
    salida << cad.c_str(); // El operador de conversión explícito introducirá en el flujo de salida la Cadena en bajo nivel
    return salida; // Para poder concatenar operaciones.
}
std::istream& operator>>(std::istream& entrada, Cadena& cad){
    char aux[33]=""; // Creamos una cadena de caracteres de 33 (32 + \0) caracteres, para que se ajuste al formato de entrada
    size_t i = 0; // Acumulador para el bucle
    while (isspace(entrada.get()) != 0 && i < 32)
    {
        i++;
    }

    entrada.unget();
    i = 0;
    
    while(!isspace(entrada.peek()) && i < 32 && entrada.good())
    {
        aux[i] = entrada.get();
        i++;
    }
    aux[i] = '\0';
    cad=aux;
    return entrada;
}
