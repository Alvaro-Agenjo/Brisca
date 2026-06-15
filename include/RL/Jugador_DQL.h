#pragma once
#include "Jugador_RL.h"

struct ESTADO_Q {
  float c1[5] = {}; // puntos normalizados, prioridad normalizada, palo relativo
                    // (pinte, mesa, otro)
  float c2[5] = {};
  float c3[5] = {};
  float mesa[4] = {}; // puntos normalizados, prioridad normalizada, palo
                      // relativo (pinte, otro)
};

namespace py = pybind11;

class Jugador_DQL : public Jugador_RL {
public:
  Jugador_DQL(int id, Manager *mg, int episodes, std::string model = "non",
              bool train = true);
  std::string identificar() override;
  void Reset() override;
  void JugarTurno() override;
  void learn(bool last_episode = false, bool winner = false);

private:
  ESTADO_Q getEstado();
  std::vector<float> state2vec(const ESTADO_Q &estado);

  ESTADO_Q _actual;
};
