#pragma once
#include "Jugador.h"

#include <random>
#include <map>

struct ESTADO_QT {
	std::vector<int> mano;
	std::vector<int> mesa;
	PALO pinte;

	auto operator<=>(const ESTADO_QT&) const = default;
};
bool Equal(const ESTADO_QT& e1, const ESTADO_QT& e2);

class Jugador_QLT : public Jugador{
public:
	Jugador_QLT(int id, Manager* mg);
	void JugarTurno() override;
	std::string identificar() override;
	void Reset() override;
	// void GetRecompensa(std::vector<Carta> cartas, bool win, int jugada) override;
private:
	ESTADO_QT getEstado();
	Accion pickBestAction(ESTADO_QT estado);
	Accion pickRandomAction();
	double bestPosiblleReward(ESTADO_QT estado);

	
	void q_learn(ESTADO_QT estado, Accion accion, double recompensa, ESTADO_QT futuro);

	ESTADO_QT _pasado; Accion _accion_pasada = Accion::NULA;
	double _recompensa = 0;
	using mem = std::map<std::pair<ESTADO_QT, Accion>, double>;
	mem _Qmem;
	
	std::mt19937 _rnd_generator;
	std::uniform_real_distribution<double> _URD;
};