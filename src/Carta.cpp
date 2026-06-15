#include "Carta.h"

Carta::Carta() : _prioridad(1000), _puntuacion(-1), _posicion(-1), _palo(PALO::UNDEFINED)
{}

Carta::Carta(int posicion){
    _posicion = posicion;
    int palo = posicion / 10;
    
    switch (palo) {
    case 0:
        _palo = PALO::OROS;
        break;
    case 1:
        _palo = PALO::COPAS;
        break;
    case 2:
        _palo = PALO::ESPADAS;
        break;
    case 3:
        _palo = PALO::BASTOS;
        break;
    };

    
    int carta = posicion % 10;
    //_prioridad = carta + 10;    //prioridad más alta --> As de pinte (0). prioridad base = prioridad_pinte + 10;

    switch (carta) {
    case 0: //As
        _puntuacion = 11;
        _prioridad = 10;
        break;
    case 2: //Tres
        _puntuacion = 10;   
        _prioridad = 11;
        break;
    case 9: //Rey
        _puntuacion = 4;
        _prioridad = 12;
        break;
    case 8: //Caballo
        _puntuacion = 3;
        _prioridad = 13;
        break;
    case 7: //Sota
        _puntuacion = 2;
        _prioridad = 14;
        break;
    case 6: //Siete
        _puntuacion = 0;
        _prioridad = 15;
        break;
    case 5: //Seis
        _puntuacion = 0;
        _prioridad = 16;
        break;
    case 4: //Cinco
        _puntuacion = 0;
        _prioridad = 17;
        break;
    case 3: //Cuatro
        _puntuacion = 0;
        _prioridad = 18;
        break;
    case 1: //Dos
        _puntuacion = 0;
        _prioridad = 19;
        break;
    };

    
}

std::ostream& operator<<(std::ostream& o, const Carta& c) {
    int carta = c._posicion % 10;
    switch (carta) {
    case 0: o << "As "; break;
    case 1: o << "Dos "; break;
    case 2: o << "Tres "; break;
    case 3: o << "Cuatro "; break;
    case 4: o << "Cinco "; break;
    case 5: o << "Seis "; break;
    case 6: o << "Siete "; break;
    case 7: o << "Sota "; break;
    case 8: o << "Caballo "; break;
    case 9: o << "Rey "; break;

    }
    o << "de ";
    switch (c._palo) {
    case PALO::OROS: o << "Oros"; break;
    case PALO::COPAS: o << "Copas"; break;
    case PALO::ESPADAS: o << "Espadas"; break;
    case PALO::BASTOS: o << "Bastos"; break;
    }
    o << std::endl;
    return o;
}

bool operator==(const Carta& c1, const Carta& c2){
    return (c1._posicion == c2._posicion);
}
bool operator!=(const Carta& c1, const Carta& c2){
    return !(c1 == c2);
}
bool operator<=(const Carta& c1, const Carta& c2){
    return c1._posicion <= c2._posicion;
}
bool operator>=(const Carta& c1, const Carta& c2){
    return c1._posicion <= c2._posicion;
}
bool operator<(const Carta& c1, const Carta& c2){
    return (c2 >= c1);
}
bool operator>(const Carta& c1, const Carta& c2){
    return (c2 <= c1);
}
