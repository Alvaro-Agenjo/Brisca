#include "Jugador_Avanzado.h"
#include "Manager.h"
#include "algorithm"


enum casos
{
    Pinte_Punto,
    Pinte_NoPunto,
    NoPinte_Punto,
    NoPinte_NoPunto
};


Jugador_AV::Jugador_AV(int id, Manager* mg) : Jugador_RULES(id, mg)
{}


std::string Jugador_AV::identificar() {
	std::ostringstream str;
	str << "[PLAYER] IA_RULES_AV, id: " << Jugador::identificar();
	return str.str();
}


int Jugador_AV::Rules(std::vector<Carta> mesa){

	int score[3] = {0, 0, 0};
	int select_inx = 0;
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

		if(score[i] > score[select_inx])
			select_inx = i;
	}

	if (mesa.size() == 0) //si no hay carta sobre la mesa tiramos la carta menor valorada (prioridad máxima)
		return select_inx;
	//Si no se rellena el score se mantiene a cero y por tanto no influye en la decision (max)

	switch(_mano.size()){
		//Si no existe carta se rellena el score con 100 y por tanto no influye en la decision (min)
		case 1: 
			score[1] = 100;
			score[2] = 100;
			break;
		case 2:
			score[2] = 100;
			break;
	}

	enum casos caso;
	if (mesa[0]._palo == _manager->getPinte()._palo){
		caso = mesa[0]._puntuacion > 0? casos::Pinte_Punto : casos::Pinte_NoPunto;
	}
	else{
		caso = mesa[0]._puntuacion > 0? casos::NoPinte_Punto : casos::NoPinte_NoPunto;
	}

	switch (caso) {
	case casos::Pinte_Punto:{
    /*****************************************
	**  0-10: Pinte_Punto --win--			**
	** 10-20: NoPinte_NoPunto;				**
	** 20-30: Pinte_NoPunto					**
	** 30-40: NoPinte_Punto					**
	** 40-50: Pinte_Punto --loss--.			**
    *****************************************/
		for (int i = 0; i < 3; i++){
			if (score[i] < 20){ //Pinte_Punto
				bool win = EnfrentarCarta(_mano[i], mesa[0]);
				if (win)
					score[i] -= 10;
				else
					score[i] += 30;
			}
			else if (score[i] < 30) // NoPinte_Punto
				score[i] += 10;
			
			else if (score[i] < 40) // Pinte_NoPunto
				score[i] -= 10;
			
			else // NoPinte_NoPunto
				score[i] -= 30;
		}
		break;
	}
	case casos::Pinte_NoPunto:{
	/*****************************************
	** 10-20: NoPinte_NoPunto 				**
	** 20-30: Pinte_NoPunto;				**
	** 30-40: Pinte_Punto					**
	** 40-50: NoPinte_Punto					**
    *****************************************/

		for (int i = 0; i < 3; i++){
			if (score[i] <20) //Pinte_Punto
				score[i] += 20;
			
			else if (score[i] < 30) // NoPinte_Punto
				score[i] += 20;
			
			else if (score[i] < 40) // Pinte_NoPunto
				score[i] -= 10;

			else // NoPinte_NoPunto
				score[i] -= 30;
		}
		break;
	} 
	case casos::NoPinte_Punto:{
	/*****************************************
	**  0-10: NoPinte_Punto --win--			**
	** 10-20: Pinte_NoPunto;				**
	** 20-30: Pinte_Punto					**
	** 30-40: NoPinte_NoPunto				**
	** 40-50: NoPinte_Punto --loss--.		**
    *****************************************/
   		for (int i = 0; i < 3; i++){
			if (score[i] < 20) //Pinte_Punto
				score[i] += 10;
			
			else if (score[i] < 30){ // NoPinte_Punto
				if(EnfrentarCarta(_mano[i], mesa[0]))
					score[i] -= 20;				
				else
					score[i] += 20;
			}
			else if (score[i] < 40) // Pinte_NoPunto
				score[i] -= 20;
			
			else // NoPinte_NoPunto
				score[i] -= 10;
		}
		break;
	}
	case casos::NoPinte_NoPunto:{
	/*****************************************
	**  0-10: NoPinte_Punto --win--			**
	** 10-20: NoPinte_NoPunto;				**
	** 20-30: Pinte_NoPunto					**
	** 30-40: Pinte_Punto					**
	** 40-50: NoPinte_Punto --loss--.		**
    *****************************************/
		for (int i = 0; i < 3; i++){
			if (score[i] < 20) //Pinte_Punto
				score[i] += 20;
			
			else if (score[i] < 30){ // NoPinte_Punto
				if (EnfrentarCarta(_mano[i], mesa[0]))
					score[i] -= 20;
				else
					score[i] += 20;
			}
			else if (score[i] < 40) // Pinte_NoPunto
				score[i] -= 10;
			
			else // NoPinte_NoPunto
				score[i] -= 30;
		}
		break;
	}
	}
	
	for (int i = 0; i < 3; i++){
		if(score[i] < score[select_inx])
			select_inx = i;
	}

	return select_inx;
}
