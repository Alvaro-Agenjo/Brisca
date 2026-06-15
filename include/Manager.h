#pragma once

#include "Jugador_Humano.h"
#include "Jugador_RND.h"
#include "Jugador_QLT.h"
#include "Jugador_DQL.h"
#include "Jugador_DQL_mem.h"
#include "Jugador_Basic.h"
#include "Jugador_Avanzado.h"

class Manager{
public:
	Manager(const TYPE_JUGADOR* jugadores);

	int GameFlow();
	void RepartirCarta(Jugador* jugador);

	bool CambioPinte(Carta* carta);
	void CartaEnMesa(Carta carta);
	
	const ESTADO& getState() { return _estado; }
	const Carta& getPinte() { return _pinte; }
	const std::vector<Carta> getMesa() { return _cartas_en_juego; }
	const RESULTADOS getResultsPlayer(int player_id);



	
	//Debug
	std::string ShowBaraja();
	std::string ShowPlayerHands();
	std::string ShowPinte();
	void ShowTraining();

	//Sudo
	void AnadirCarta(int carta, int jugador);
	void SetPinte(int carta);

private:
	void CrearJugador(TYPE_JUGADOR tipo, int id);
	Carta DarCarta();
	int Resolver();
	bool AplicarVentaja(Carta *mejor, Carta actual);
	void Reset();
	
	static int _j_inicial;
	const int _rondas_max;
	int _ronda_actual;
	static int _cartas_restantes;
	bool _baraja[40] = {}; //representa las cartas en la baraja, 0 si permanecen, 1 si no est�n.

	std::mt19937 _rnd_generator;
	std::uniform_int_distribution<int> _UID;

	const int _n_jugadores = 2;
	std::vector<Jugador*> _jugadores;
	std::vector<Carta> _cartas_en_juego;
	Carta _pinte;

	ESTADO _estado = ESTADO::INICIO;
	Jugador* _jugador_inicial = {};
};

