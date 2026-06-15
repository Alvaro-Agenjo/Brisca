#pragma once


#include "Jugador_RULES.h"

class Jugador_BS : public Jugador_RULES{
public:
	Jugador_BS(int id, Manager* mg);
	std::string identificar() override;

private:
    int Rules(std::vector<Carta> mesa) override;
};

