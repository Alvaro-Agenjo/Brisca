#include "Jugador_RULES.h"
#include "Manager.h"
#include "algorithm"



Jugador_RULES::Jugador_RULES(int id, Manager* mg) : Jugador(id, mg)
{}

void Jugador_RULES::JugarTurno() {
	auto mesa = _manager->getMesa();
	int selected_card = Rules(mesa);

	Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
	Logger::Viz(std::stringstream() << selected_card + 1 << "->");
	JugarCarta(selected_card);
}

void Jugador_RULES::Reset(){
	Jugador::Reset();
}


bool Jugador_RULES::EnfrentarCarta(Carta mi_carta, Carta oponente){
    if (mi_carta._palo != _manager->getPinte()._palo && mi_carta._palo != oponente._palo)  return false;
	if (mi_carta._palo == _manager->getPinte()._palo)
		mi_carta._prioridad -= 10;
	if (oponente._palo == _manager->getPinte()._palo)
		oponente._prioridad -= 10;

	if (mi_carta._prioridad < oponente._prioridad){
		return true;
	}
	return false;
}
