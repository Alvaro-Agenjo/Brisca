#pragma once

#include "Jugador_RL.h"
// #include <pybind11/embed.h> // Para embeber el intérprete
// #include <pybind11/numpy.h> // Para pasar estados como arrays de numpy
// #include <pybind11/stl.h> //  Para pasar estructuras de datos de C++ a Python

struct ESTADO_Q_mem {
  float c1[5] = {}; // puntos normalizados, prioridad normalizada, palo relativo (pinte, mesa, otro)
  float c2[5] = {};
  float c3[5] = {};
  float mesa[4] = {}; // puntos normalizados, prioridad normalizada, palo relativo (pinte, otro)
  float mem[40] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  float primero = -1.0; //indica si soy el primero en jugar o no
};

namespace py = pybind11;

class Jugador_DQL_mem : public Jugador_RL {
public:
  Jugador_DQL_mem(int id, Manager *mg, int episodes, std::string model = "non",
                  bool train = true);
  std::string identificar() override;
  void JugarTurno() override;
  void GetRecompensa(std::vector<Carta> cartas, bool win, int jugada) override;
  void learn(bool last_episode = false, bool winner = false);
  void setCarta(Carta carta) override;
  void Reset() override;

private:
  ESTADO_Q_mem getEstado();
  std::vector<float> state2vec(const ESTADO_Q_mem &estado);

  ESTADO_Q_mem _actual;
  float memoria[40] = {};
};
