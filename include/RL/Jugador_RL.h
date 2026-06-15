#pragma once
#include "Jugador.h"
#include <pybind11/embed.h> // Para embeber el intérprete
#include <pybind11/numpy.h> // Para pasar estados como arrays de numpy
#include <pybind11/stl.h> //  Para pasar estructuras de datos de C++ a Python




namespace py = pybind11;

class Jugador_RL : public Jugador{
public:
	Jugador_RL(int id, Manager* mg, bool train = true);
	virtual ~Jugador_RL();

	virtual std::string identificar() = 0;
	virtual void Reset();
	virtual void JugarTurno() = 0;	
	virtual void GetRecompensa(std::vector<Carta> cartas, bool win, int jugada);
	virtual void learn(bool last_episode = false, bool winner = false) = 0;
	

	void showTraining();
protected:

    py::object agente; // Objeto Python que representa al agente DQL
	bool _entrenando;
	float _recompensa = 0;

	Accion _accion_pasada = Accion::NULA;
	
	long _initial_steps = 0;
};

