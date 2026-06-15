#pragma once

#include "Jugador_RULES.h"

class Jugador_AV : public Jugador_RULES{
public:
	Jugador_AV(int id, Manager* mg);
	std::string identificar() override;

private:
    int Rules(std::vector<Carta> mesa) override;
};

