#pragma once
#include "Jugador.h"

class Jugador_RULES : public Jugador{
public:
	Jugador_RULES(int id, Manager* mg);
	void JugarTurno() override;
	virtual std::string identificar() = 0;
	void Reset() override;

protected:
    virtual int Rules(std::vector<Carta> mesa) = 0;
	bool EnfrentarCarta(Carta mi_carta, Carta oponente);

};

