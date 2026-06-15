#include "Manager.h"
#include "Logger.h"

int Manager::_cartas_restantes = 40;
int Manager::_j_inicial = 0;

Manager::Manager(const TYPE_JUGADOR *jugadores)
	: _n_jugadores(NUM_JUGADORES),
		_rondas_max(_cartas_restantes / NUM_JUGADORES), _UID(0, 39),
		_rnd_generator(std::random_device{}())
{
	Logger::Init();

	for (int n = 0; n < _n_jugadores; n++){
		CrearJugador(jugadores[n], n);
		Logger::Log(_jugadores[n]->identificar());
	}
	_jugador_inicial = _jugadores[_j_inicial];
	_ronda_actual = 0;
	_estado = ESTADO::INICIO;
}

int Manager::GameFlow(){
	switch (_estado){
	case ESTADO::INICIO:{
		_pinte = DarCarta();

		for (int i = 0; i < 3; i++){
			for (int j = 0; j < _n_jugadores; j++){
				RepartirCarta(_jugadores[j]);
			}
		}

		_estado = ESTADO::RONDA;
		break;
	}
	case ESTADO::REPARTO:{
		for (int i = 0; i < _n_jugadores; i++){
			int indice = (i + _jugador_inicial->getID()) % _n_jugadores;
			RepartirCarta(_jugadores[indice]);
		}
		_estado = ESTADO::RONDA;
		break;
	}
	case ESTADO::RONDA:{
		Logger::Viz("\n-----------------------------------------------------\n");
		Logger::Viz(ShowPinte());
		Logger::Viz(ShowPlayerHands());

		Logger::Log(std::stringstream()<< "\n-----------------------------------------------------\n[INFO] Ronda: "<< _ronda_actual + 1 << " |\t" << ShowPinte());
		// Logger::Log(ShowBaraja());
		Logger::Log(ShowPlayerHands());

		for (int n = 0; n < _n_jugadores; n++){
			int indice = (n + _jugador_inicial->getID()) % _n_jugadores;
			if (Jugador_RL *DQL = dynamic_cast<Jugador_RL *>(_jugadores[indice]))
				DQL->learn();
			_jugadores[indice]->JugarTurno();
		}
		_estado = ESTADO::RESOLUCION;
		break;
	}
	case ESTADO::RESOLUCION:{
		int ganador = Resolver();

		for (int n = 0; n < _n_jugadores; n++){
			if (Jugador_RL *DQL = dynamic_cast<Jugador_RL *>(_jugadores[n])){
				DQL->GetRecompensa(_cartas_en_juego, n == ganador, n);
				// break; //Solo se espera que exista un jugador DQL, se comentará en
				// caso de que existan más de uno
			}
		}

		_cartas_en_juego.clear();
		_jugador_inicial = _jugadores[ganador];

		Logger::Viz(std::stringstream()<< "Gana el jugador: " << ganador + 1 << "\n\n");
		Logger::Log(std::stringstream()<< "[INFO] Gan� el jugador: " << ganador + 1);

		if (_ronda_actual < _rondas_max - 3)
			_estado = ESTADO::REPARTO;
		else if (_ronda_actual == _rondas_max - 1)
			_estado = ESTADO::RECUENTO;
		else
			_estado = ESTADO::RONDA;

		_ronda_actual++;
		break;
	}
	case ESTADO::RECUENTO:{
		bool tie = false;
		int ganador = _jugadores[0]->getID();

		if (Jugador_RL *DQL = dynamic_cast<Jugador_RL *>(_jugadores[0]))
			DQL->learn(true, ganador == DQL->getID());

		for (int n = 1; n < _n_jugadores; n++){
			if (_jugadores[n]->getPoints() > _jugadores[ganador]->getPoints()){
				ganador = _jugadores[n]->getID();
				tie = false;
			}
			else if (_jugadores[n]->getPoints() == _jugadores[ganador]->getPoints())
				tie = true;

			if (Jugador_RL *DQL = dynamic_cast<Jugador_RL *>(_jugadores[n]))
				DQL->learn(true, ganador == DQL->getID());
		}

		if (!tie){
			_jugador_inicial = _jugadores[ganador];
			Logger::Viz("\n-----------------------------------------------------\n");
			Logger::Viz(std::stringstream()<< "Jugador: " << ganador + 1 << " gana la partida!!\n");
			Logger::Log("\n-----------------------------------------------------\n");
			Logger::Log(std::stringstream() << "\n[WIN] Jugador: " << ganador + 1 << " gana la partida!!\n");
		}
		else{
			_jugador_inicial = nullptr;
			Logger::Viz("\n-----------------------------------------------------\n");
			Logger::Viz(std::stringstream() << "Empate!!\n");
			Logger::Log("\n-----------------------------------------------------\n");
			Logger::Log(std::stringstream() << "\n[WIN] Empate!!\n");
		}

		for (int n = 0; n < _n_jugadores; n++){
			Logger::Log(std::stringstream() << "[WIN] Jugador: " << n + 1 << " : " << _jugadores[n]->getPoints() << "pts\n");
		}
		Logger::End();

		_estado = ESTADO::FIN;
		break;
	}
	case ESTADO::FIN:{
		Reset();
		_estado = ESTADO::INICIO;
	}
	}
	return 0;
}

void Manager::RepartirCarta(Jugador *jugador) { 
	jugador->setCarta(DarCarta()); 
}

bool Manager::CambioPinte(Carta *carta){
	if (carta->_palo != _pinte._palo)
		return false;
	if (carta->_prioridad < _pinte._prioridad)
		return false;

	if ((_pinte._prioridad <= 14 && carta->_prioridad == 15) ||	(_pinte._prioridad >= 15 && carta->_prioridad == 19)){
		Carta tmp;
		tmp = _pinte;
		_pinte = *carta;
		*carta = tmp;

		return true;
	};

	return false;
}

void Manager::CartaEnMesa(Carta carta) { 
	_cartas_en_juego.push_back(carta); 
}

const RESULTADOS Manager::getResultsPlayer(int player_id){
	RESULTADOS result;

	if (_jugador_inicial == nullptr)
		result.victoria = -1;
	else
		result.victoria = player_id == _jugador_inicial->getID();

	result.puntos = _jugadores[player_id]->getPoints();
	return result;
}

std::string Manager::ShowBaraja(){
	std::ostringstream str;
	for (int i = 0; i < 40; i++){
		str << _baraja[i] << ", ";
		if ((i + 1) % 10 == 0)
			str << std::endl;
	}
	str << std::endl;

	return str.str();
}

std::string Manager::ShowPlayerHands(){
	std::ostringstream str;
	for (int i = 0; i < _n_jugadores; i++){
		str << _jugadores[i]->identificar() << std::endl;
		_jugadores[i]->ShowHand(str);
		str << std::endl;
	}
	return str.str();
}

std::string Manager::ShowPinte(){
	std::ostringstream str;
	str << "Pinte: " << _pinte;
	return str.str();
}

void Manager::ShowTraining(){
	for (int n = 0; n < _n_jugadores; n++){
		if (Jugador_RL *DQL = dynamic_cast<Jugador_RL *>(_jugadores[n]))
			DQL->showTraining();
	}
}

void Manager::AnadirCarta(int carta, int jugador){
	if (jugador < _n_jugadores)
		_jugadores[jugador]->setCarta(Carta(carta));
}

void Manager::SetPinte(int carta) { 
	_pinte = Carta(carta); 
}

void Manager::CrearJugador(TYPE_JUGADOR tipo, int id){
	switch (tipo){
	case TYPE_JUGADOR::HUMANO:{
		Jugador *p = new Jugador_Humano(id, this);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_RND:{
		Jugador *p = new Jugador_RND(id, this);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_QLT:{
		Jugador *p = new Jugador_QLT(id, this);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_DQL:{
		#ifdef TRAIN
			Jugador *p = new Jugador_DQL(id, this, PARTIDAS * _rondas_max);
		#else
			Jugador *p = new Jugador_DQL(id, this, PARTIDAS * _rondas_max, MODEL, false);
		#endif
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_DQL_OPPONENT:	{
		#ifndef OPPONENT
		#define OPPONENT ""
		#endif

		Jugador *p = new Jugador_DQL(id, this, PARTIDAS * _rondas_max, OPPONENT, false);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_DQL_mem:{
		#ifdef TRAIN
			Jugador *p = new Jugador_DQL_mem(id, this, PARTIDAS * _rondas_max);
		#else
			Jugador *p = new Jugador_DQL_mem(id, this, PARTIDAS * _rondas_max, MODEL, false);
		#endif
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_DQL_OPPONENT_mem:	{
		#ifndef OPPONENT
		#define OPPONENT ""
		#endif

		Jugador *p = new Jugador_DQL_mem(id, this, PARTIDAS * _rondas_max, OPPONENT, false);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_RULES_BS:{
		Jugador *p = new Jugador_BS(id, this);
		_jugadores.push_back(p);
		break;
	}
	case TYPE_JUGADOR::AI_RULES_AV:{
		Jugador *p = new Jugador_AV(id, this);
		_jugadores.push_back(p);
		break;
	}
	}
}

Carta Manager::DarCarta(){
	int carta;
	if (_cartas_restantes == 0)
		return _pinte;

	do{
		carta = _UID(_rnd_generator);
	} while (_baraja[carta] != 0);
	_baraja[carta] = 1;

	_cartas_restantes--;
	return Carta(carta);
}

int Manager::Resolver(){
	Carta mejor = _cartas_en_juego[0];
	int id_ganador = _jugador_inicial->getID();
	for (int n = 1; n < _n_jugadores; n++){
		int indice = (n + _jugador_inicial->getID()) % _n_jugadores;
		id_ganador = AplicarVentaja(&mejor, _cartas_en_juego[n]) ? _jugadores[indice]->getID() : id_ganador;
	}

	for (int n = 0; n < _cartas_en_juego.size(); n++){
		_jugadores[id_ganador]->addPuntos(_cartas_en_juego[n]._puntuacion);
	}
	return id_ganador;
}

bool Manager::AplicarVentaja(Carta *mejor, Carta actual){
	if (actual._palo != _pinte._palo && actual._palo != mejor->_palo)
		return false;
	if (actual._palo == _pinte._palo)
		actual._prioridad -= 10;

	Carta tmp = *mejor;
	if (tmp._palo == _pinte._palo)
		tmp._prioridad -= 10;

	if (actual._prioridad < tmp._prioridad){
		*mejor = actual;
		return true;
	}

	return false;
}

void Manager::Reset(){
	std::fill(std::begin(_baraja), std::end(_baraja), 0);
	_ronda_actual = 0;
	_cartas_restantes = 40;

	for (int n = 0; n < _n_jugadores; n++){
		_jugadores[n]->Reset();
	}
	_j_inicial = (_j_inicial + 1) % _n_jugadores;
	_jugador_inicial = _jugadores[_j_inicial];
	// _rnd_generator.seed(52);
}
