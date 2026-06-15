#pragma once

#include <iostream>
#include <vector>

#include "Carta.h"
#include "Logger.h"
#include "properties.h"

class Manager;
class Jugador{
public:
	Jugador();
	Jugador(int id, Manager* mg);
	virtual void Reset();

	virtual void JugarTurno() = 0;
	virtual std::string identificar();
	// virtual void GetRecompensa(std::vector<Carta> cartas, bool win, int jugada);

	int getID() const{ return _id; };
	int getPoints() const { return _puntos; }
	virtual void setCarta(Carta carta);
	
	void addPuntos(int pts) { _puntos += pts; }
	std::ostringstream& ShowHand(std::ostringstream& salida);
protected:

	bool CambiarPinte(Carta * carta);
	void JugarCarta(int pos);

	const int _id;
	std::vector<Carta> _mano;
	const int _mano_MAX = 3;
	
	int _puntos = 0;
	Manager* _manager;
};