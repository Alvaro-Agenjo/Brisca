#pragma once
#include "Jugador.h"

class Jugador_Humano : public Jugador{
public: 
	Jugador_Humano(int id, Manager * mg);
	void JugarTurno() override;
	std::string identificar() override;
};


