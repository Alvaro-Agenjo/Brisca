#include "Jugador.h"
#include "Manager.h"

Jugador::Jugador() : _id(-1), _manager(nullptr)
{}

Jugador::Jugador(int id, Manager *mg) : _id(id), _manager(mg)
{}

void Jugador::Reset(){
	_mano.clear();
	_puntos = 0;
}

std::string Jugador::identificar(){
	std::ostringstream str;
	str << getID() + 1 << "\t";
	return str.str();
}

// void Jugador::GetRecompensa(std::vector<Carta> cartas, bool win, int jugada)
// {}

void Jugador::setCarta(Carta carta){
	if (_mano.size() < _mano_MAX)
		_mano.push_back(carta);
	else 
		Logger::Viz("[ERROR] try to take more cards than capable");
}

std::ostringstream& Jugador::ShowHand(std::ostringstream& salida){
	for (int i = 0; i < _mano.size(); i++) {
		salida<< "\t" << _mano[i];
	}
	return salida;
}

bool Jugador::CambiarPinte(Carta* carta){
	if (!_manager->CambioPinte(carta)) {
		Logger::Viz("No puedes intercambiar la carta seleccionada con la pinte.");
		return false;
	}
	else {
		Logger::Log(std::stringstream() << "[PINTE] Jugador " << _id + 1 << " cambio la pinte.");
		Logger::Log(std::stringstream() << " Nueva pinte: " << _manager->getPinte());
		return true;
	}
}

void Jugador::JugarCarta(int pos){
	if (pos >= _mano.size()) return;
	Logger::Viz(std::stringstream() << _mano[pos]);
	Logger::Log(std::stringstream() << "[PLAY] Jugador " << _id + 1 << ": " << _mano[pos]);
	_manager->CartaEnMesa(_mano[pos]);
	_mano.erase(_mano.begin() + pos);
}
