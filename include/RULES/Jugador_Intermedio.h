#include "Jugador_RULES.h"

class Jugador_IM : public Jugador_RULES{
public:
	Jugador_IM(int id, Manager* mg);
	std::string identificar() override;

private:
    int Rules(std::vector<Carta> mesa) override;
};

