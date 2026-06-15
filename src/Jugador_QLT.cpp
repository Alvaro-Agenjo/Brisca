#include "Jugador_QLT.h"
#include "Manager.h"

Jugador_QLT::Jugador_QLT(int id, Manager* mg) : Jugador(id, mg), _rnd_generator(std::random_device{}()), _URD(0.0, 1.0)
{}

void Jugador_QLT::JugarTurno() {
	constexpr double epsilon = 0.1; // exploracion/explotacion

	ESTADO_QT actual = getEstado();

	q_learn(_pasado, _accion_pasada, _recompensa, actual);
	Accion accion = _URD(_rnd_generator) < epsilon ? pickRandomAction() : pickBestAction(actual);

	int selected_card = 0;
	switch (accion) {
	case Accion::PLAY_1:
		selected_card = 0;
		break;
	case Accion::PLAY_2:
		selected_card = 1;
		break;
	case Accion::PLAY_3:
		selected_card = 2;
		break;
	}
	
	
	Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
	Logger::Viz(std::stringstream() << selected_card + 1 << "->");
	JugarCarta(selected_card);

	_accion_pasada = accion;
	_pasado = actual;
}

std::string Jugador_QLT::identificar() {
	std::ostringstream str;
	str << "[PLAYER] IA_QLT, id: " << Jugador::identificar();
	return str.str();
}

void Jugador_QLT::Reset() {
	_rnd_generator.seed(std::random_device{}());
	_accion_pasada = Accion::NULA;
	_recompensa = 0;
	Jugador::Reset();
}

// void Jugador_QLT::GetRecompensa(std::vector<Carta> cartas, bool win, int jugada){
// 	double recompensa = 0;
// 	if (!win) {
// 		recompensa = -cartas[jugada]._puntuacion;
// 	}
// 	else {
// 		for (int n = 0; n < cartas.size(); n++) {
// 			recompensa += cartas[n]._puntuacion;
// 		}
// 	}
// 	_recompensa = recompensa;
// }

void Jugador_QLT::q_learn(ESTADO_QT estado, Accion accion, double recompensa, ESTADO_QT futuro){
	if (accion == Accion::NULA) return ;
	constexpr double learning_rate = 0.1;
	constexpr double long_range_rate = 0.99;
	const double predict = _Qmem[{estado, accion}];
	const double potential_best = bestPosiblleReward(futuro);
	double target = recompensa + long_range_rate * potential_best;
	_Qmem[{estado, accion}] += learning_rate * (target - predict);
}

ESTADO_QT Jugador_QLT::getEstado(){
	ESTADO_QT estado;
	for (int n = 0; n < _mano.size(); n++) {
		estado.mano.push_back(_mano[n]._posicion);
	}
	for (int n = 0; n < _manager->getMesa().size(); n++) {
		estado.mesa.push_back(_manager->getMesa()[n]._posicion);
	}

	estado.pinte = _manager->getPinte()._palo;
	
	return estado;
}

Accion Jugador_QLT::pickBestAction(ESTADO_QT estado){
	std::vector<double> weights;
	std::vector<Accion> acciones;
	switch (_mano.size()) {
	case 1:
		return Accion::PLAY_1;
	case 2: {
		acciones.push_back(Accion::PLAY_1);
		acciones.push_back(Accion::PLAY_2);
		break;
	}
	case 3: {
		acciones.push_back(Accion::PLAY_1);
		acciones.push_back(Accion::PLAY_2);
		acciones.push_back(Accion::PLAY_3);
		break;
	}
	}

	//Actializa la probabilidad de realizar cada tipo de accion
	for (auto act : acciones)
		weights.push_back(_Qmem[{estado, act}]);
	auto [smallest_pos, biggest_pos] = std::minmax_element(weights.cbegin(), weights.cend());
	auto smallest = *smallest_pos;
	auto biggest = *biggest_pos;

	std::transform(weights.begin(), weights.end(), weights.begin(), [biggest](const double& x) {return x == biggest ? 1.0 : 0.0; });

	std::discrete_distribution<int> dd{ weights.cbegin(), weights.cend() };
	return acciones[dd(_rnd_generator)];
}

Accion Jugador_QLT::pickRandomAction() {
	std::uniform_int_distribution<> _UID(0, _mano.size() - 1);
	int card = _UID(_rnd_generator);
	switch (card) {
	case 0: 
		return Accion::PLAY_1;
	case 1:
		return Accion::PLAY_2;
	case 2: 
		return Accion::PLAY_3;
	default:
		return Accion::NULA;
	}
}
double Jugador_QLT::bestPosiblleReward(ESTADO_QT actual){
	double best = 0.0;
	for (const auto& element : _Qmem) {
		ESTADO_QT estado = element.first.first;
		double value = element.second;

		if (actual == estado) {
			if (value > best)
				best = value;
		}
	}
	return best;
}

bool Equal(const ESTADO_QT& e1, const ESTADO_QT& e2)
{
	return (e1.mano[0] == e2.mano[0] && e1.mano[1] == e2.mano[1] && e1.mano[2] == e2.mano[2] && e1.pinte == e2.pinte && e1.mesa == e2.mesa);
}
