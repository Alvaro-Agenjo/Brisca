#include "Jugador_Basic.h"
#include "Manager.h"
#include "algorithm"


Jugador_BS::Jugador_BS(int id, Manager* mg) : Jugador_RULES(id, mg)
{}


std::string Jugador_BS::identificar() {
	std::ostringstream str;
	str << "[PLAYER] IA_RULES_BS, id: " << Jugador::identificar();
	return str.str();
}



int Jugador_BS::Rules(std::vector<Carta> mesa){

	int score[3] = {0, 0, 0};
	int min_inx = 100, max_inx = 0;
	//Prioridad base [10-19]
	//ordenamos en cuatro grupos 10-20 Pinte_Punto, 20-30 NoPinte_Punto, 30-40 Pinte_NoPunto, 40-50 NoPinte_NoPunto. 
	for(int i = 0; i < _mano.size(); i++){
		score[i] = (_mano[i]._prioridad +20);
		if(_mano[i]._palo == _manager->getPinte()._palo)
			score[i] -= 10;

		if(_mano[i]._puntuacion == 0){
			score[i] +=10;
		}
		if(_mano[i]._puntuacion > 0){
			score[i] -=10;
		}

		if(score[i] > score[max_inx])
			max_inx = i;
		else if(score[i] < score[min_inx])
			min_inx = i;
	}

	if (mesa.size() == 0) //si no hay carta sobre la mesa tiramos la carta menor valorada (prioridad máxima)
		return max_inx;
	else
		return min_inx; //si hay carta, la más valorada
}