#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Data_types.h"

// Seleccion de configuracion
// #define DQL_MEM

#define RULES_BS


// #define INITIAL_RUN
// #define DEBUG
// #define LOG
// #define PYTHON_DEBUG

// #define RULESvsRND
// #define RULESvsRULES
// #define DQLvsHUM
// #define QLTvsHUM
// #define HUMvsRND
// #define RNDvsRND
// #define HUMvsHUM
// #define TEST_CONFIG

// #define RND_TRAIN
// #define RND_TRAIN_v2
// #define RND_TRAIN_v3
// #define RND_TRAIN_v4
// #define RND_TRAIN_v5

// #define RND_TRAIN_init
// #define RND_TRAIN_continue
#define RND_TRAIN_refine

// #define RULES_TRAIN_init
// #define RULES_TRAIN_continue
// #define RULES_TRAIN_refine

// #define DQL_TRAIN_init
// #define DQL_TRAIN_continue

// #define RND_TESTv1
// #define RND_TESTv2
// #define RND_TESTv3
// #define RND_TESTv4

// #define RND_TESTv5
// #define RULES_TESTv1
// #define DQL_TESTv5




#ifdef RULES_BS
	#define RULES TYPE_JUGADOR::AI_RULES_BS
#else
	#define RULES TYPE_JUGADOR::AI_RULES_AV
#endif
// Definicion de configuraciones
#ifdef TEST_CONFIG
	#define PARTIDAS 1
	#define NUM_JUGADORES 10
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::HUMANO};
#endif // TEST_CONFIG

#ifdef HUMvsHUM
	#define PARTIDAS 1
	#define LOG
	#define DEBUG
	#define NUM_JUGADORES 2
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::HUMANO};
#endif // HUMvsHUM

#ifdef RNDvsRND
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_RND,TYPE_JUGADOR::AI_RND};
#endif // RNDvdRND

#ifdef HUMvsRND
	#define PARTIDAS 1
	#define LOG
	#define DEBUG
	#define NUM_JUGADORES 2
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::HUMANO,TYPE_JUGADOR::AI_RND};
#endif // HUMvdRND

#ifdef QLTvsHUM
	#define PARTIDAS 1
	#define LOG
	#define DEBUG
	#define NUM_JUGADORES 2
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_QLT,TYPE_JUGADOR::HUMANO};
#endif // QLTvsHUM

#ifdef DQLvsHUM
	#define PARTIDAS 1
	#define LOG
	#define DEBUG
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v250_205000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::HUMANO};

	#define INFO PARTIDAS
#endif // DQLvsHUM

#ifdef RULESvsRND
	#define PARTIDAS 10000
	// #define LOG
	// #define DEBUG
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v250_205000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {RULES, TYPE_JUGADOR::AI_RND};

	#define INFO PARTIDAS
#endif // DQLvsHUM

#ifdef RULESvsRULES
	#define PARTIDAS 10000
	// #define LOG
	// #define DEBUG
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v250_205000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_RULES_AV, TYPE_JUGADOR::AI_RULES_BS};

	#define INFO PARTIDAS
#endif // DQLvsHUM

#ifdef RND_TRAIN
	#define TRAIN
	#define PARTIDAS 10000
	#define NUM_JUGADORES 2
	#define MODEL_NAME "brisca_v1"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
#endif // RND_TRAIN

#ifdef RND_TRAIN_v2
	#define TRAIN
	#define PARTIDAS 20000
	#define NUM_JUGADORES 2
	// #define MODEL_NAME "brisca_v2"
	#define MODEL_NAME "brisca_v5"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
#endif // RND_TRAIN_v2

#ifdef RND_TRAIN_v3
	#define TRAIN
	#define PARTIDAS 10000
	#define NUM_JUGADORES 2
	#define MODEL_NAME "brisca_v12"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
#endif // RND_TRAIN_v3

#ifdef RND_TRAIN_v4
	#define TRAIN
	#define PARTIDAS 50000
	#define NUM_JUGADORES 2
	#define MODEL_NAME "brisca_v110"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO 5000
#endif // RND_TRAIN_v4

#ifdef RND_TRAIN_v5
	#define TRAIN
	#define PARTIDAS 10000
	#define NUM_JUGADORES 2
	#define MODEL_NAME "brisca_v113"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO 1000
#endif // RND_TRAIN_v5



#ifdef RND_TRAIN_init
	#define TRAIN
	#define INITIAL_RUN
	#define PARTIDAS 200000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem,TYPE_JUGADOR::AI_RND};
	#else
		#define MODEL_NAME "brisca_v201"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
	#endif

	#define INFO 50000
#endif // RND_TRAIN_init

#ifdef RND_TRAIN_continue
	#define TRAIN
	#define PARTIDAS 100000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem,TYPE_JUGADOR::AI_RND};
	#else
		#define MODEL_NAME "brisca_v201"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
	#endif

	#define INFO 10000
#endif // RND_TRAIN_continue

#ifdef RND_TRAIN_refine
	#define TRAIN
	#define PARTIDAS 100000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem,TYPE_JUGADOR::AI_RND};
	#else
		#define MODEL_NAME "brisca_v201"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
	#endif

	#define INFO 10000
#endif // RND_TRAIN_refine



#ifdef RULES_TRAIN_init
	#define TRAIN
	#define INITIAL_RUN
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem, RULES};
	#else
		#define MODEL_NAME "brisca_v200"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, RULES};
	#endif

	#define INFO 10000
#endif // RULES_TRAIN_init

#ifdef RULES_TRAIN_continue
	#define TRAIN
	#define PARTIDAS 100000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem, RULES};
	#else
		#define MODEL_NAME "brisca_v200"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, RULES};
	#endif

	#define INFO 10000
#endif // RULES_TRAIN_continue

#ifdef RULES_TRAIN_refine
	#define TRAIN
	#define PARTIDAS 50000
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v252"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem, RULES};
	#else
		#define MODEL_NAME "brisca_v200"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, RULES};
	#endif

	#define INFO 10000
#endif // RULES_TRAIN_refine



#ifdef DQL_TRAIN_init
	#define TRAIN
	#define INITIAL_RUN
	#define PARTIDAS 50
	#define NUM_JUGADORES 2

	#ifdef DQL_MEM
		#define MODEL_NAME "brisca_v250"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem, TYPE_JUGADOR::AI_DQL_OPPONENT_mem};
	#else
		#define MODEL_NAME "brisca_v200"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, TYPE_JUGADOR::AI_DQL_OPPONENT};
	#endif

	#define INFO 10
#endif // RND_TRAIN_init

#ifdef DQL_TRAIN_continue
	#define TRAIN
	#define PARTIDAS 50
	#define NUM_JUGADORES 2
	#define MODEL_NAME "brisca_v121"
	#define OPPONENT "brisca_v110_100000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, TYPE_JUGADOR::AI_DQL_OPPONENT};

	#define INFO 10
#endif // RND_TRAIN_continue



#ifdef RND_TESTv1
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v101_10000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO PARTIDAS
#endif // RND_TESTv1

#ifdef RND_TESTv2
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v102_100000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO PARTIDAS
#endif // RND_TESTv2

#ifdef RND_TESTv3
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v103_50000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO PARTIDAS
#endif // RND_TESTv3

#ifdef RND_TESTv4
	#define PARTIDAS 1000
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v110_100000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};

	#define INFO PARTIDAS
#endif // RND_TESTv4


#ifdef RND_TESTv5
	#define PARTIDAS 10000
	// #define LOG
	#define NUM_JUGADORES 2
	#define OPPONENT "rnd"

	#ifdef DQL_MEM
		#define MODEL "brisca_v252_1200000.pth"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem,TYPE_JUGADOR::AI_RND};
	#else
		#define MODEL "brisca_v200_430000.pth"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL,TYPE_JUGADOR::AI_RND};
	#endif

	#define INFO PARTIDAS
#endif // RND_TESTv5

#ifdef RULES_TESTv1
	#define PARTIDAS 10000
	#define NUM_JUGADORES 2
	#define OPPONENT "rules"
	// #define LOG
	#ifdef DQL_MEM
		#define MODEL "brisca_v252_950000.pth"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem, RULES};
	#else
		#define MODEL "brisca_v200_430000.pth"
		constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL, RULES};
	#endif

	#define INFO PARTIDAS
#endif // RULES_TESTv1

#ifdef DQL_TESTv5
	#define PARTIDAS 10000
	#define NUM_JUGADORES 2
	#define MODEL "brisca_v252_950000.pth"
	#define OPPONENT "brisca_v252_750000.pth"
	constexpr TYPE_JUGADOR jugadores[NUM_JUGADORES] = {TYPE_JUGADOR::AI_DQL_mem,
													TYPE_JUGADOR::AI_DQL_OPPONENT_mem};

	#define INFO PARTIDAS
#endif // DQL_TESTv5

// Constantes
constexpr char LOG_PATH[] = "log";
// constexpr char DISPLAY_PATH[] = "Figuras";
#endif // PROPERTIES_H
