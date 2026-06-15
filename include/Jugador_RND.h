#pragma once
#include <random>

#include "Jugador.h"

class Jugador_RND : public Jugador{
public:
	Jugador_RND(int id, Manager* mg);
	void JugarTurno() override;
	std::string identificar() override;
	void Reset() override;
	
private:
	std::mt19937 _rnd_generator;
	std::uniform_real_distribution<double> _URD;
};

