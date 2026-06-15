#include "Jugador_RND.h"
Jugador_RND::Jugador_RND(int id, Manager* mg) : Jugador(id, mg), _rnd_generator(std::random_device{}()), _URD(0.0,1.0)
{}

void Jugador_RND::JugarTurno() {
	constexpr double paccion = 0.05;	
	Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
	
	bool jugada_realizada = false;
	do {
		std::uniform_int_distribution<> _UID(0, _mano.size() - 1);
		int selected_card = _UID(_rnd_generator);
		if (paccion > _URD(_rnd_generator)){
			CambiarPinte(&_mano[selected_card]);
			Logger::Viz("\n");
		}
		else {
			Logger::Viz(std::stringstream() << selected_card + 1 << "->");
			JugarCarta(selected_card);
			jugada_realizada = true;
		}
	} while (jugada_realizada != true);
}

std::string Jugador_RND::identificar() {
	std::ostringstream str;
	str << "[PLAYER] IA_RND, id: " << Jugador::identificar();
	return str.str();
}

void Jugador_RND::Reset(){
	_rnd_generator.seed(std::random_device{}());
	Jugador::Reset();
}
