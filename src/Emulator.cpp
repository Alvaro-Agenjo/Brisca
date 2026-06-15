#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

#include "Manager.h"
#include "properties.h"

int main(){
	int wins = 0, ties = 0, points = 0;
	Manager* manager = new Manager(jugadores);
	for (int i = 0; i < PARTIDAS; i++) {
		//Juego
		while (manager->getState() != ESTADO::FIN)
			manager->GameFlow();

		//Resultados del jugador elegido
		RESULTADOS r = manager->getResultsPlayer(0);

		(r.victoria < 0)? ties++ : wins+=r.victoria;
		if (i > PARTIDAS-100)
			points += r.puntos;
		
		manager->GameFlow();

		if(!(i%INFO))
			std::cout << "Iteration: " << i << std::endl;
	}
	manager->ShowTraining();
	std::cout << "WINS: " << wins << "/" << PARTIDAS << " | TIES: " << ties << "/" << PARTIDAS << std::endl;
	std::cout << "Mean points obtained per play during the last 100 games: " << points / 100 << std::endl;
}