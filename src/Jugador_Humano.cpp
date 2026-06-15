#include "Jugador_Humano.h"
#include <conio_rep.h>

Jugador_Humano::Jugador_Humano(int id, Manager * mg) : Jugador(id, mg)
{}

void Jugador_Humano::JugarTurno(){
	// seleccionar carta
	char input;
	int selected_card = 0;

	Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
	do {
		Logger::Viz(std::stringstream() << selected_card + 1 << "->");

		// input = _getch();
		input = GETCH();
		if (input <= 122 && input >= 97) input -= 32;

		switch (input) {
		case 'A':
			selected_card = (selected_card + _mano.size()-1) % _mano.size();
			break;
		case 'D':
			selected_card = (selected_card + 1) % _mano.size();
			break;
		case 'P':
			CambiarPinte(&_mano[selected_card]);
			break;
		case 27:
			exit(6);
		}
	} while (input != enter);

	JugarCarta(selected_card);
}

std::string Jugador_Humano::identificar(){
	std::ostringstream str;
	str << "[PLAYER] HUMAN, id: " << Jugador::identificar();
	return str.str();
}