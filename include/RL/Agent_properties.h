#define GAMMA 0.99
#define MAX_MEMORY_SIZE 150000
#define BATCH_SIZE 128

#define NEURON_fc1 512
#define NEURON_fc2 512

#define TRANSFER_WEIGHTS_PER 3000


#if defined(RND_TRAIN_init) || defined(RULES_TRAIN_init) || defined(DQL_TRAIN_init)
    #define EPSILON_START 1.0
    #define EPSILON_END 0.05
    #define EPSILON_PERCENTAGE 0.8
    #define LEARNING_RATE 0.0005


#elif defined(RND_TRAIN_continue) || defined(RULES_TRAIN_continue) || defined(DQL_TRAIN_continue) 
    #define EPSILON_START 0.6
    #define EPSILON_END 0.05
    #define EPSILON_PERCENTAGE 0.6
    #define LEARNING_RATE 0.0005
#else
    #define EPSILON_START 0.2
    #define EPSILON_END 0.05
    #define EPSILON_PERCENTAGE 0.15
    #define LEARNING_RATE 0.001
#endif 