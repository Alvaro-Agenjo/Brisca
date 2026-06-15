#pragma once
#include <iostream>
#include "Data_types.h"

struct Carta {
	PALO _palo;			//Representa el palo de la baraja
	
	int _prioridad;		//Valor a la hora de computar el vencedor entre varias cartas

	int _puntuacion;		//Puntuaci�n adquirida al "ganar" dicha carta
	int _posicion;		//Posicion de la carta entre las 40 de la baraja. Se utiliza para identificarla

	Carta();
	Carta(int posicion);
};

std::ostream& operator<<(std::ostream& o, const Carta& c);
bool operator==(const Carta& c1, const Carta& c2);
