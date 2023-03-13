#include "fecha.hpp"
#include <iostream>

using namespace std;

/* --- Constructores --- */

Fecha::Fecha(int a, int b, int c) : dd(a), mm(b), aaaa(c)// Constructor general
{
    if(a==0){ // Si el día pasado por parámetro es 0 (por omisión o por especificación del usuario)
        dd=ltm->tm_mday; // Se le asignará el dia del sistema
    }
    if (b==0){ // Si el mm pasado por parámetro es 0 (por omisión o por especificación del usuario)
        mm=ltm->tm_mon+1; // Se le asignará el mes del sistema
    }
    if(c==0){ // Si el año pasado por parámetro es 0 (por omisión o por especificación del usuario)
        aaaa=ltm->tm_year+1900; // Se le asignará el año del sistema
    }
    comprobar(); // Se llama a la función que comprueba que la fecha construida es válida.
    // Esta tomará las acciones necesarias en caso de que no lo fuese (informando siempre al usuario del éxito o del tipo de excepción).
}

Fecha::Fecha(const char *cadena) : dd(0), mm(0), aaaa(0){
    int day=0;
    int month=0;
    int year=0;

        int conversiones = sscanf(cadena, "%d/%d/%d", &day, &month, &year);
        if (conversiones!=3){
            throw Fecha::Invalida ("Formato no valido."); // Se lanza una excepción indicando el error en el formato.
            //Fecha(0,0,0);
        }else{
            if(day==0){ // Si el día pasado por parámetro es 0 (por omisión o por especificación del usuario)
                dd=ltm->tm_mday; // Se le asignará el dia del sistema
            }else{
                dd=day; // En caso contrario, se le asignará el día pasado por parámetro
            }
            if (month==0){ // Si el mm pasado por parámetro es 0 (por omisión o por especificación del usuario)
                mm=ltm->tm_mon+1; // Se le asignará el mes del sistema
            }else{
                mm=month; // En caso contrario, se le asignará el mes pasado por parámetro
            }
            if(year==0){ // Si el año pasado por parámetro es 0 (por omisión o por especificación del usuario)
                aaaa=ltm->tm_year+1900; // Se le asignará el año del sistema
            }else {
                aaaa = year; // En caso contrario, se le asignará el año pasado por parámetro
            }
                comprobar();
        }
}

/* --- Comprobantes y corregidores de fechas --- */

int Fecha::diaMax() const { // Función que devuelve el día máximo según el mes
    int mesEquivalente = mm; // Variable donde almacenaremos el mes equivalente en caso de que este fuese superior a 12
    // Esto nos permitirá hacer operaciones de fechas con exactitud (atendiendo a todos los factores).

    if(mesEquivalente>12){
        while(mesEquivalente>12){
            mesEquivalente-=12; // Resta al mes dado 12, para sacar el mes equivalente (del año siguiente en teoría).
        }
    }

    if (mesEquivalente == 2){ // Si el mes es febrero, comprobamos si el año es bisiesto (para determinar si tiene 28 o 29 días).
        if ((aaaa % 4 == 0) && (aaaa % 400 == 0 || aaaa % 100 != 0)){ // Expresión matemática que comprueba si el año dado es bisiesto
            return 29;
        }else{
            return 28;
        }
    }else{ // Si el mes es cualquiera menos febrero, comprobamos si tiene 30 o 31 días.
        switch(mesEquivalente){
            default: break;
            case 1: return 31; // Enero --> 31 días
            case 3: return 31; // Marzo --> 31 días
            case 4: return 30; // Abril --> 30 días
            case 5: return 31; // Mayo --> 31 días
            case 6: return 30; // Junio --> 30 días
            case 7: return 31; // Julio --> 31 días
            case 8: return 31; // Agosto --> 31 días
            case 9: return 30; // Septiembre --> 30 días
            case 10: return 31; // Octubre --> 31 días
            case 11: return 30; // Noviembre --> 30 días
            case 12: return 31; // Diciembre --> 31 días
        }
    }
    return 0; // Realmente no es necesario, pero así el compilador no se queja de no devolver ningún valor :)
}

int Fecha::valida() const{

    int codigoError = 0; // Variable para almacenar el código de error, que luego nos especificará la excepción surgida.

    if ((dia() <= 0)||(dia() > diaMax())){
        codigoError += 1; // Que el día pasado por parámetro no sea válido tendrá valor 1
    }
    if ((mes()<=0)||(mes()>12)){
        codigoError += 2; // Que el mes pasado por parámetro no sea válido tendrá valor 2
    }
    if ((anno()<AnnoMinimo)||(anno()>AnnoMaximo)) {
        //codigoError += 4; // Que el año pasado por parámetro no sea válido tendrá valor 4
        throw Fecha::Invalida("Desbordamiento, anno fuera de rango. No se puede normalizar la fecha");
    }
    /* -- Tabla de códigos de errores -- */
    // 0 --> Ningún error.
    // 1 --> Día invalido.
    // 2 --> Mes invalido.
    // 3 --> Día y mes inválidos.
    // 4 --> Año invalido.
    // 5 --> Día y Año inválidos.
    // 6 --> Mes y año inválidos.
    // 7 --> Día, Mes y Años inválidos.
    // Al no poder normalizar la fecha fuera de los rangos de años máximo y mínimos, nunca alcanzaremos valores iguales o superiores a 4, sino que directamente lanzaremos una excepción.

    return codigoError; // Devuelve el código de error.
}

void Fecha::comprobar() {
    const char *aux; // Cadena que guardará el mensaje de error del código correspondiente.
    int aceptada = valida(); // Entero que guarda el valor del código error del código correspondiente.
    // El motivo de la creación de una variable que guarda el valor de una función que devuelve un int, es evitar hacer la comprobación más de una vez, ahorrando tiempo de ejecución.
    switch(aceptada) {
        default:
            break;
        case 1:
            aux = "Dia invalido.";
            break;
        case 2:
            aux = "Mes invalido.";
            break;
        case 4:
            aux = "Ano invalido.";
            break;
        case 3:
            aux = "Dia y Mes invalido.";
            break;
        case 5:
            aux = "Dia y Ano invalido.";
            break;
        case 6:
            aux = "Mes y Ano invalido.";
            break;
        case 7:
            aux = "Dia, Mes y Ano invalido.";
            break;
        }
    if (aceptada!=0){ // Si existiese algún error (traducido a un valor distinto de 0) se procede a lanzar la excepción.
        corregir();
        throw Fecha::Invalida(aux); // Throw lanza una excepción con el mensaje de error correspondiente.
        // Catch recogerá esta excepción (espera un tipo igual a la excepción que se lanza [en este caso de tipo Inválida]).
    }
}

Fecha::Invalida::Invalida(const char *Info):errorInfo(Info){ // Constructor de la clase Inválida
    // Inicializa errorInfo (esto no puede ser modificado en ningún otro momento, ya que está declarado como const)
}

const char* Fecha::Invalida::por_que() const {
    return errorInfo; // Devuelve el mensaje de error almacenado en la variable. Esto será dirigido al flujo de salida de errores.
}

void Fecha::corregir() {
    ltm->tm_year = aaaa - 1900;
    ltm->tm_mon = mm - 1;
    ltm-> tm_mday = dd;
    mktime(ltm);
    if ((ltm->tm_year+1900>AnnoMaximo)||(ltm->tm_year+1900<AnnoMinimo)){
        //Inválida e("Desbordamiento de años, no se puede normalizar la fecha");
        throw Fecha::Invalida("Desbordamiento de años, no se puede normalizar la fecha");
    }else{
        aaaa=ltm->tm_year+1900;
        mm=ltm->tm_mon+1;
        dd=ltm->tm_mday;
    }
}

/* --- Funciones observadoras --- */
int Fecha::dia() const{
    return dd;
}

int Fecha::mes() const{
    return mm;
}

int Fecha::anno() const{
    return aaaa;
}


/* --- Operadores --- */
Fecha& Fecha::operator+=(int i) { // Operador suma-igual (f+=i)
    dd+=i;
    try {
        comprobar();
    } catch (Fecha::Invalida &e){
        corregir();
    }
    return *this;
}

Fecha& Fecha::operator-=(int i){ // Operador resta-igual (f-=i)
    *this += -i;
    return *this;
}

// Diferencias entre pre y post
// Al hacer una pre-operación, imaginemos que f = ++p, la operación de incremento se realizará antes de la asignación,
// por lo que devolveremos el propio objeto modificado (por referencia).
// Sin embargo, al hacer una post-operación, imaginemos que f = p++, la operación de incremento se realizará después de la asignación,
// teniendo que devolver, en este caso, una fecha (por copia, ya que creamos una fecha temporal y local al ámbito de la función)
// con el estado anterior a la operación.

Fecha& Fecha::operator++(){ // Operador pre-incremento (++f)
    *this += 1;
    return *this;
}

Fecha Fecha::operator++(int){ // Operador post-incremento (f++)
    Fecha f(*this);
    *this += 1;
    return f;
}

Fecha& Fecha::operator--(){ // Operador pre-decremento
    *this += -1;
    return *this;
}

Fecha Fecha::operator--(int){ // Operador post-decremento
    Fecha f(*this);
    *this += -1;
    return f;
}

Fecha operator+(const Fecha& f, int i) { // Operador aritmético suma
    Fecha t(f);
    t+=i;
    return t;
}

Fecha operator-(const Fecha& f, int i){ // Operador aritmético resta
    Fecha t(f);
    t-=i;
    return t;
}


bool operator==(const Fecha& f1, const Fecha& f2){ // Operador lógico comparación igual que (==)
    return (f1.dia()==f2.dia() && f1.mes()==f2.mes() && f1.anno()==f2.anno());
}

bool operator!=(const Fecha& f1, const Fecha& f2){ // Operador lógico comparación no igual que (!=)
    return !(f1==f2);
}

bool operator< (const Fecha& f1, const Fecha& f2){ // Operador lógico comparación menor que (<)
    if(f1.anno() < f2.anno()){
        return true;
    }else if (f1.anno() > f2.anno()){
        return false;
    }else if(f1.mes() < f2.mes()){
        return true;
    }else if (f1.mes() > f2.mes()){
        return false;
    }else if(f1.dia() < f2.dia()){
        return true;
    }else if (f1.dia() > f2.dia()){
        return false;
    }
    return false; // Realmente no es necesario, pero así el compilador no se queja de haber llegado al final de una función sin devolver algo :)
}

bool operator> (const Fecha& f1, const Fecha& f2){ // Operador lógico comparación mayor que (>)
    return (f2 < f1);
}

bool operator>=(const Fecha& f1, const Fecha& f2){ // Operador lógico comparación mayor o igual que (>=)
    return !(f1 < f2);
}

bool operator<=(const Fecha& f1, const Fecha& f2) { // Operador lógico comparación menor o igual que (<=)
    return !(f2 < f1);
}

const char * Fecha::cadena () const {
    char* cadena = new char[50];
    tm t={};
    t.tm_mday=dia();
    t.tm_mon=mes()-1;
    t.tm_year=anno()-1900;
    mktime(&t);
    strftime(cadena, 50, "%A %d de %B de %Y", &t);
    return cadena;
}

std::ostream& operator<<(std::ostream& salida, const Fecha& f){ // Operador de inserción (en flujo)
    //const char* aux = f.cadena(); // Capta el puntero devuelto por cadena()
    salida << f.cadena(); // Inserta el contenido de cadena() en el flujo de salida
    //delete [] aux; // Elimina el contenido de aux (puntero devuelto por cadena)
    return salida;
}

std::istream& operator>>(std::istream& entrada, Fecha& f){ // Operador de extracción (de flujo)
    char cad [11]="00/00/0000"; // Creamos una cadena de caracteres de 11 caracteres, para que se ajuste al formato de entrada
    // No hace falta declararla como un puntero (o cadena de caracteres de bajo nivel dinámica) porque sabemos el tamaño exacto que va a tener en todo momento.
    try{
        // a dd/mm/aaaa\0
        entrada.width(11); // Ajustamos el tamaño del flujo de entrada a 11, los mismos que los caracteres a almacenar
        entrada >> cad; // Introducimos en la cadena de caracteres el contenido del flujo de entrada
        f = cad; // Al no haber un operador de asignación declarado para asignar una cadena de caracteres a una fecha,
        // se hará una llamada implícita al constructor de Fecha que recibe por parámetro una cadena de caracteres,
        // y este devolverá una Fecha creada a partir de dicha cadena, pudiéndose hacer uso del operador de asignación
        // definido trivialmente.
        // En caso de no ser correcto el formato a la hora de crear dicha cadena, se lanzará una excepción que será
        // recogida por el catch de debajo.
    }catch(const Fecha::Invalida& e){
        entrada.setstate(std::ios::failbit); // Se marca el estado del flujo de entrada como erróneo
        throw Fecha::Invalida("Error en el operador >>. Asegúrese de introducir la fecha correctamente (Formato : dd/mm/aaaa). "); // Se lanza una excepción.
    }
    return entrada; // Se devuelve para poder concatenar operaciones
}

