#pragma once

enum class ESTADO { INICIO, REPARTO, RONDA, RESOLUCION, ULTIMAS_RONDA, RECUENTO, FIN};
enum class TYPE_JUGADOR { HUMANO, AI_RND, AI_RULES_BS, AI_RULES_AV, AI_QLT, AI_DQL, AI_DQL_OPPONENT, AI_DQL_mem, AI_DQL_OPPONENT_mem };
enum class PALO { OROS, COPAS, ESPADAS, BASTOS, UNDEFINED };
enum class Accion {PLAY_1, PLAY_2, PLAY_3, NULA};

struct RESULTADOS {
	int victoria;
	int puntos;
};
