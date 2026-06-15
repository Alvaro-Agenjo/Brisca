#include "Jugador_DQL.h"
#include "Agent_properties.h"
#include "Manager.h"
#include <filesystem>

Jugador_DQL::Jugador_DQL(int id, Manager *mg, int decisions, std::string model,
                         bool train)
    : Jugador_RL(id, mg, train) {
  try {
    static py::scoped_interpreter guard{}; // Inicializa el intérprete de Python
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("insert")(0, PYTHON_PATH_ENV);
    sys.attr("path").attr("insert")(0, PYTHON_SCRIPTS_PATH);

    py::module_ modulo_ia = py::module_::import("DQL");
#ifdef PYTHON_DEBUG
    modulo_ia.attr("activar_debug")();
    modulo_ia.attr("DEBUG") = true;
#endif

    if (train) {
      float epsilon_dec = (float)(EPSILON_START - EPSILON_END) /
                          (decisions * EPSILON_PERCENTAGE);
      this->agente = modulo_ia.attr("Agent")(
          LEARNING_RATE, GAMMA, EPSILON_START,
          py::make_tuple(sizeof(ESTADO_Q) / sizeof(float)), BATCH_SIZE, 3,
          NEURON_fc1, NEURON_fc2, TRANSFER_WEIGHTS_PER, MAX_MEMORY_SIZE,
          EPSILON_END, epsilon_dec);

#ifndef INITIAL_RUN
      agente.attr("load_model")("brisca_v201_300000.pth");
      _initial_steps = 300000;
#endif
    } else {
      this->agente = modulo_ia.attr("Agent")(
          LEARNING_RATE, GAMMA, 0,
          py::make_tuple(sizeof(ESTADO_Q) / sizeof(float)), BATCH_SIZE, 3,
          NEURON_fc1, NEURON_fc2, TRANSFER_WEIGHTS_PER, MAX_MEMORY_SIZE, 0, 0);

      agente.attr("load_model")(model);
    }

  } catch (const py::error_already_set &e) {
    std::cerr << "Error de Python en el constructor: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error de C++ en el constructor: " << e.what() << std::endl;
  }
}

void Jugador_DQL::JugarTurno() {
  // Choose action

  py::object accion =
      agente.attr("choose_action")(py::cast(state2vec(_actual)), _mano.size());
  int selected_card = accion.cast<int>();

  Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
  Logger::Viz(std::stringstream() << selected_card + 1 << "->");
  JugarCarta(selected_card);

  _accion_pasada = static_cast<Accion>(selected_card);
}

std::string Jugador_DQL::identificar() {
  std::ostringstream str;
  str << "[PLAYER] IA_DQL, id: " << Jugador::identificar();
  return str.str();
}

void Jugador_DQL::Reset() {
  _actual = ESTADO_Q();
  try{
        agente.attr("record_episode_points")(py::cast(getPoints()));
    } catch (const py::error_already_set &e) {
        std::cerr << "Error recording episode points: " << e.what() << std::endl;
    }
  Jugador_RL::Reset();
}

void Jugador_DQL::learn(bool last_episode, bool winner) {
  if (!_entrenando)
    return;
  // Get observation
  ESTADO_Q future = getEstado();

  if (_accion_pasada != Accion::NULA) {

    agente.attr("store_transition")(
        py::cast(state2vec(_actual)), static_cast<int>(_accion_pasada),
        _recompensa, py::cast(state2vec(future)), last_episode);

    agente.attr("learn")();
  }

  if (!last_episode)
    _actual = future;
}

ESTADO_Q Jugador_DQL::getEstado() {
  ESTADO_Q estado;

  PALO palo_pinte = _manager->getPinte()._palo;
  PALO palo_mesa = _manager->getMesa().size() ? _manager->getMesa()[0]._palo
                                              : PALO::UNDEFINED;

  auto populate_card = [&](float *c, const Carta &card) {
    c[0] = static_cast<float>(card._puntuacion) / 11.0f;
    c[1] = static_cast<float>(20 - card._prioridad) / 10.0f;

    if (card._palo == palo_pinte) {
      c[2] = 1.0f;
    } else if (palo_mesa != PALO::UNDEFINED && card._palo == palo_mesa) {
      c[3] = 1.0f;
    } else {
      c[4] = 1.0f;
    }
  };

  if (_mano.size() > 0) {
    populate_card(estado.c1, _mano[0]);
  }
  if (_mano.size() > 1) {
    populate_card(estado.c2, _mano[1]);
  }
  if (_mano.size() > 2) {
    populate_card(estado.c3, _mano[2]);
  }

  if (_manager->getMesa().size()) {
    const Carta &mesa_card = _manager->getMesa()[0];
    estado.mesa[0] = static_cast<float>(mesa_card._puntuacion) / 11.0f;
    estado.mesa[1] = static_cast<float>(20 - mesa_card._prioridad) / 10.0f;
    if (mesa_card._palo == palo_pinte) {
      estado.mesa[2] = 1.0f;
    } else {
      estado.mesa[3] = 1.0f;
    }
  }

  return estado;
}

std::vector<float> Jugador_DQL::state2vec(const ESTADO_Q &estado) {
  const float *inicio = reinterpret_cast<const float *>(&estado);
  return std::vector<float>(inicio, inicio + sizeof(ESTADO_Q) / sizeof(float));
}