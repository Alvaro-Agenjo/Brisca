#include "Jugador_DQL_mem.h"
#include "Manager.h"
#include "Agent_properties.h"
#include <filesystem>

Jugador_DQL_mem::Jugador_DQL_mem(int id, Manager *mg, int decisions, std::string model, bool train) : Jugador_RL(id, mg, train) {
    try{
        static py::scoped_interpreter guard{}; // Inicializa el intérprete de Python
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("insert")(0, PYTHON_PATH_ENV);
        sys.attr("path").attr("insert")(0, PYTHON_SCRIPTS_PATH);
        
        py::module_ modulo_ia = py::module_::import("DQL");
        #ifdef PYTHON_DEBUG
            modulo_ia.attr("activar_debug")();
            modulo_ia.attr("DEBUG") = true;
        #endif

        if (train){
            float epsilon_dec = (float)(EPSILON_START - EPSILON_END) / (decisions * EPSILON_PERCENTAGE);
            // epsilon_dec = 0.0000074;
            this->agente = modulo_ia.attr("Agent")(LEARNING_RATE, GAMMA, EPSILON_START, 
                py::make_tuple(sizeof(ESTADO_Q_mem)/sizeof(float)), BATCH_SIZE, 3, 
                NEURON_fc1, NEURON_fc2, TRANSFER_WEIGHTS_PER , MAX_MEMORY_SIZE, EPSILON_END, epsilon_dec);
                
                #ifndef INITIAL_RUN    
                    agente.attr("load_model")("brisca_v252_1150000.pth");
                    _initial_steps = 1150000;
                #endif
        }
        else {
            this->agente = modulo_ia.attr("Agent")(LEARNING_RATE, GAMMA, 0, 
                py::make_tuple(sizeof(ESTADO_Q_mem)/sizeof(float)), BATCH_SIZE, 3, 
                NEURON_fc1, NEURON_fc2, TRANSFER_WEIGHTS_PER , MAX_MEMORY_SIZE, 0, 0);

            agente.attr("load_model")(model);
        }       
            
    } 
    catch (const py::error_already_set& e) {
        std::cerr << "Error de Python en el constructor: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error de C++ en el constructor: " << e.what() << std::endl;
    }
}


void Jugador_DQL_mem::JugarTurno(){
    //Choose action
    
    py::object accion = agente.attr("choose_action")(py::cast(state2vec(_actual)), _mano.size());
    int selected_card = accion.cast<int>();


    Logger::Viz(std::stringstream() << "Jugador " << _id + 1 << ":  ");
	Logger::Viz(std::stringstream() << selected_card + 1 << "->");
	JugarCarta(selected_card);

    _accion_pasada = static_cast<Accion>(selected_card );
}

std::string Jugador_DQL_mem::identificar(){
    std::ostringstream str;
	str << "[PLAYER] IA_DQL_mem, id: " << Jugador::identificar();
	return str.str();
}

void Jugador_DQL_mem::Reset() {
    _actual = ESTADO_Q_mem(); 
    memset(memoria, 1, sizeof(memoria));

    try{
        agente.attr("record_episode_points")(py::cast(getPoints()));
    } catch (const py::error_already_set &e) {
        std::cerr << "Error recording episode points: " << e.what() << std::endl;
    }
    
	Jugador_RL::Reset();
}

void Jugador_DQL_mem::GetRecompensa(std::vector<Carta> cartas, bool win, int jugada){
    if (!_entrenando) return;

    double puntos_ronda = 0.0;
    double puntos_mios = 0.0;
    for (int n = 0; n < cartas.size(); n++) {
        puntos_ronda += cartas[n]._puntuacion;
        if (n == jugada)
            puntos_mios += cartas[n]._puntuacion;

        if (n != jugada)
            memoria[cartas[n]._posicion] = 0.0f;
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

void Jugador_DQL_mem::learn(bool last_episode, bool winner){
    if (!_entrenando) return;
    // Get observation
    ESTADO_Q_mem future = getEstado();
    
    if (_accion_pasada != Accion::NULA) {

        // if(last_episode){
        //     _recompensa = winner? _recompensa + 25.0 : _recompensa - 25.0;
        // }
        agente.attr("store_transition")(py::cast(state2vec(_actual)), static_cast<int>(_accion_pasada), _recompensa,
            py::cast(state2vec(future)), last_episode);

        agente.attr("learn")();
    }

    if(!last_episode)
        _actual = future;
}

void Jugador_DQL_mem::setCarta(Carta carta){
    Jugador::setCarta(carta);

    memoria[carta._posicion] = 0.0f;    
}

ESTADO_Q_mem Jugador_DQL_mem::getEstado(){
    ESTADO_Q_mem estado;

    PALO pinte_palo = _manager->getPinte()._palo;
    PALO mesa_palo = _manager->getMesa().size() ? _manager->getMesa()[0]._palo : PALO::UNDEFINED;

    // Mano de jugador
    auto populate_card = [&](float* c, const Carta& card) {
        c[0] = static_cast<float>(card._puntuacion) / 11.0f;
        c[1] = static_cast<float>(20 - card._prioridad) / 10.0f;
        
        if (card._palo == pinte_palo) {
            c[2] = 1.0f;
        } else if (mesa_palo != PALO::UNDEFINED && card._palo == mesa_palo) {
            c[3] = 1.0f;
        } else {
            c[4] = 1.0f;
        }
    };

    if (_mano.size() > 0) {
        populate_card(estado.c1, _mano[0]);
    }
    if (_mano.size() > 1){
        populate_card(estado.c2, _mano[1]);
    }
    if (_mano.size() > 2){
        populate_card(estado.c3, _mano[2]);
    }

    // mesa
    if (_manager->getMesa().size()) {
        const Carta& mesa_card = _manager->getMesa()[0];
        estado.mesa[0] = static_cast<float>(mesa_card._puntuacion) / 11.0f;
        estado.mesa[1] = static_cast<float>(20 - mesa_card._prioridad) / 10.0f;
        if (mesa_card._palo == pinte_palo) {
            estado.mesa[2] = 1.0f;
        } else {
            estado.mesa[3] = 1.0f;
        }
    }
    
    //memoria
    memcpy(estado.mem, memoria, sizeof(memoria));

    //turno
    if (mesa_palo == PALO::UNDEFINED)
        estado.primero = 1.0;

    return estado;
}


std::vector<float> Jugador_DQL_mem::state2vec(const ESTADO_Q_mem &estado) {
    const float* inicio = reinterpret_cast<const float*>(&estado);
    return std::vector<float>(inicio, inicio + sizeof(ESTADO_Q_mem)/sizeof(float));
}