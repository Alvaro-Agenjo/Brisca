#include "Jugador_RL.h"
#include "Manager.h"
#include "Agent_properties.h"
#include <filesystem>

Jugador_RL::Jugador_RL(int id, Manager *mg, bool train) : Jugador(id, mg), _entrenando(train) 
{}

Jugador_RL::~Jugador_RL(){
    agente.release();
    py::finalize_interpreter();
}


void Jugador_RL::Reset() {
	_accion_pasada = Accion::NULA;
	_recompensa = 0;
	Jugador::Reset();
}

void Jugador_RL::GetRecompensa(std::vector<Carta> cartas, bool win, int jugada){
    if (!_entrenando) return;

    double puntos_ronda = 0.0;
    double puntos_mios = 0.0;
    for (int n = 0; n < cartas.size(); n++) {
        puntos_ronda += cartas[n]._puntuacion;
        if (n == jugada)
            puntos_mios += cartas[n]._puntuacion;
    }

    double recompensa = (win ? 1.0 : -1.0) * (puntos_ronda / 11.0);
    bool jugue_pinte = cartas[jugada]._palo == _manager->getPinte()._palo;

    if (win) {
        if (!jugue_pinte && puntos_mios > 0)
            recompensa += 0.25;                // good win without spending trump
        if (jugue_pinte && puntos_mios == 0 && puntos_ronda > 0)
            recompensa += 0.15;                // reward low-trump captures
    } else {
        if (puntos_mios == 0)
            recompensa += 0.1;                 // losing with zero-value card is less bad
        if (jugue_pinte && puntos_mios > 0)
            recompensa -= 0.25;                // penalize losing a trump trick with value
    }

    _recompensa = recompensa;
}



void Jugador_RL::showTraining(){
    #ifdef TRAIN
    agente.attr("save_model")(py::cast(MODEL_NAME), PARTIDAS + _initial_steps);
    agente.attr("show_training")(py::cast(MODEL_NAME), PARTIDAS + _initial_steps);
    #endif
}
